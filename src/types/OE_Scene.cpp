#include <types/OE_Scene.h>

using namespace std;

size_t OE_Scene::current_id = 0;
map<size_t, string> OE_Scene::id2name;
OE_Name2ID          OE_Scene::name2id = OE_Name2ID(&OE_Scene::id2name);


OE_Scene::OE_Scene(){
    
    OE_Scene::current_id++;
    
    this->id                        = OE_Scene::current_id;
    OE_Scene::id2name[this->id]     = "noname_" + to_string(this->id);
    
}


OE_Scene::OE_Scene(const string &name){
    
    OE_Scene::current_id++;
    
    this->id                        = OE_Scene::current_id;
    OE_Scene::id2name[this->id]     = name;
    
}

OE_Scene::~OE_Scene(){
    for (auto object : this->objects){
        delete object.second;
    }
    this->objects.clear();
    
    for (auto material : this->materials){
        delete material.second;
    }
    this->materials.clear();
    
    for (auto tcm : this->texture_CMs){
        delete tcm.second;
    }
    this->texture_CMs.clear();
    
    for (auto texture : this->textures){
        delete texture.second;
    }
    this->textures.clear();
}

string OE_Scene::to_str(){
    
    string output = outputTypeTag("Scene", {{"name", "\"" + id2name[this->id] + "\""}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    for (const auto& x: this->textures){
        output.append(x.second->to_str());
        output.append("\n");
    }
    
    for (const auto& x: this->texture_CMs){
        output.append(x.second->to_str());
        output.append("\n");
    }
    
    for (const auto& x: this->materials){
        output.append(x.second->to_str());
        output.append("\n");
    }
    
    for (const auto& x: this->objects){
        if(x.second->getType() == "MESH32"){
            output.append(x.second->to_str());
            output.append("\n");
        }
    }
    for (const auto& x: this->objects){
        if(x.second->getType() == "LIGHT"){
            output.append(x.second->to_str());
            output.append("\n");
        }
    }
    for (const auto& x: this->objects){
        if(x.second->getType() == "CAMERA"){
            output.append(x.second->to_str());
            output.append("\n");
        }
    }
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Scene"));
    return output;
}
