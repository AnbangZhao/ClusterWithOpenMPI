#include <fstream>
#include "slave.h"
#include "algorithm.h"
#include "twoDPoints.h"

slave::slave(const char *name, int sNum, int r, string t) {
    type = t;
    processorName = name;
    slaveNum = sNum;
    rank = r;
    masterID = 0;
    points = NULL;
}

bool slave::recTotalPtsNum() {
    int rc;
    int count = 1;
    rc = MPI_Bcast(&fileSize, count, MPI_LONG, masterID, MPI_COMM_WORLD);
    //cout << "rank: " << rank << " pointNum: " << fileSize << endl;

    if(rc != MPI_SUCCESS){
        cout << "receive Bcast error" << endl;
        return false;
    }

    return true;
}

// read in data points for this slave
// seek to his starting point and read in len lines
bool slave::readPts(string type, string inFile){
    int length = stringHelper::upRoundDivide(fileSize, slaveNum);
    //cout << "length:" <<length << endl;
    int start = (rank - 1) * length;
    ifstream ifs(inFile.c_str());
    if(!ifs.good()){
        cout << "invalid inputfile:" << inFile << endl;
        return NULL;
    }
    string str;

    points = new vector<pointBase*>();

    int lineStart = start;
    // avoid read half way
    // if starts from zero, do not need to seek
    if(lineStart != 0){
        lineStart--;
        ifs.seekg(lineStart, ifs.beg);
        getline(ifs, str);
        // +1 for the '/n'
        lineStart += str.size() + 1;
    }

    vector<string> strVec;
    double x;
    double y;
    while(lineStart < start + length && getline(ifs,str)){
        strVec = algorithm::split(str, " ");
        x = atof(strVec[0].c_str());
        y = atof(strVec[1].c_str());
        //cout << "rank:" << rank << " " << x << "  " << y << endl;

        twoDPoints* onePoint = new twoDPoints(x, y);
        (*points).push_back(onePoint);
        // +1 for the '/n'
        lineStart += str.size() + 1;
    }


    return true;
}

// return true means continue clustering
// return false means terminate
bool slave::recClstCtrs(){
    int bufInfo[2];
    int rc;
    int terminate;

    rc = MPI_Bcast(&terminate, 1, MPI_INT, masterID, MPI_COMM_WORLD);
    if(terminate == 1)
        return false;
    rc = MPI_Bcast(&bufInfo, 2, MPI_INT, masterID, MPI_COMM_WORLD);
    int strLen = bufInfo[0];
    int strNum = bufInfo[1];
    char *buf = new char[strLen * strNum];
    rc = MPI_Bcast(buf, strLen * strNum, MPI_CHAR, masterID, MPI_COMM_WORLD);

    //cout << "in rank:" << rank << endl;
    //stringHelper::printLongBuf(buf, strLen, strNum);

    if(rc != MPI_SUCCESS){
        cout << "receive cluster centers error" << endl;
        cout.flush();
        return true;
    }



    // create new clusters
    createCluster(buf, strLen, strNum);

    delete buf;
    return true;

}

bool slave::createCluster(const char *buf, int strLen, int strNum) {

    string *strArray = convBufToStrs(buf, strLen, strNum);
    if(strArray == NULL){
        cout << "strArray is NULL" << endl;
        cout.flush();
        return false;
    }

    clusterNum = strNum;
    // clean up the cluster space
    cleanUpclsts();
    // cluster num is based on the string numbers master has sent
    clusters = new cluster*[strNum];


    if(type.compare("point") == 0){
        for(int i = 0; i < strNum; i++){
            twoDPoints* centerPnt = new twoDPoints(strArray[i]);
            int estClstSize = (int)(points->size()/strNum + 100);
            clusters[i] = new cluster(centerPnt->fork(), centerPnt->getZero(), estClstSize);
        }
    }
    else{
        cout << "unknown type" << endl;
        return false;
    }


//        for(int i = 0; i < strNum; i++){
//            cout << "cluster " << i << endl;
//            clusters[i]->print();
//        }

//    cout << "converted str:" << endl;
//    for(int i = 0 ; i < strNum; i++){
//        cout << strArray[i] << endl;
//        cout << i << " Length:" << strArray[i].size() << endl;
//    }



    return true;
}

void slave::cleanUpclsts() {
    if (clusters == NULL)
        return;

    // delete all the resource allocated
    for(int i = 0; i < clusterNum; i++){
        delete clusters[i];
    }
    delete clusters;
}

// strLen contains '\0' already
string* slave::convBufToStrs(const char *buf, int strLen, int strNum) {
    if(buf == NULL){
        cout << "buf is NULL, cannot convert to strings" << endl;
        return NULL;
    }


    string *strArray = new string[strNum];
    //char *tmp = new char[strLen + 1];
    for(int i = 0; i < strNum; i++){
        strArray[i] = buf + i * strLen;
    }



    return strArray;
}

bool slave::sendInfo(){
    int ptsNum = (int)(*points).size();
    int rc = 0;
    rc = MPI_Send(&ptsNum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);

    if(rc != MPI_SUCCESS){
        cout << "receive cluster centers error" << endl;
        cout.flush();
        return false;
    }

    return true;
}

void slave::doCluster(){

//    cout << "show points" << endl;
//    for(int i = 0; i < points->size(); i++){
//        cout << (*points)[i]->toString();
//    }
//    cout.flush();


    for(int i = 0; i < points->size(); i++){
        addToNearCluster((*points)[i]);
    }


//    for(int i = 0; i < clusterNum; i++){
//        clusters[i]->updateCenter();
//        clusters[i]->print();
//    }

}

void slave::addToNearCluster(pointBase *point) {
    double minDis = 999999;
    int minPos = -1;

    for(int i = 0; i < clusterNum; i++){
        double tmpDis = clusters[i]->dis(point);
        if(tmpDis < minDis){
            minPos = i;
            minDis = tmpDis;
        }
    }

    if(minPos == -1)
        cerr << "min pos is -1(no dis < 999999 or no available clusters)" << endl;
    else{
        clusters[minPos]->addPoint(point);
    }
}

bool slave::reportInfo() {
    string *strArray = getCtrs();
    int strLen;
    int rc;
    char *buf = stringHelper::genCharArrays(strArray, clusterNum, &strLen);
    int bufInfo[2];
    int bufInfoLen = 2;
    bufInfo[0] = strLen;
    bufInfo[1] = clusterNum;

    rc = MPI_Send(&bufInfo, bufInfoLen, MPI_INT, 0, 1, MPI_COMM_WORLD);
    rc = MPI_Send(buf, bufInfo[0] * bufInfo[1], MPI_CHAR, 0, 1, MPI_COMM_WORLD);

    int* ptsNumArr = new int[clusterNum];
    for(int i = 0; i < clusterNum; i++){
        ptsNumArr[i] = clusters[i]->getPtsNum();
    }

    rc = MPI_Send(ptsNumArr, clusterNum, MPI_INT, 0, 1, MPI_COMM_WORLD);

    delete []buf;
    delete []strArray;
    delete []ptsNumArr;

    return true;
}

string* slave::getCtrs(){
    string* strArray = new string[clusterNum];
    for(int i = 0; i < clusterNum; i++){
        strArray[i] = clusters[i]->getSum();
    }

    return strArray;
}