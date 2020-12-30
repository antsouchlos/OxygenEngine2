#include <types/OE_Texture.h>
#include <types/OE_Camera.h>
#include <types/OE_World.h>

using namespace std;


std::atomic<std::size_t> OE_Texture::current_id(0);

OE_Texture::OE_Texture(){
    
    this->source                    = 0;
    this->path                      = "";
    this->camera                    = 0;
    this->custom_data               = nullptr;
    
    this->id                        = ++OE_Texture::current_id;
}


// TODO: YOU HAVE TO BE FUCKING KIDDING ME M8. JUST REMOVE THE CONSTRUCTOR
OE_Texture::OE_Texture(const string &name){
    
    this->source                    = 0;
    this->path                      = "";
    this->camera                    = 0;
    this->custom_data               = nullptr;
    
    this->id                        = ++OE_Texture::current_id;
}

OE_Texture::~OE_Texture(){

} 

string OE_Texture::to_str(){
    
    string output = outputTypeTag("Texture", {{"name", "\"" + OE_World::texturesList.id2name[this->id] + "\""}});
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
        output.append(outputVar("camera", "\"" + OE_World::objectsList.id2name[this->camera] + "\""));
        output.append("\n");
    }
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Texture"));
    return output;
}
