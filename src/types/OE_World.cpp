#include <types/OE_World.h>

using namespace std;

OE_SharedIndexMap<OE_Scene>             OE_World::scenesList = OE_SharedIndexMap<OE_Scene>();
OE_SharedIndexMap<OE_Object>            OE_World::objectsList = OE_SharedIndexMap<OE_Object>();
OE_SharedIndexMap<OE_Material>          OE_World::materialsList = OE_SharedIndexMap<OE_Material>();
OE_SharedIndexMap<OE_Texture>           OE_World::texturesList = OE_SharedIndexMap<OE_Texture>();
OE_SharedIndexMap<OE_TCM>               OE_World::tcmsList = OE_SharedIndexMap<OE_TCM>();
OE_SharedIndexMap<OE_ViewportConfig>    OE_World::viewportsList = OE_SharedIndexMap<OE_ViewportConfig>();

OE_World::OE_World(){
    
    this->loaded_scene      = 0;
    this->loaded_viewport   = 0;
    
}

OE_World::~OE_World(){
    
    for (auto& x: scenes)
        OE_World::scenesList.remove(x);
    for (auto &x: viewports)
        OE_World::viewportsList.remove(x);
    
}

string OE_World::to_str() const{
    
    string output = outputTypeTag("World", {});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    for (const auto& x: this->scenes){
        output.append(OE_World::scenesList[x].p_->to_str());
        output.append("\n");
    }
    
    for (const auto& x: this->viewports){
        output.append(OE_World::viewportsList[x].p_->to_str());
        output.append("\n");
    }
    
    output.append(outputVar("loaded_scene", "\"" + OE_World::scenesList.id2name_[this->loaded_scene] + "\""));
    output.append("\n");
    
    output.append(outputVar("loaded_viewport", "\"" + OE_World::viewportsList.id2name_[this->loaded_viewport] + "\""));
    output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("World"));
    return output;
}

