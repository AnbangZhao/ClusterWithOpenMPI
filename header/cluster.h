#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include "pointBase.h"
#include "pointBaseVec.h"


class cluster{
private:
    pointBase* center;
    // sum all the points to get the new center
    pointBase* sum;
    pointBaseVec pointsVec;

public:
    // c may be changed in clustering process
    // if c needs to be reserved, use c->fork to create a new one
    cluster(pointBase* c, pointBase* zero, int num);

    cluster();

    void addPoint(pointBase*);

    // calculate the distance of the point to the center of this cluster
    double dis(pointBase* point);

    void cleanPts();

    bool updateCenter();

    bool updateCenter(int div);

    int getPtsNum();

    pointBase* ctrPtr();

    void print();

    ~cluster();

    string getCenter();

    string getSum();

};


#endif