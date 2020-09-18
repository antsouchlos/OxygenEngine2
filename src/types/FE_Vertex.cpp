#include "FE_Vertex.h"

FE_Vertex::FE_Vertex(unsigned int inputa)
{
    n = inputa;
    createMutex();
}

FE_Vertex::FE_Vertex()
{
     createMutex();
}
FE_Vertex::~FE_Vertex()
{
    //dtor
}

bool FE_Vertex::init(){
    return true;
}
bool FE_Vertex::destroy(){
    for(auto x: uvmaps){
        delete x;
    }
    uvmaps.clear();
    return true;
}

bool FE_Vertex::equivalents(FE_Vertex* other){
    // compare the pos/rot
    bool output = this->n == other->n;
    // check uvmaps

    if(this->uvmaps.size() == other->uvmaps.size())
    for(size_t i = 0 ; i < uvmaps.size(); i++){
        output = output && uvmaps[i]->name == other->uvmaps[i]->name && fabs(uvmaps[i]->u-other->uvmaps[i]->u)< FE_EPSILON \
                                                                    && fabs(uvmaps[i]->v == other->uvmaps[i]->v) < FE_EPSILON;
    }
    else output = false;
    return output;
}

FE_Vertex* FE_Vertex::setIndex(size_t a_name){
    lockMutex();
    n = a_name;
    unlockMutex();
    return this;
}

size_t FE_Vertex::getIndex(){
    lockMutex();
    auto output = n;
    unlockMutex();
    return output;
}

FE_Vec4 FE_Vertex::getPos(){
    lockMutex();
    auto output = FE_Vec4(x, y, z, 0.0f);
    unlockMutex();
    return output;
}

FE_Vertex* FE_Vertex::setPos(FE_Vec4 a_name){
    lockMutex();
    x = a_name.x;
    y = a_name.y;
    z = a_name.z;
    unlockMutex();
    return this;
}

FE_Vertex* FE_Vertex::setX(float a_name){
    lockMutex();
    x = a_name;
    unlockMutex();
    return this;
}

FE_Vertex* FE_Vertex::setY(float a_name){
    lockMutex();
    y = a_name;
    unlockMutex();
    return this;
}

FE_Vertex* FE_Vertex::setZ(float a_name){
    lockMutex();
    z = a_name;
    unlockMutex();
    return this;
}


FE_Vec4 FE_Vertex::getNormals(){
    lockMutex();
    auto output = FE_Vec4(normalx, normaly, normalz, 0.0f);
    unlockMutex();
    return output;
}

FE_Vertex* FE_Vertex::setNormals(FE_Vec4 a_name){
    lockMutex();
    normalx = a_name.x;
    normaly = a_name.y;
    normalz = a_name.z;
    unlockMutex();
    return this;
}

FE_Vertex* FE_Vertex::setNormalX(float a_name){
    lockMutex();
    normalx = a_name;
    unlockMutex();
    return this;
}

FE_Vertex* FE_Vertex::setNormalY(float a_name){
    lockMutex();
    normaly = a_name;
    unlockMutex();
    return this;
}

FE_Vertex* FE_Vertex::setNormalZ(float a_name){
    lockMutex();
    normalz = a_name;
    unlockMutex();
    return this;
}


FE_UVMap* FE_Vertex::createUVMap(string a_name){
    lockMutex();

    FE_UVMap* output = new FE_UVMap();
    output->name = a_name;
    uvmaps.push_back(output);

    unlockMutex();
    return output;
}

FE_UVMap* FE_Vertex::getUVMap(string a_name){
    lockMutex();
    FE_UVMap* output = nullptr;
    for(auto x : uvmaps)
    if(x->name == a_name)
        output = x;

    unlockMutex();
    return output;
}

FE_Vertex* FE_Vertex::renameUVMap(string a_name, string prev_name){
    lockMutex();
    this->getUVMap(a_name)->name = prev_name;
    unlockMutex();
    return this;
}

FE_Vertex* FE_Vertex::removeUVMap(string a_name){
    lockMutex();
    for(unsigned int i=0; i< uvmaps.size();i++)
    if(uvmaps[i]->name == a_name){
        delete uvmaps[i];
        uvmaps.erase(uvmaps.begin()+i);
    }

    unlockMutex();
    return this;
}

vector<string> FE_Vertex::getUVMapNames(){
    vector<string> output;
    for(auto scene : this->uvmaps) output.push_back(scene->name);
    return output;
}

/*****************UV*MAP******************/
FE_UVMap::FE_UVMap()
{
    //dtor
}
FE_UVMap::~FE_UVMap()
{
    //dtor
}

FE_UVMap* FE_UVMap::setU(float a_name){
    lockMutex();
    u = a_name;
    unlockMutex();
    return this;
}

FE_UVMap* FE_UVMap::setV(float a_name){
    lockMutex();
    v = a_name;
    unlockMutex();
    return this;
}

FE_UV FE_UVMap::getUV(){
    lockMutex();
    FE_UV output;
    output.u = u;
    output.v = v;
    unlockMutex();
    return output;
}

