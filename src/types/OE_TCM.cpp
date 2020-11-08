#include <types/OE_TCM.h>
#include <types/OE_World.h>

using namespace std;

OE_TCM_Texture::OE_TCM_Texture(){}
OE_TCM_Texture::~OE_TCM_Texture(){}


std::string OE_TCM_Texture::to_str() const{
    string output = outputTypeTag("TCM_Texture", {});
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    output.append(outputVar("textureID", "\"" + OE_World::texturesList.id2name[this->textureID] + "\""));
    output.append("\n");
    
    output.append(outputVar("mode", convert(this->mode)));
    output.append("\n");
    
    output.append(outputVar("textureMulFactor", convert(this->textureMulFactor)));
    output.append("\n");
    
    output.append(outputVar("uvmap", convert(this->uvmap)));
    output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("TCM_Texture"));
    return output;
}


size_t OE_TCM::current_id = 0;

OE_TCM::OE_TCM(){
    
    OE_TCM::current_id++;
    
    
    this->r                         = 0.0f; this->g = 0.0f; this->b = 0.0f; this->a = 0.0f;
    this->texture_array             = false;
    this->combine_mode              = 0;
    
    this->id                        = OE_TCM::current_id;
}


OE_TCM::OE_TCM(const string &name){
    
    OE_TCM::current_id++;
    
    this->r                         = 0.0f; this->g = 0.0f; this->b = 0.0f; this->a = 0.0f;
    this->texture_array             = false;
    this->combine_mode              = 0;
    
    this->id                        = OE_TCM::current_id;
}

OE_TCM::~OE_TCM(){

} 
 
string OE_TCM::to_str() const{
    string output = outputTypeTag("TextureCombineMode", {{"name", "\"" + OE_World::tcmsList.id2name[this->id] + "\""}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    output.append(outputVar("texture_array", convert((int)this->texture_array)));
    output.append("\n");
    
    output.append(outputVar("r", convert(this->r)));
    output.append("\n");
    
    output.append(outputVar("g", convert(this->g)));
    output.append("\n");
    
    output.append(outputVar("b", convert(this->b)));
    output.append("\n");
    
    output.append(outputVar("a", convert(this->a)));
    output.append("\n");
    
    output.append(outputVar("combine_mode", convert(this->combine_mode)));
    output.append("\n");
    
    for(const auto& x: this->textures){
        output.append(x.to_str());
        output.append("\n");
    }
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("TextureCombineMode"));
    return output;
}
