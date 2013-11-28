#ifndef STRINGHELPER_H
#define STRINGHELPER_H

#include <iostream>
using namespace std;

class stringHelper{
public:
    static int getMaxStrLen(string *strPtr, int num);
    static char * genCharArrays(string* strPtr, int num, int *strLen);
    static void printLongBuf(char *buf, int len, int num);
    static int upRoundDivide(double dividor, double divident);
};

// get the length of the longest string in the string array

#endif