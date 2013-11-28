#ifndef TWODPOINTS_H
#define TWODPOINTS_H

#include "pointBase.h"
#include <math.h>

class twoDPoints : public pointBase{
public:
    double x;
    double y;
    
    twoDPoints(double a, double b);

    twoDPoints(string str);

    virtual double dist(pointBase* p);

    virtual void add(pointBase*);
    
    virtual void divide(int n);

    virtual pointBase* fork();


    virtual void clone(pointBase*);

    virtual pointBase* getZero();

    virtual string toString();

    virtual void clean();

    virtual ~twoDPoints();

private:
    double euculidianDist(double x1, double y1, double x2, double y2);
};

#endif