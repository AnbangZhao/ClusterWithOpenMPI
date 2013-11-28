#include "pointBaseVec.h"

pointBaseVec::pointBaseVec(int c) {
    capacity = c;
    length = 0;
    array = new pointBase*[capacity];
}

pointBaseVec::~pointBaseVec() {
    delete array;
}

void pointBaseVec::push_back(pointBase *point) {
    if(length >= capacity)
        resize(2 * capacity);

    array[length++] = point;
}

void pointBaseVec::resize(int c){
    capacity = c;
    pointBase** newArray = new pointBase*[capacity];
    for(int i = 0; i < length; i++)
        newArray[i] = array[i];

    delete array;
    array = newArray;
    capacity = c;
}

pointBase* pointBaseVec::get(int i){
    if(i > length){
        cout<< "i is bigger than length" << endl;
        return NULL;
    }

    return array[i];
}

void pointBaseVec::clean(){
    delete array;
    array = new pointBase*[capacity];
    length = 0;
}

int pointBaseVec::getLength() {
    return length;
}