#include "cluster.h"

cluster::cluster(pointBase* c, pointBase* zero, int num):pointsVec(num){
    center = c;
    sum = zero;
}

cluster::~cluster(){
    delete center;
    delete sum;
}

void cluster::addPoint(pointBase *point) {
    sum->add(point);
    pointsVec.push_back(point);
}

void cluster::cleanPts(){
    sum->clean();
    pointsVec.clean();
}

double cluster::dis(pointBase* point){
    return center->dist(point);
}


// if center has changed, return true
// if it has not changed, return false
bool cluster::updateCenter() {

    if(pointsVec.getLength() != 0)
        sum->divide(pointsVec.getLength());
    //cout << "sum" << sum->toString() << endl;
    //cout << "center" << center->toString() << endl;
    //cout << "dist" << center->dist(sum) << endl;

    // double calculation may result in some minor errors making distance
    // not exactly zero

    center->clone(sum);
    return true;
}

bool cluster::updateCenter(int div){
    if(div != 0)
        sum->divide(div);
    if(div == 0){
        cout << "div is zero" << endl;
        return false;
    }

    center->clone(sum);
    return true;
}

void cluster::print(){
    for(int i = 0; i < pointsVec.getLength(); i++){
        cout << i << endl;
        cout << (pointsVec.get(i))->toString() << endl;
    }
    cout << "center:" << center->toString() << endl;
    cout << "sum:" << sum->toString() << endl;
}

string cluster::getCenter(){
    return center->toString();
}

string cluster::getSum(){
    return sum->toString();
}

int cluster::getPtsNum(){
    return pointsVec.getLength();
}

pointBase* cluster::ctrPtr() {
    return center;
}

cluster::cluster() : pointsVec(0) {
}


