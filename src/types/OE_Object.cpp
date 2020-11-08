#include <types/OE_Object.h>
#include <types/OE_World.h>

using namespace std;

std::vector<float> OE_ObjectData::to_arr() const{
    return {pos_x, pos_y, pos_z, rot_w, rot_x, rot_y, rot_z, sca_x, sca_y, sca_z};
}


size_t              OE_Object::current_id = 0;

OE_Object::OE_Object(){
    
    OE_Object::current_id++;
    
    this->parent                    = 0;
    this->parent_type               = 0;
    this->visible                   = true;
    
    this->id                        = OE_Object::current_id;
}


OE_Object::OE_Object(const string &name){
    
    OE_Object::current_id++;
    
    this->parent                    = 0;
    this->parent_type               = 0;
    this->visible                   = true;
    
    this->id                        = OE_Object::current_id;
}

OE_Object::~OE_Object(){

}  

string OE_Object::to_str() const{
    return "NOTHING";
}

string OE_Object::getType() const{
    return "OBJECT";
}

void OE_Object::SetRot(OE_Quat quat){
    this->current_state.rot_x = quat.x;
    this->current_state.rot_y = quat.y;
    this->current_state.rot_z = quat.z;
    this->current_state.rot_w = quat.w;
}

OE_Quat OE_Object::GetRot(){
    return OE_Quat(current_state.rot_w, current_state.rot_x, current_state.rot_y, current_state.rot_z);
}

OE_Mat4x4 OE_Object::GetModelMatrix(){
    
    OE_Mat4x4 model_mat(1.0f);
    OE_Vec4 nor_quat = OE_Normalize(OE_Vec4(this->current_state.rot_w, this->current_state.rot_x, this->current_state.rot_y, this->current_state.rot_z));
    
    OE_Quat rot_quat = OE_Quat(nor_quat[0], nor_quat[1], nor_quat[2], nor_quat[3]);
    OE_Vec3 translation_vec = OE_Vec3(this->current_state.pos_x, this->current_state.pos_y, this->current_state.pos_z);
    //model_mat = OE_Quat2Mat4x4(rot_quat) * OE_Translate(model_mat, translation_vec);
    model_mat = OE_Translate(model_mat, translation_vec) * OE_Quat2Mat4x4(rot_quat);
    return model_mat;
}

OE_Mat4x4 OE_Object::GetViewMatrix(){
    
    OE_Mat4x4 view_mat(1.0f);
    OE_Vec4 nor_quat = OE_Normalize(OE_Vec4(this->current_state.rot_w, -this->current_state.rot_x, -this->current_state.rot_y, -this->current_state.rot_z));
    
    OE_Quat rot_quat = OE_Quat(nor_quat[0], nor_quat[1], nor_quat[2], nor_quat[3]);
    OE_Vec3 translation_vec = OE_Vec3(-this->current_state.pos_x, -this->current_state.pos_y, -this->current_state.pos_z);
    view_mat = OE_Quat2Mat4x4(rot_quat) * OE_Translate(view_mat, translation_vec);
    return view_mat;
}

