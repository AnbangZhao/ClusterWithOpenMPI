#ifndef POINTBASEVEC_H
#define POINTBASEVEC_H

#include "pointBase.h"

class pointBaseVec{
private:
    pointBase** array;
    int length;
    int capacity;

public:
    pointBaseVec(int c);

    void push_back(pointBase*);

    void resize(int c);

    pointBase* get(int i);

    int getLength();

    void clean();

    ~pointBaseVec();
};

#endif