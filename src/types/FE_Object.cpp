#include "FE_Object.h"

FE_Object::FE_Object()
{

}

FE_Object::~FE_Object()
{

}


FE_Vec4 FE_Object::getPos(){

    lockMutex();
    FE_Vec4 output = pos;
    unlockMutex();
    return output;
}

FE_Vec4 FE_Object::getRot(){

    lockMutex();
    FE_Vec4 output = quat_rot.toAxisAngle();
    unlockMutex();
    return output;
}

FE_Vec4 FE_Object::getRotEuler(){

    lockMutex();
    FE_Vec4 output = rot;
    unlockMutex();
    return output;
}

FE_Vec4 FE_Object::getScale(){

    lockMutex();
    FE_Vec4 output = sca;
    unlockMutex();
    return output;
}
// DELTA
FE_Vec4 FE_Object::getDeltaPos(){

    lockMutex();
    FE_Vec4 output = pos-delta_pos;
    unlockMutex();
    return output;
}

FE_Vec4 FE_Object::getDeltaRot(){

    lockMutex();
    FE_Vec4 quat1 = quat_rot.toAxisAngle();
    quat1.w = -quat1.w;
    FE_Vec4 output = (FE_Quat(quat1) * FE_Quat(delta_quat)).toAxisAngle();
    unlockMutex();
    return output;
}

FE_Vec4 FE_Object::getDeltaRotEuler(){

    lockMutex();
    FE_Vec4 output = rot;
    unlockMutex();
    return output;
}

FE_Vec4 FE_Object::getDeltaScale(){

    lockMutex();
    FE_Vec4 output = sca;
    unlockMutex();
    return output;
}
void* FE_Object::getData(){
    return data;
};
/*
FE_Vec4 FE_Object::setPos(){

    lockMutex();
    FE_Vec4 output = pos;
    unlockMutex();
    return output;
}

FE_Vec4 FE_Object::setRot(FE_Vec4){

    lockMutex();
    FE_Vec4 output = rot;
    unlockMutex();
    return output;
}

FE_Vec4 FE_Object::setScale(FE_Vec4 scale){

    lockMutex();
    FE_Vec4 output = sca;
    unlockMutex();
    return output;
}
*/

FE_Mat4 FE_Object::getModelMatrix(){
    lockMutex();
    FE_Mat4 mat;
    mat.setRot(quat_rot);
    mat.setPos(pos);
    unlockMutex();
    return mat;
}
void FE_Object::internalSetPos(FE_Vec4 info){
    lockMutex();
    pos = info;
    unlockMutex();
}
void FE_Object::internalSetRot(FE_Vec4 info){
    lockMutex();
    rot = info;
    rotation_type = true;
    unlockMutex();
};
void FE_Object::internalSetRot(FE_Quat info){
    lockMutex();
    quat_rot = info;
    rotation_type = false;
    unlockMutex();
};
void FE_Object::internalSetScale(FE_Vec4 info){
    lockMutex();
    sca = info;
    unlockMutex();
};

void FE_Object::internalSetX(float info){
    lockMutex();
    pos.x = info;
    unlockMutex();
}

void FE_Object::internalSetY(float info){
    lockMutex();
    pos.y = info;
    unlockMutex();
}

void FE_Object::internalSetZ(float info){
    lockMutex();
    pos.z = info;
    unlockMutex();
}

void FE_Object::internalSetRotX(float info){
    lockMutex();
    rot.x = info;
    unlockMutex();
}

void FE_Object::internalSetRotY(float info){
    lockMutex();
    rot.y = info;
    unlockMutex();
}

void FE_Object::internalSetRotZ(float info){
    lockMutex();
    rot.z = info;
    unlockMutex();
}

void FE_Object::internalSetScaleX(float info){
    lockMutex();
    sca.x = info;
    unlockMutex();
}

void FE_Object::internalSetScaleY(float info){
    lockMutex();
    sca.y = info;
    unlockMutex();
}

void FE_Object::internalSetScaleZ(float info){
    lockMutex();
    sca.z = info;
    unlockMutex();
}

