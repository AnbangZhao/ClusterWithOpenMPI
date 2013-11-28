#include "master.h"
#include "stringHelper.h"
#include <mpi.h>

master::master(int r, int kvalue, string t) : rank(r), k(kvalue), clusters(NULL), type(t),
                                   isConvergence(false){
}

bool master::init(string type, string inFile){
    clusters = readData(type, inFile);
    if(clusters == NULL){
        cerr << "points are null in init" << endl;
        return false;
    }

    return true;
}

bool master::BcastPtsNum() {
    int count = 1;
    int rc;
    rc = MPI_Bcast(&fileSize, count, MPI_LONG, rank, MPI_COMM_WORLD);
    if(rc != MPI_SUCCESS){
        cout << "BcastPtsNum failed" << endl;
        return false;
    }
    return true;
}

cluster** master::readPoints(string inputFile){
    // get file size
    fileSize = getFileSize(inputFile.c_str());
    ifstream ifs(inputFile.c_str());
    string tmp;
    if(!ifs.good()){
        cout << "invalid inputfile:" << inputFile << endl;
        return NULL;
    }

    vector<pointBase*>* centers = new vector<pointBase*>();

    vector<string> strVec;
    double x;
    double y;
    long interval = fileSize/k;
    int cnt = 0;
    // to estimate line numbers
    int strlen = 0;
    while(getline(ifs, tmp) && cnt < k){
        int tmpLen = (int)tmp.size();
        if(tmpLen > strlen)
            strlen = tmpLen;
        strVec = algorithm::split(tmp, " ");
        x = atof(strVec[0].c_str());
        y = atof(strVec[1].c_str());
        //cout << x << "  " << y << endl;

        twoDPoints* onePoint = new twoDPoints(x, y);
        (*centers).push_back(onePoint);

        cnt++;
        ifs.seekg(cnt * interval - 1, ifs.beg);
        getline(ifs, tmp);
    }

    if(cnt < k){
        cout << "have not found enough centers" << endl;
        return NULL;
    }

    cluster **c = new cluster*[k];
    for(int i = 0; i < k; i++){
        c[i] = new cluster((*centers)[i], (*centers)[i]->getZero(), (int)(fileSize/(k*strlen)));
        //c[i]->print();
    }
    return c;

}

cluster** master::readData(string type, string inputFile){
    if(type.compare("point") == 0)
        return readPoints(inputFile);
    else
        return NULL;
}

long master::getFileSize(string fileName){
    struct stat st;
    if (stat(fileName.c_str(), &st) == -1)
        throw std::runtime_error(std::strerror(errno));
    return (long)st.st_size;
}

bool master::BcastClstCtrs(bool endFlag) {
    int terminate;
    if(endFlag)
        terminate = 1;
    else
        terminate = 0;

    int rc;
    // get centers in a string array
    //cout << "string:" << endl;

    string * strArray = getCtrs();
    //    for(int i = 0; i < k; i++){
    //        cout << strArray[i] << endl;
    //    }
    int strLen;
    // merge string array  into a big one dimensional char array
    char * buf = stringHelper::genCharArrays(strArray, k, &strLen);

    //cout << "broadcasting cluster centers" << endl;
    //stringHelper::printLongBuf(buf, strLen, k);

    // buffer information. bufInfo[0] is strlen. bufInfo[1] is strNum
    int bufInfo[2];
    int bufInfoLen = 2;
    bufInfo[0] = strLen;
    bufInfo[1] = k;

    rc = MPI_Bcast(&terminate, 1, MPI_INT, rank, MPI_COMM_WORLD);
    rc = MPI_Bcast(&bufInfo, bufInfoLen, MPI_INT, rank, MPI_COMM_WORLD);
    rc = MPI_Bcast(buf, bufInfo[0] * bufInfo[1], MPI_CHAR, rank, MPI_COMM_WORLD);

    delete[] strArray;
    delete buf;
//    rc = MPI_Bcast(&fileSize, count, MPI_LONG, rank, MPI_COMM_WORLD);
//    if(rc != MPI_SUCCESS){
//        cout << "BcastPtsNum failed" << endl;
//        return false;
//    }
    return true;
}

