#ifndef MASTER_H
#define MASTER_H

#include <iostream>
#include <vector>
#include <fstream>
#include "pointBase.h"
#include "algorithm.h"
#include "twoDPoints.h"
#include "cluster.h"
#include <sys/stat.h>



using namespace std;

class master{
private:
    int rank;
    int numprocs;
    long fileSize;
    cluster** clusters;
    int k;
    string type;
    // starts from 1
    // 0 is not used
    int *slavePtsNum;



    cluster** readPoints(string inputFile);
    cluster** readData(string type, string inputFile);
    long getFileSize(string fileName);
    // get cluster centers in string form
    string* getCtrs();
    string* convBufToStrs(const char *buf, int strLen, int strNum);
    cluster** createEmptyClusters();
    bool checkConvgce(cluster** newClust);
    void cleanUpclasts();

public:

    bool isConvergence;

    master(int r, int kvalue, string t);
    bool init(string type, string inFile);
    bool BcastPtsNum();
    // get information about slaves(how many points they are running)
    bool collectSlvInfo();
    bool BcastClstCtrs(bool endFlag);
    bool updateCtrs();
};


#endif