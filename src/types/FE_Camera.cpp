#include "FE_Camera.h"

FE_Camera::FE_Camera()
{
     createMutex();
}

FE_Camera::~FE_Camera()
{
    //dtor
}
FE_Camera* FE_Camera::setData(void* info)
{
    data = info;
    return this;
}

bool FE_Camera::init()
{
    return true;
}

bool FE_Camera::destroy()
{
    return true;
}

float FE_Camera::getAspectRatio(){
    lockMutex();
    float output = aspect_ratio;
    unlockMutex();
    return output;
}

FE_Camera* FE_Camera::setAspectRatio(float a_value){
    lockMutex();
    aspect_ratio = a_value;
    unlockMutex();
    return this;
}

float FE_Camera::getFOV(){
    lockMutex();
    float output = field_of_view;
    unlockMutex();
    return output;
}

FE_Camera* FE_Camera::setFOV(float a_value){
    lockMutex();
    field_of_view = a_value;
    unlockMutex();
    return this;
}

int FE_Camera::getNear(){
    lockMutex();
    int output = near;
    unlockMutex();
    return output;
}

int FE_Camera::getFar(){
    lockMutex();
    int output = far;
    unlockMutex();
    return output;
}

FE_Camera* FE_Camera::setRange(int neara, int fara){
    lockMutex();
    near = neara; far = fara;
    unlockMutex();
    return this;
}

FE_Mat4 FE_Camera::genProjectionMatrix(){
    return FE_Math::perspective(field_of_view, aspect_ratio, (float)near, (float)far);
}

// boilerplate virtual functions needed to ensure thread-safety
FE_Camera* FE_Camera::setPos(FE_Vec4 posa){ this->internalSetPos(posa); return this;}
FE_Camera* FE_Camera::setRot(FE_Vec4 posa){ this->internalSetRot(posa); return this;}
FE_Camera* FE_Camera::setRotEuler(FE_Vec4 axis_angle){ this->internalSetRot(FE_Quat(axis_angle)); return this;}
FE_Camera* FE_Camera::setScale(FE_Vec4 posa){ this->internalSetScale(posa); return this;}

FE_Camera* FE_Camera::setX(float posa){ this->internalSetX(posa); return this;}
FE_Camera* FE_Camera::setY(float posa){ this->internalSetY(posa); return this;}
FE_Camera* FE_Camera::setZ(float posa){ this->internalSetZ(posa); return this;}

FE_Camera* FE_Camera::setRotX(float posa){ this->internalSetRotX(posa); return this;}
FE_Camera* FE_Camera::setRotY(float posa){ this->internalSetRotY(posa); return this;}
FE_Camera* FE_Camera::setRotZ(float posa){ this->internalSetRotZ(posa); return this;}

FE_Camera* FE_Camera::setScaleX(float posa){ this->internalSetScaleX(posa); return this;}
FE_Camera* FE_Camera::setScaleY(float posa){ this->internalSetScaleY(posa); return this;}
FE_Camera* FE_Camera::setScaleZ(float posa){ this->internalSetScaleZ(posa); return this;}
// *********
