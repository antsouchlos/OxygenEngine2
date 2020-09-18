#include "FE_Texture.h"

FE_Texture::FE_Texture()
{
     createMutex();
}

FE_Texture::~FE_Texture()
{

}

FE_Texture* FE_Texture::setPath(string a_name){
    lockMutex();
    path = a_name;
    unlockMutex();
    return this;
}

string FE_Texture::getPath(){
    lockMutex();
    auto output = path;
    unlockMutex();
    return output;
}


FE_Texture* FE_Texture::setType(string a_name){
    lockMutex();
    type = a_name;
    unlockMutex();
    return this;
}

string FE_Texture::getType(){
    lockMutex();
    auto output = type;
    unlockMutex();
    return output;
}


FE_Texture* FE_Texture::setStencil(string a_name){
    lockMutex();
    this->stencil_name = a_name;
    unlockMutex();
    return this;
}

string FE_Texture::getStencil(){
    lockMutex();
    auto output = stencil_name;
    unlockMutex();
    return output;
}


FE_Texture* FE_Texture::setUVMap(string a_name){
    lockMutex();
    uv_map = a_name;
    unlockMutex();
    return this;
}

string FE_Texture::getUVMap(){
    lockMutex();
    auto output = uv_map;
    unlockMutex();
    return output;
}


FE_Texture* FE_Texture::setStencilIndex(int a_name){
    lockMutex();
    stencil_num = a_name;
    unlockMutex();
    return this;
}

int FE_Texture::getStencilIndex(){
    lockMutex();
    auto output = stencil_num;
    unlockMutex();
    return output;
}

