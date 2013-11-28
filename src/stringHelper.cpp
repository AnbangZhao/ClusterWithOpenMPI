#include "stringHelper.h"

int stringHelper::getMaxStrLen(string *strPtr, int num){
    if(strPtr == NULL){
        cout << "empty string array" << endl;
        return -1;
    }
    int max = -1;
    for(int i = 0; i < num; i++){
        if(max < (int)strPtr[i].size())
            max = (int)strPtr[i].size();
    }
    return max;
}

inline bool copyStr(char *buf, int bufLen, string str){

    // >= because /0 will occupy one position
    if(str.size() >= bufLen){
        cout << "string is longer than bufLen" << endl;
        return false;
    }

    for(int i = 0; i < (int)str.size(); i++){
        buf[i] = str[i];
    }
    for(int i = (int)str.size(); i < bufLen; i++){
        buf[i] = '\0';
    }
    return true;

}

char * stringHelper::genCharArrays(string* strPtr, int num, int *strLen){
    *strLen = getMaxStrLen(strPtr, num) + 1;
    // allocate maxLen * strNum chars to store the strings
    // using an one-dimentional array to ensure the allocated space is continuous
    char *buf = new char[num * (*strLen)];
    for(int i = 0; i < num; i++){
        copyStr(buf + i*(*strLen), *strLen, strPtr[i]);
    }
    return buf;
}
// specifically print a long one dimensional array
// this array is just like a two dimensional array
// len is a string length
// num is string number
void stringHelper::printLongBuf(char *buf, int len, int num){
    if(buf == NULL){
        cout << "buf is NULL" << endl;
        return;
    }
    for(int i = 0; i < num; i++){
        cout << buf + len * i << endl;
    }
}

int stringHelper::upRoundDivide(double dividor, double divident){
    double res = dividor/divident;
    int round = (int)res;
    if(res > round)
        round++;
    return round;
}
