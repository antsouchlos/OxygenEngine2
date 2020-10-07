#include <types/OE_Texture.h>
#include <types/OE_Camera.h>

using namespace std;


size_t              OE_Texture::current_id = 0;
unordered_map<size_t, string> OE_Texture::id2name;
OE_Name2ID          OE_Texture::name2id = OE_Name2ID(&OE_Texture::id2name);

OE_Texture::OE_Texture(){
    
    OE_Texture::current_id++;
    
    this->source                    = 0;
    this->path                      = "";
    this->camera                    = 0;
    this->custom_data               = nullptr;
    
    this->id                        = OE_Texture::current_id;
    OE_Texture::id2name[this->id]   = "noname_" + to_string(this->id);
    
}


OE_Texture::OE_Texture(const string &name){
    
    OE_Texture::current_id++;
    
    this->source                    = 0;
    this->path                      = "";
    this->camera                    = 0;
    this->custom_data               = nullptr;
    
    this->id                        = OE_Texture::current_id;
    OE_Texture::id2name[this->id]   = name;
    
}

OE_Texture::~OE_Texture(){

} 

string OE_Texture::to_str(){
    
    string output = outputTypeTag("Texture", {{"name", "\"" + id2name[this->id] + "\""}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    output.append(outputVar("source", convert(this->source)));
    output.append("\n");
    
    output.append(outputVar("path", "\"" + this->path + "\""));
    output.append("\n");
    
    if (this->camera != 0){
        output.append(outputVar("camera", "\"\""));
        output.append("\n");
    }
    else{
        output.append(outputVar("camera", "\"" + OE_Camera::id2name[this->camera] + "\""));
        output.append("\n");
    }
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Texture"));
    return output;
}
