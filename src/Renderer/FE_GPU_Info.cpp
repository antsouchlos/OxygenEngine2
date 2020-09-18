#include "FE_GPU_Info.h"

FE_GPU_Info::FE_GPU_Info()
{
    //ctor
}

FE_GPU_Info::~FE_GPU_Info()
{
    //dtor
}

// find GPU objects by name easily
size_t FE_GPU_Info::findVBO(string a_name){
    //cout << vbos.size() << endl;
    for(size_t x =0; x < vbos.size(); x++)
    if(vbos[x].name == a_name){
        return x;
    }
    return vbos.size();
}
size_t FE_GPU_Info::findVAO(string a_name){

    for(size_t x =0; x < vaos.size(); x++)
    if(vaos[x].name == a_name){
        return x;
    }
    return vaos.size();
}
size_t FE_GPU_Info::findProgram(string a_name){

    for(size_t x =0; x < programs.size(); x++)
    if(programs[x].name == a_name){
        return x;
    }
    return programs.size();
}
size_t FE_GPU_Info::findUniform(string a_name){
    for(size_t x =0; x < ubos.size(); x++)
    if(ubos[x].name == a_name){
        return x;
    }
    return ubos.size();
}
