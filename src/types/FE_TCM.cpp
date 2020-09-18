#include "FE_TCM.h"

FE_TCM::FE_TCM()
{
     createMutex();
}

FE_TCM::~FE_TCM()
{

}
bool FE_TCM::init(){
    return true;
}

bool FE_TCM::destroy(){
    textureNames.clear();
    tex_indices.clear();
    modes.clear();
    return true;
}
//DONE
vector<string> FE_TCM::getTextureNames(){
    lockMutex();
    auto output = textureNames;
    unlockMutex();
    return output;
}
//DONE
FE_TCM* FE_TCM::assignTexture(string a_name, string a2_name, string mode){

    lockMutex();
    bool isIncluded[2] = {false, false};
    unsigned int tex_index[2] = {0, 0};

    // check if a_name is already in the list
    for(unsigned int x = 0; x<textureNames.size(); x++){
        if(textureNames[x]==a_name){
            isIncluded[0] = true;
            tex_index[0] = x;
        }
        if(textureNames[x]==a2_name){
            isIncluded[1] = true;
            tex_index[1] = x;
        }
    }

    if(isIncluded[0] && isIncluded[1]){

        // if both are included only change the appropriate mode
        for(unsigned int i=0; i< modes.size()-1; i+=2){
        if(modes[i] == tex_index[0] && modes[i+1] == tex_index[1])
                tex_indices[i/2] = mode;
        }
    }
    else if(isIncluded[0] && !isIncluded[1]){

        textureNames.push_back(a2_name);
        modes.push_back(tex_index[0]);
        modes.push_back(textureNames.size()-1);
        tex_indices.push_back(mode);
    }
    else if(!(isIncluded[0] && isIncluded[1])){
        textureNames.push_back(a_name);
        textureNames.push_back(a2_name);
        modes.push_back(textureNames.size()-2);
        modes.push_back(textureNames.size()-1);
        tex_indices.push_back(mode);
    }
    else throw 10;
    unlockMutex();
    return this;

}
//DONE
FE_TCM* FE_TCM::removeTexture(string a_name){
    lockMutex();
    unsigned int indexa;
    for(unsigned int x = 0; x<textureNames.size(); x++){
        if(textureNames[x]==a_name) indexa = x;
    }
    textureNames.erase(textureNames.begin()+indexa);
    int i= 0; int a= 0;
    while(i < a){
        if(modes[i] == indexa || modes[i+1] == indexa){
            modes.erase(modes.begin()+i, modes.begin()+i+1);
            tex_indices.erase(tex_indices.begin() + i/2);
            a-=2; i-=2;
        }
        i+=2;
    }
    unlockMutex();
    return this;
}
//DONE
FE_Vec4 FE_TCM::getIntensity(){
    lockMutex();
    auto output = value;
    unlockMutex();
    return output;
}
//DONE
FE_TCM* FE_TCM::setIntensity(FE_Vec4 a_name){
    lockMutex();
    value = a_name;
    unlockMutex();
    return this;
}
//DONE
FE_TCM* FE_TCM::setMode(string a_name){
    lockMutex();
    combine_mode = a_name;
    unlockMutex();
    return this;
}
//DONE
string FE_TCM::getMode(){
    lockMutex();
    auto output = combine_mode;
    unlockMutex();
    return output;
}

