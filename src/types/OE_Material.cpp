#include <types/OE_Material.h>
#include <types/OE_World.h>

using namespace std;

std::atomic<std::size_t> OE_Material::current_id(0);
//unordered_map<size_t, string> OE_Material::id2name;
//OE_Name2ID          OE_Material::name2id = OE_Name2ID(&OE_Material::id2name);

OE_Material::OE_Material(){    
    
    this->id                        = ++OE_Material::current_id;
}


OE_Material::OE_Material(const string &name){

    this->id                        = ++OE_Material::current_id;
}

OE_Material::~OE_Material(){

}  

std::vector<float> OE_Material::GetRendererData(){
    
    vector<float> output;
    output.reserve(12);
    
    output.push_back(this->dif_r);
    output.push_back(this->dif_g);
    output.push_back(this->dif_b);
    output.push_back(this->dif_a);
    
    output.push_back(this->scol_r);
    output.push_back(this->scol_g);
    output.push_back(this->scol_b);
    output.push_back(this->alpha);
    
    output.push_back(this->specular_intensity);
    output.push_back(this->specular_hardness);
    output.push_back(this->translucency);
    output.push_back(this->illuminosity);
    
    return output;
}

string OE_Material::to_str(){
    string output = outputTypeTag("Material", {{"name", "\"" + OE_World::materialsList.id2name[this->id] + "\""}});
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;

    vector<string> tcm_strs;
    for(const auto &x: this->textureCM_IDs){
        tcm_strs.push_back("\"" + OE_World::tcmsList.id2name[x] + "\"");
    }
    if(tcm_strs.size() != 0){
        output.append(outputList("textureCM_IDs", tcm_strs));
        output.append("\n");
    }
    output.append(outputVar("dif_r", convert(this->dif_r)));
    output.append("\n");
    
    output.append(outputVar("dif_g", convert(this->dif_g)));
    output.append("\n");
    
    output.append(outputVar("dif_b", convert(this->dif_b)));
    output.append("\n");
    
    output.append(outputVar("dif_a", convert(this->dif_a)));
    output.append("\n");
    
    output.append(outputVar("scol_r", convert(this->scol_r)));
    output.append("\n");
    
    output.append(outputVar("scol_g", convert(this->scol_g)));
    output.append("\n");
    
    output.append(outputVar("scol_b", convert(this->scol_b)));
    output.append("\n");
    
    output.append(outputVar("alpha", convert(this->alpha)));
    output.append("\n");
    
    output.append(outputVar("translucency", convert(this->translucency)));
    output.append("\n");
    
    output.append(outputVar("illuminosity", convert(this->illuminosity)));
    output.append("\n");
    
    output.append(outputVar("specular_hardness", convert(this->specular_hardness)));
    output.append("\n");
    
    output.append(outputVar("specular_intensity", convert(this->specular_intensity)));
    output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Material"));
    return output;
}
