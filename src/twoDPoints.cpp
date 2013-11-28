#ifndef __TWOPOINTS_H
#define __TWOPOINTS_H

#include <vector>
#include "twoDPoints.h"
#include "algorithm.h"

twoDPoints::twoDPoints(double a, double b){
    x = a;
    y = b;
}

twoDPoints::twoDPoints(string s) {
    vector<string> strVec;
    strVec = algorithm::split(s, " ");
    x = atof(strVec[0].c_str());
    y = atof(strVec[1].c_str());
}

double twoDPoints::dist(pointBase* p){
    twoDPoints* tmp = (twoDPoints*)p;
    return euculidianDist(x, y, tmp->x, tmp->y);
}


double twoDPoints::euculidianDist(double x1, double y1, double x2, double y2){
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

pointBase* twoDPoints::fork(){
    pointBase *newPoint = new twoDPoints(this->x, this->y);
    return newPoint;
}

pointBase* twoDPoints::getZero(){
    pointBase* ret = new twoDPoints(0, 0);
    return ret;
}

void twoDPoints::add(pointBase* p){
    twoDPoints* tmp = (twoDPoints*)p;
    x += tmp->x;
    y += tmp->y;
}

void twoDPoints::divide(int n){
    double pos_x = x/n;
    double pos_y = y/n;
    this->x = pos_x;
    this->y = pos_y;
}

void twoDPoints::clone(pointBase* point){
    this->x = ((twoDPoints*)point)->x;
    this->y = ((twoDPoints*)point)->y;
}

string twoDPoints::toString() {
    string xStr = to_string(x);
    string yStr = to_string(y);
    return xStr+" "+yStr;
}

void twoDPoints::clean() {
    x = 0;
    y = 0;
}

twoDPoints::~twoDPoints(){
}

#endif