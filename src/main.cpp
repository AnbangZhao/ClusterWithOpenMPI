//
//  main.cpp
//  OpenMPIClustering
//
//  Created by Anbang Zhao on 11/25/13.
//  Copyright (c) 2013 Anbang Zhao. All rights reserved.
//

#include <iostream>
#include <mpi.h>
#include <fstream>
#include <assert.h>
#include "master.h"
#include "slave.h"

using namespace std;

int main(int argc, char **argv){

    assert(argc == 4 && "Example: ./a.out type(point/DNA) k inputFile");

    string type = argv[1];
    int k = atoi(argv[2]);
    string inFile = argv[3];

    ifstream ifs(inFile.c_str());

    // processor name
    char processorName[MPI_MAX_PROCESSOR_NAME];
    int numprocs, rank, namelen;
    // init openmpi
    MPI_Init(&argc, &argv);
    // get total processors in the cluster
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    // get local machine rank(ID)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // get processor name
    MPI_Get_processor_name(processorName, &namelen);

    // master node
    if(rank == 0){
//        vector<pointBase*> *points = readData(type, inFile);
//        int pointNum = (int)points->size();
//        int count = 1;
//        // tell every slave the total point number
//        MPI_Bcast(&pointNum, count, MPI_INT, rank, MPI_COMM_WORLD);
        master masterNode(rank, k, type);
        masterNode.init(type, inFile);
        // broadcast total points info to all the slaves
        masterNode.BcastPtsNum();
        // collect slave info
        //cout << "broadcast finished" << endl;
        //cout.flush();
        masterNode.collectSlvInfo();
        //cout << "collect finished" << endl;
        //cout.flush();

        while(!masterNode.isConvergence){
            // broadcast new center info to all the slaves
            masterNode.BcastClstCtrs(false);
            cout << "Bcast finished" << endl;
            cout.flush();
            // update center infos
            masterNode.updateCtrs();
            cout << "update finished" << endl;
            cout.flush();
        }
        cout << "out of loop" << endl;
        masterNode.BcastClstCtrs(true);

    }
    // slave nodes
    else{
        // n -1 slaves, 1 master
        slave slaveNode(processorName, numprocs - 1, rank, type);
        // receive total pointsNum
        slaveNode.recTotalPtsNum();
        // read points this slave needs to deal with
        slaveNode.readPts(type, inFile);
        //cout << "readPts finished" << endl;
        //cout.flush();
        // send info to the master about how many point it has to run
        slaveNode.sendInfo();
        //cout << "sendInfo finished" << endl;
        //cout.flush();

        while(true){

            // receive new cluster centers and create clusters
            bool flag = slaveNode.recClstCtrs();
            if(!flag){
                break;
            }
            //cout << "recClastCtrs finished" << endl;
            //cout.flush();
            // cluster local points to clusters
            slaveNode.doCluster();
            //cout << "doCluster finished" << endl;
            //cout.flush();
            // report new cluster centers to master
            slaveNode.reportInfo();
            //cout << "reportInfo finished" << endl;
            //cout.flush();
        }


//        int pointNum;
//        int count = 1;
//        int masterID = 0;
//        // receive the broadcast from master
//        MPI_Bcast(&pointNum, count, MPI_INT, masterID, MPI_COMM_WORLD);
//        cout << "rank: " << rank << " pointNum: " << pointNum << endl;
    }

    MPI_Finalize();
    return 1;
}