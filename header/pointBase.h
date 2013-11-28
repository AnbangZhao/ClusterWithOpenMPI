#ifndef __POINTBASE_H
#define __POINTBASE_H


#include <iostream>
using namespace std;

class pointBase{
public:
    virtual double dist(pointBase*)=0;

    virtual void add(pointBase*)=0;

    virtual void divide(int n)=0;

    virtual string toString()=0;

    virtual void clean()=0;

    virtual void clone(pointBase*)=0;

    virtual pointBase* fork()=0;

    virtual pointBase* getZero()=0;

    virtual ~pointBase(){};
};

#endif