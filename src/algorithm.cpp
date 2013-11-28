#include "algorithm.h"

vector<string> algorithm::split(const string& src, string delimit){
    vector<string> r;
    int f = 0;
    int p = -1;

    while ((p = (int)src.find(delimit, p + 1)) != (int)string::npos)
    {
        if (p > f)
            r.push_back(src.substr(f, p - f));
        f = p + (int)delimit.size();
    }
    p = (int)src.size();
        if (p > f)
            r.push_back(src.substr(f, p - f));
    return r;
}