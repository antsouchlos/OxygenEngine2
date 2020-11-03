#include <types/OE_Object.h>

using namespace std;

std::vector<float> OE_ObjectData::to_arr() const{
    return {pos_x, pos_y, pos_z, rot_w, rot_x, rot_y, rot_z, sca_x, sca_y, sca_z};
}


size_t              OE_Object::current_id = 0;
unordered_map<size_t, string> OE_Object::id2name;
OE_Name2ID          OE_Object::name2id = OE_Name2ID(&OE_Object::id2name);

OE_Object::OE_Object(){
    
    OE_Object::current_id++;
    
    this->parent                    = 0;
    this->parent_type               = 0;
    this->visible                   = true;
    
    this->id                        = OE_Object::current_id;
    OE_Object::id2name[this->id]    = "noname_" + to_string(this->id);
    
}


OE_Object::OE_Object(const string &name){
    
    OE_Object::current_id++;
    
    this->parent                    = 0;
    this->parent_type               = 0;
    this->visible                   = true;
    
    this->id                        = OE_Object::current_id;
    OE_Object::id2name[this->id]       = name;
    
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
    OE_Quat rot_quat = OE_Quat(this->current_state.rot_w, this->current_state.rot_x, this->current_state.rot_y, this->current_state.rot_z);
    OE_Vec3 translation_vec = OE_Vec3(this->current_state.pos_x, this->current_state.pos_y, -this->current_state.pos_z);
    model_mat = OE_Quat2Mat4x4(rot_quat) * OE_Translate(model_mat, translation_vec);
    return model_mat;
}

