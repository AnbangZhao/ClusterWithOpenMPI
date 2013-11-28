#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include "pointBase.h"
#include "cluster.h"

class kmeans{
private:
    int k;
    const vector<pointBase*>* points;
    int size;
    cluster **clusterPtr;

    bool inVec(int num, vector<int> vec);
    // clean up all the points and sum.
    // ready for next round
    void cleanClusters();
    // add this point to the nearest cluster
    void addToNearCluster(pointBase* point);
    // update cluster center and check whether clusters have reached convergence
    // if some clusters(one or more) have been updated(center has changed), return false
    // not return true
    bool update();

public:
    kmeans(const vector<pointBase*>* p, int kNum);

    void randomSelectCenter();

    // if clustering algorithm reaches the maximum and still not finished
    // return false
    // if it finishes before maxrounds, return true
    bool doCluster(int maxrounds);

    void print();
};




#endif