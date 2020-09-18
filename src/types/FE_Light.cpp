#include "FE_Light.h"

FE_Light::FE_Light()
{
     createMutex();
    value = 0; fov = 45; range = 4;
}

FE_Light::~FE_Light()
{
    //dtor
}

bool FE_Light::init(){
    return true;
}

bool FE_Light::destroy(){
    return true;
}

FE_Light* FE_Light::setData(void* a_value){
    lockMutex();
    data = a_value;
    unlockMutex();
    return this;
}

FE_Light* FE_Light::setType(string a_value){
    lockMutex();
    light_type = a_value;
    unlockMutex();
    return this;
}

string FE_Light::getType(){
    lockMutex();
    string output = light_type;
    unlockMutex();
    return output;
}

FE_Light* FE_Light::setValue(float a_value){
    lockMutex();
    value = a_value;
    unlockMutex();
    return this;
}

float FE_Light::getValue(){
    lockMutex();
    float output = value;
    unlockMutex();
    return output;
}

FE_Light* FE_Light::setFOV(float a_value){
    lockMutex();
    fov = a_value;
    unlockMutex();
    return this;
}

float FE_Light::getFOV(){
    lockMutex();
    float output = fov;
    unlockMutex();
    return output;
}

FE_Light* FE_Light::setRange(float a_value){
    lockMutex();
    range = a_value;
    unlockMutex();
    return this;
}

float FE_Light::getRange(){
    lockMutex();
    float output = range;
    unlockMutex();
    return output;
}

FE_Light* FE_Light::setColor(FE_Vec4 a_value){
    lockMutex();
    color = a_value;
    unlockMutex();
    return this;
}

FE_Vec4 FE_Light::getColor(){
    lockMutex();
    FE_Vec4 output = color;
    unlockMutex();
    return output;
}

// boilerplate virtual functions needed to ensure thread-safety
FE_Light* FE_Light::setPos(FE_Vec4 posa){ this->internalSetPos(posa); return this;}
FE_Light* FE_Light::setRot(FE_Vec4 posa){ this->internalSetRot(posa); return this;}
FE_Light* FE_Light::setRotEuler(FE_Vec4 axis_angle){ this->internalSetRot(FE_Quat(axis_angle)); return this;}
FE_Light* FE_Light::setScale(FE_Vec4 posa){ this->internalSetScale(posa); return this;}

FE_Light* FE_Light::setX(float posa){ this->internalSetX(posa); return this;}
FE_Light* FE_Light::setY(float posa){ this->internalSetY(posa); return this;}
FE_Light* FE_Light::setZ(float posa){ this->internalSetZ(posa); return this;}

FE_Light* FE_Light::setRotX(float posa){ this->internalSetRotX(posa); return this;}
FE_Light* FE_Light::setRotY(float posa){ this->internalSetRotY(posa); return this;}
FE_Light* FE_Light::setRotZ(float posa){ this->internalSetRotZ(posa); return this;}

FE_Light* FE_Light::setScaleX(float posa){ this->internalSetScaleX(posa); return this;}
FE_Light* FE_Light::setScaleY(float posa){ this->internalSetScaleY(posa); return this;}
FE_Light* FE_Light::setScaleZ(float posa){ this->internalSetScaleZ(posa); return this;}
// *********
