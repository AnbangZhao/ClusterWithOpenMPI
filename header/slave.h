#ifndef SLAVE_H
#define SLAVE_H

#include <vector>
#include <mpi.h>
#include "pointBase.h"
#include "stringHelper.h"
#include "cluster.h"


class slave{
private:
    int rank;
    int masterID;
    long fileSize;
    const char *processorName;
    int slaveNum;
    string type;
    vector<pointBase*> *points;
    cluster** clusters;
    int clusterNum;

    bool createCluster(const char *buf, int strLen, int strNum);
    string *convBufToStrs(const char *buf, int strLen, int strNum);
    void cleanUpclsts();
    void addToNearCluster(pointBase *point);
    string* getCtrs();


public:
    slave(const char *name, int sNum, int r, string type);
    bool recTotalPtsNum();
    bool readPts(string type, string inFile);
    bool recClstCtrs();
    // send internal info to master
    bool sendInfo();
    // report cluster infos to master
    bool reportInfo();

    // do clustering
    void doCluster();

};


#endif