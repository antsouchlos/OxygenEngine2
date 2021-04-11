#include <types/OE_ViewportConfig.h>
#include <types/OE_Camera.h>
#include <types/OE_World.h>

using namespace std;

std::atomic<std::size_t> OE_ViewportConfig::current_id(0);

OE_ViewportConfig::OE_ViewportConfig(){
    
    this->id                                = ++OE_ViewportConfig::current_id;
    this->addLayer();
}


OE_ViewportConfig::OE_ViewportConfig(const string &name){

    this->id                                = ++OE_ViewportConfig::current_id;    
    this->addLayer();
}

OE_ViewportConfig::~OE_ViewportConfig(){

}

void OE_ViewportConfig::addLayer(){
    
    this->layer_combine_modes.push_back(1);
    this->split_screen_positions.push_back(0.5f);
    this->split_screen_positions.push_back(0.5f);

}

void OE_ViewportConfig::addCamera(std::size_t cam_id, std::size_t layer){
    
    for (size_t i=0; i< this->cameras.size(); i++){
        if ((this->cameras[i] == cam_id) && (this->layers[i] == layer)){
            return;
        }
    }
    
    this->cameras.push_back(cam_id);
    this->layers.push_back(layer);
    this->camera_modes.push_back(1);
}

void OE_ViewportConfig::removeCamera(std::size_t cam_id, std::size_t layer){
    
    for (size_t i=0; i< this->cameras.size(); i++){
        if ((this->cameras[i] == cam_id) && (this->layers[i] == layer)){
            this->cameras.erase(this->cameras.begin() + i);
            this->layers.erase(this->layers.begin() + i);
            this->camera_modes.erase(this->camera_modes.begin() + i);
            break;
        }
    }
}

void OE_ViewportConfig::removeLayer(std::size_t id){

    auto layer_size = this->layer_combine_modes.size();
    if (id < layer_size){
        this->layer_combine_modes.erase(this->layer_combine_modes.begin() + id);
        this->split_screen_positions.erase(this->split_screen_positions.begin() + id*2); 
        this->split_screen_positions.erase(this->split_screen_positions.begin() + id*2); 
    }
}
        
void OE_ViewportConfig::setCameraMode(std::size_t cam_id, int mode){

    for (size_t i=0; i< this->cameras.size(); i++){
        if (this->cameras[i] == cam_id){
            this->camera_modes[i] = mode;
        }
    }
}

void OE_ViewportConfig::setCameraLayerMode(std::size_t cam_id, std::size_t layer, int mode){
    for (size_t i=0; i< this->cameras.size(); i++){
        if ((this->cameras[i] == cam_id) && (this->layers[i] == layer)){
            this->camera_modes[i] = mode;
            break;
        }
    }
}
        
void OE_ViewportConfig::setLayerCombineMode(std::size_t id, int layer){
    auto layer_size = this->layer_combine_modes.size();
    if (id < layer_size){
        this->layer_combine_modes[id] = layer;
    }
}

void OE_ViewportConfig::setLayerSplitScreenPosHorizontal(std::size_t id, float num){
    auto layer_size = this->layer_combine_modes.size();
    if (id < layer_size){
        this->split_screen_positions[id*2] = num;
    }
}

void OE_ViewportConfig::setLayerSplitScreenPosVertical(std::size_t, float num){
    auto layer_size = this->layer_combine_modes.size();
    if (id < layer_size){
        this->split_screen_positions[id*2+1] = num;
    }
}

bool OE_ViewportConfig::existsLayer(std::size_t layer){
    return layer < this->layer_combine_modes.size();
}

bool OE_ViewportConfig::existsCamera(std::size_t cam_id){
    return std::find(this->cameras.begin(), this->cameras.end(), cam_id) != this->cameras.end();
}

bool OE_ViewportConfig::existsCameraLayer(std::size_t cam_id, std::size_t layer){
    for (size_t i=0; i< this->cameras.size(); i++){
        if ((this->cameras[i] == cam_id) && (this->layers[i] == layer)){
            return true;
        }
    }
    return false;
}

std::string OE_ViewportConfig::to_str() const{
    
    string output = outputTypeTag("ViewportConfig", {{"name", "\"" + OE_World::viewportsList.id2name_[this->id] + "\""}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    output.append(outputList("layers", this->layers));
    output.append("\n");
    
    vector<string> camera_strs;
    for(const auto &x: this->cameras){
        camera_strs.push_back("\"" + OE_World::objectsList.id2name_[x] + "\"");
    }
    output.append(outputList("cameras", camera_strs));
    output.append("\n");
    
    output.append(outputList("camera_modes", this->camera_modes));
    output.append("\n");
    
    output.append(outputList("layer_combine_modes", this->layer_combine_modes));
    output.append("\n");
    
    output.append(outputList("split_screen_positions", this->split_screen_positions));
    output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("ViewportConfig"));
    return output;
}
