#include <types/OE_Camera.h>
#include <types/OE_World.h>

using namespace std; 

OE_Camera::OE_Camera(){
    
    this->near = 0.1f;
    this->far = 10000.0f;
    this->fov = 0.0f;
    this->aspect_ratio = 16.0f/9.0f;
    
}

OE_Camera::OE_Camera(const string& name) : OE_Object(name){
    this->near = 0.1f;
    this->far = 10000.0f;
    this->fov = 0.0f;
    this->aspect_ratio = 16.0f/9.0f;
}

OE_Camera::~OE_Camera(){
    
}

string OE_Camera::getType() const{
    return "CAMERA";
}

string OE_Camera::to_str() const{
    string output = outputTypeTag("Camera", {{"name", "\"" + OE_World::objectsList.id2name_[this->id] + "\""}, {"visible", convert((int)visible)}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    output.append(outputList("current_state", this->current_state.to_arr()));
    output.append("\n");
    
    output.append(outputVar("parent", "\"" + OE_World::objectsList.id2name_[this->parent] + "\""));
    output.append("\n");
    
    output.append(outputVar("parent_type", convert(this->parent_type)));
    output.append("\n");
    
    output.append(outputVar("fov", convert(this->fov)));
    output.append("\n");
    
    output.append(outputVar("aspect_ratio", convert(this->aspect_ratio)));
    output.append("\n");
    
    output.append(outputVar("near", convert(this->near)));
    output.append("\n");
    
    output.append(outputVar("far", convert(this->far)));
    output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Camera"));
    return output;
}
