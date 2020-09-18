#include "FE_VertexGroup.h"

FE_VertexGroup::FE_VertexGroup()
{
     createMutex();
}

FE_VertexGroup::~FE_VertexGroup()
{
    //dtor
}


string FE_VertexGroup::getType(){
    lockMutex();
    auto output = material;
    unlockMutex();
    return output;
}

FE_VertexGroup* FE_VertexGroup::setType(string val){
    lockMutex();
    material = val;
    unlockMutex();
    return this;
}

string FE_VertexGroup::getTypeName(){
    lockMutex();
    auto output = bone_name;
    unlockMutex();
    return output;
}

FE_VertexGroup* FE_VertexGroup::setTypeName(string val){
    lockMutex();
    bone_name = val;
    unlockMutex();
    return this;
}

FE_VertexGroup* FE_VertexGroup::assignPolygon(size_t val){
    lockMutex();
    for(size_t i = 0; i< polygons.size(); i++)
    if(polygons[i] == val){
        unlockMutex();
        return this;
    }
    polygons.push_back(val);
    unlockMutex();
    return this;
}

FE_VertexGroup* FE_VertexGroup::removePolygon(size_t val){
    lockMutex();
    for(size_t i = 0; i< polygons.size(); i++)
    if(polygons[i] == val){
        polygons.erase(polygons.begin()+i);
        break;
    }
    unlockMutex();
    return this;
}

size_t FE_VertexGroup::getNumPolygons(){
    lockMutex();
    size_t output = polygons.size();
    unlockMutex();
    return output;
}
