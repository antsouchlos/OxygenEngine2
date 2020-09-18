#include "FE_Polygon.h"

FE_Polygon::FE_Polygon(int indexa, int v1, int v2, int v3)
{
     createMutex();
    n = indexa;
    vertexNums.push_back(v1);
    vertexNums.push_back(v2);
    vertexNums.push_back(v3);
}

FE_Polygon::~FE_Polygon()
{
    vertexNums.clear();
}

FE_Polygon* FE_Polygon::setIndex(int a_name){
    lockMutex();
    n = a_name;
    unlockMutex();
    return this;
}

int FE_Polygon::getIndex(){
    lockMutex();
    auto output = n;
    unlockMutex();
    return output;
}

FE_Polygon* FE_Polygon::setNormal(FE_Vec4 a_name){
    lockMutex();
    normal = a_name;
    unlockMutex();
    return this;
}

FE_Vec4 FE_Polygon::getNormal(){
    lockMutex();
    auto output = normal;
    unlockMutex();
    return output;
}

FE_Polygon* FE_Polygon::setVertices(int v1, int v2, int v3){
    lockMutex();
    vertexNums[0] = v1; vertexNums[1] = v2; vertexNums[2] = v3;
    unlockMutex();
    return this;
}

vector<size_t> FE_Polygon::getVertices(){
    lockMutex();
    auto output = vertexNums;
    unlockMutex();
    return output;
}

