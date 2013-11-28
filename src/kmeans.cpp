#include "kmeans.h"

kmeans::kmeans(const vector<pointBase*>* p, int kNum) {
    points = p;
    k = kNum;
    size = (int)p->size();
    clusterPtr = new cluster*[k];

}

void kmeans::randomSelectCenter() {
    if(k > points->size()){
        cout << "k is bigger than points size" << endl;
        return;
    }

    vector<int> selectedPts;
    for(int i = 0; i < k; i++){
        int tmp = 0;
        while(1){
            // randomly select points
            tmp = rand()%size;
            // not selected yet
            if(!inVec(tmp, selectedPts))
                break;
        }
        selectedPts.push_back(tmp);
        // create a init cluster with a selected center
        // cluster's center may change its internal data, so we need to fork
        // another memory block for it to play
        clusterPtr[i] = new cluster((*points)[tmp]->fork(), (*points)[tmp]->getZero(), size/k);
    }
}

void kmeans::cleanClusters() {
    for(int i = 0; i < k; i++){
        clusterPtr[i]->cleanPts();
    }
}

bool kmeans::doCluster(int maxrounds){
    int curRound = 0;
    randomSelectCenter();
    while(++curRound <= maxrounds){
        cleanClusters();
        for(int i = 0; i < points->size(); i++){
            addToNearCluster((*points)[i]);
        }

        // if no cluster updates any more, it means convergence.
        if(!update())
            return true;
    }
    return false;
}

void kmeans::addToNearCluster(pointBase *point) {
    double minDis = 999999;
    int minPos = -1;

    for(int i = 0; i < k; i++){
        double tmpDis = clusterPtr[i]->dis(point);
        if(tmpDis < minDis){
            minPos = i;
            minDis = tmpDis;
        }
    }

    if(minPos == -1)
        cerr << "min pos is -1(no dis < 999999 or no available clusters)" << endl;
    else{
        clusterPtr[minPos]->addPoint(point);
    }
}


bool kmeans::update(){
    bool flag = false;
    for(int i = 0; i < k; i++){
        // any one cluster has changed, return true
        if(clusterPtr[i]->updateCenter())
            flag = true;
    }

    return flag;
}


bool kmeans::inVec(int num, vector<int> vec){
    for(int i = 0; i < vec.size(); i++){
        if(num == vec[i])
            return true;
    }

    return false;
}

void kmeans::print(){

    for(int i = 0; i < k; i++){
        cout << "cluster " << i << endl;
        if(clusterPtr[i] == NULL)
            cout << "this cluster pointer is NULL" << endl;
        else
            clusterPtr[i]->print();
    }
}