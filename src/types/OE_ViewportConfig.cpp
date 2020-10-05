#include <types/OE_ViewportConfig.h>
#include <types/OE_Camera.h>

using namespace std;

size_t              OE_ViewportConfig::current_id = 0;
map<size_t, string> OE_ViewportConfig::id2name;
OE_Name2ID          OE_ViewportConfig::name2id = OE_Name2ID(&OE_ViewportConfig::id2name);

OE_ViewportConfig::OE_ViewportConfig(){
    
    OE_ViewportConfig::current_id++;
    
    this->id                                = OE_ViewportConfig::current_id;
    OE_ViewportConfig::id2name[this->id]    = "noname_" + to_string(this->id);
    
}


OE_ViewportConfig::OE_ViewportConfig(const string &name){
    
    OE_ViewportConfig::current_id++;
    
    this->id                                = OE_ViewportConfig::current_id;
    OE_ViewportConfig::id2name[this->id]    = name;
    
}

OE_ViewportConfig::~OE_ViewportConfig(){

}
 
std::string OE_ViewportConfig::to_str() const{
    
    string output = outputTypeTag("ViewportConfig", {{"name", "\"" + id2name[this->id] + "\""}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    output.append(outputList("layers", this->layers));
    output.append("\n");
    
    vector<string> camera_strs;
    for(const auto &x: this->cameras){
        camera_strs.push_back("\"" + OE_Camera::id2name[x] + "\"");
    }
    output.append(outputList("cameras", camera_strs));
    output.append("\n");
    
    output.append(outputList("camera_modes", this->camera_modes));
    output.append("\n");
    
    output.append(outputList("layer_combine_modes", this->layer_combine_modes));
    output.append("\n");
    
    output.append(outputList("split_screen_positions", this->split_screen_positions));
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("ViewportConfig"));
    return output;
}
