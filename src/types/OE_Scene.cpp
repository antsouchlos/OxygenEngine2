#include <types/OE_Scene.h>
#include <types/OE_World.h>

using namespace std;

size_t OE_Scene::current_id = 0;

OE_Scene::OE_Scene(){
    
    OE_Scene::current_id++;
    
    this->id                        = OE_Scene::current_id;
}


OE_Scene::OE_Scene(const string &name){
    
    OE_Scene::current_id++;
    
    this->id                        = OE_Scene::current_id;
}

OE_Scene::~OE_Scene(){
    for (auto object : this->objects){
        OE_World::objectsList.remove(object);
    }
    this->objects.clear();
    
    for (auto material : this->materials){
        OE_World::materialsList.remove(material);
    }
    this->materials.clear();
    
    for (auto tcm : this->texture_CMs){
        OE_World::tcmsList.remove(tcm);
    }
    this->texture_CMs.clear();
    
    for (auto texture : this->textures){
        OE_World::texturesList.remove(texture);
    }
    this->textures.clear();
}

string OE_Scene::to_str(){
    
    string output = outputTypeTag("Scene", {{"name", "\"" + OE_World::scenesList.id2name[this->id] + "\""}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    for (const auto& x: this->textures){
        output.append(OE_World::texturesList[x]->to_str());
        output.append("\n");
    }
    
    for (const auto& x: this->texture_CMs){
        output.append(OE_World::tcmsList[x]->to_str());
        output.append("\n");
    }
    
    for (const auto& x: this->materials){
        output.append(OE_World::materialsList[x]->to_str());
        output.append("\n");
    }
    
    for (const auto& x: this->objects){
        if(OE_World::objectsList[x]->getType() == "MESH32"){
            output.append(OE_World::objectsList[x]->to_str());
            output.append("\n");
        }
    }
    for (const auto& x: this->objects){
        if(OE_World::objectsList[x]->getType() == "LIGHT"){
            output.append(OE_World::objectsList[x]->to_str());
            output.append("\n");
        }
    }
    for (const auto& x: this->objects){
        if(OE_World::objectsList[x]->getType() == "CAMERA"){
            output.append(OE_World::objectsList[x]->to_str());
            output.append("\n");
        }
    }
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Scene"));
    return output;
}
