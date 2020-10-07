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