string* master::getCtrs(){
    string* strArray = new string[k];
    for(int i = 0; i < k; i++){
        strArray[i] = clusters[i]->getCenter();
    }

    return strArray;
}

bool master::collectSlvInfo(){
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    slavePtsNum = new int[numprocs];
    int rc = 0;
    MPI_Status stat;
    // rank 0 is master
    for(int i = 1; i < numprocs; i++){
        rc = MPI_Recv(&slavePtsNum[i], 1, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
        if(rc != MPI_SUCCESS){
            cout << "collecting points in slaves error" << "i=" <<i << endl;
            return false;
        }
    }

//    for(int i = 1; i < numprocs; i++){
//        cout << "rank " << i << " has " << slavePtsNum[i] << " points" << endl;
//    }
    return true;
}

bool master::updateCtrs(){
    int rc = 0;
    int bufInfo[2];


    cluster** newClust = createEmptyClusters();
    int* ptsNumArr = new int[k];
    memset(ptsNumArr, 0, (size_t)k * sizeof(int));
    int *tmpNumArr = new int[k];

    MPI_Status stat;
    for(int i = 1; i < numprocs; i++){
        rc = MPI_Recv(&bufInfo, 2, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);
        int strLen = bufInfo[0];
        int strNum = bufInfo[1];
        char *buf = new char[strLen * strNum];
        rc = MPI_Recv(buf, strLen*strNum, MPI_CHAR, i, 1, MPI_COMM_WORLD, &stat);
        //receive points numbers for cluster in a slave
        rc = MPI_Recv(tmpNumArr, k, MPI_INT, i, 1, MPI_COMM_WORLD, &stat);

        cout <<"tmp array:" << endl;
        for(int i = 0; i < k; i++){
            ptsNumArr[i] += tmpNumArr[i];
            cout << tmpNumArr[i] << " ";
        }
        cout << endl;

        string *strArray = convBufToStrs(buf, strLen, strNum);
        if(strArray == NULL){
        cout << "strArray is NULL" << endl;
        cout.flush();
        return false;
        }

        for(int i = 0; i < k; i++){
            pointBase* pt = NULL;
            if(type.compare("point") == 0)
                pt = new twoDPoints(strArray[i]);

            newClust[i]->addPoint(pt);
        }

        if(rc != MPI_SUCCESS){
            cout << "collecting points in slaves error" << "i=" <<i << endl;
            delete strArray;
            delete buf;
            return false;
        }

        delete[] strArray;
        delete[] buf;

    }

    cout << "update centers:" << endl;
    for(int i = 0; i < k ; i++){
        newClust[i]->updateCenter(ptsNumArr[i]);
        cout << "cluster"<< i <<endl;
        newClust[i]->print();
    }

    if(checkConvgce(newClust))
        isConvergence = true;
    else{
        cleanUpclasts();
        clusters = newClust;
    }

    delete[] ptsNumArr;
    delete[] tmpNumArr;

    return true;
}

string* master::convBufToStrs(const char *buf, int strLen, int strNum) {
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


// create empty clusters
cluster** master::createEmptyClusters(){
    cluster** newClust = new cluster*[k];
    pointBase* pt;
    pointBase* zero;



    for(int i = 0; i < k; i++){
        if(type.compare("point") == 0){
            pt = new twoDPoints(0, 0);
            zero = new twoDPoints(0,0);
        }
        newClust[i] = new cluster(pt, zero, 10);
    }

    return newClust;
}

bool master::checkConvgce(cluster **newClust) {
    int sum=  0;
    for(int i = 0; i < k; i++){
        sum += clusters[i]->dis(newClust[i]->ctrPtr());
    }

    if(sum < 0.0001)
        return true;
    else
        return false;
}


void master::cleanUpclasts() {
    if (clusters == NULL)
        return;

    // delete all the resource allocated
    for(int i = 0; i < k; i++){
        delete clusters[i];
    }
    delete clusters;
}