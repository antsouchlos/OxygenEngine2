#include <types/OE_Mesh.h>

using namespace std;

OE_Mesh32::OE_Mesh32(){
    
   this->physics_data     = nullptr;
    
}

OE_Mesh32::OE_Mesh32(const string& name) : OE_Object(name){
    
    this->physics_data     = nullptr;
}

OE_Mesh32::~OE_Mesh32(){
    
}

string OE_Mesh32::getType() const{
    return "MESH32";
}

std::string OE_Mesh32::to_str() const{
    
    string output = "";
    //vector<string> output_list;
    //output_list.reserve(9+ this->textureCM_IDs.size() + this->data.vertices.uvmaps.size() + this->data.triangles.size() + this->data.triangle_groups.size()+1);
    
    output.append(outputTypeTag("Mesh", {{"name", "\"" + id2name[this->id] + "\""}, {"visible", convert((int)visible)}, {"num_uvs", convert(data.vertices.uvmaps.size())}}));
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    output.append("\n");
    
    output.append(outputList("current_state", this->current_state.to_arr()));
    output.append("\n");
    
    output.append(outputVar("parent", "\"" + OE_Object::id2name[this->parent] + "\""));
    output.append("\n");
    
    output.append(outputVar("parent_type", convert(this->parent_type)));
    output.append("\n");
    
    output.append(outputVar("num_of_vertices", convert(this->data.vertices.positions.size())));
    output.append("\n");
    
    output.append(outputVar("num_of_normals", convert(this->data.vertices.normals.size())));
    output.append("\n");
    
    output.append(outputVar("num_of_triangles", convert(this->data.triangles.size())));
    output.append("\n");
    
    output.append(outputList("vertices", this->data.vertices.positions));
    output.append("\n");
    
    output.append(outputList("normals", this->data.vertices.normals));
    vector<string> tcm_strs;
    for(const auto &x: this->textureCM_IDs){
        tcm_strs.push_back("\"" + OE_TCM::id2name[x] + "\"");
    }
    if(tcm_strs.size() != 0){
        output.append(outputList("textureCM_IDs", tcm_strs));
        output.append("\n");
    }
    
    for (const auto& x: this->data.vertices.uvmaps){
        output.append(x.to_str());
        output.append("\n");
    }
    for (const auto& x: this->data.triangles){
        output.append(x.to_str(2+this->data.num_of_uvs));
        output.append("\n");
    }
    for (const auto& x: this->data.triangle_groups){
        output.append(x.second->to_str());
        output.append("\n");
    }
    output = output.substr(0, output.size()-1);
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Mesh"));
    //return CSL_Join("\n", output_list);
    return output;
}
