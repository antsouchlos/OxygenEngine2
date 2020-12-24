#include <types/OE_Light.h>
#include <types/OE_World.h>

using namespace std; 

std::vector<float> OE_RGBColor::to_arr() const{
    return {r, g, b};
}

OE_Light::OE_Light(){
    
    this->light_type = 0;
    this->fov = 0.0f;
    this->range = 0.0f;
    this->intensity = 0.0f;
    
}

OE_Light::OE_Light(const string& name) : OE_Object(name){
    this->light_type = 0;
    this->fov = 0.0f;
    this->range = 0.0f;
    this->intensity = 0.0f;
}

OE_Light::~OE_Light(){
    
}

string OE_Light::getType() const{
    return "LIGHT";
}

string OE_Light::to_str() const{
    string output = outputTypeTag("Light", {{"name", "\"" + OE_World::objectsList.id2name[this->id] + "\""}, {"visible", convert((int)visible)}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    output.append(outputList("current_state", this->current_state.to_arr()));
    output.append("\n");
    
    output.append(outputVar("parent", "\"" + OE_World::objectsList.id2name[this->parent] + "\""));
    output.append("\n");
    
    output.append(outputVar("parent_type", convert(this->parent_type)));
    output.append("\n");
    
    vector<string> object_strs;
    for(const auto &x: this->objects){
        object_strs.push_back("\"" + OE_World::objectsList.id2name[x] + "\"");
    }
    
    if(object_strs.size() != 0){
        output.append(outputList("objects", object_strs));
        output.append("\n");
    }
    
    output.append(outputList("color", this->color.to_arr()));
    output.append("\n");
    
    output.append(outputVar("intensity", convert(this->intensity)));
    output.append("\n");
    
    output.append(outputVar("fov", convert(this->fov)));
    output.append("\n");
    
    output.append(outputVar("light_type", convert(this->light_type)));
    output.append("\n");
    
    output.append(outputVar("range", convert(this->range)));
    output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Light"));
    return output;
}
