#include <types/OE_World.h>

using namespace std;


OE_World::OE_World(){
    
    this->loaded_scene      = 0;
    this->loaded_viewport   = 0;
    
}

OE_World::~OE_World(){
    
    for (auto& x: scenes)
        delete x.second;
    for (auto &x: viewports)
        delete x.second;
    
}

string OE_World::to_str() const{
    
    string output = outputTypeTag("World", {});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    for (const auto& x: this->scenes){
        output.append(x.second->to_str());
        output.append("\n");
    }
    
    for (const auto& x: this->viewports){
        output.append(x.second->to_str());
        output.append("\n");
    }
    
    output.append(outputVar("loaded_scene", "\"" + OE_Scene::id2name[this->loaded_scene] + "\""));
    output.append("\n");
    
    output.append(outputVar("loaded_viewport", "\"" + OE_ViewportConfig::id2name[this->loaded_viewport] + "\""));
    output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("World"));
    return output;
}

