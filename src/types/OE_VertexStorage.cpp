#include <types/OE_VertexStorage.h>

using namespace std;

std::unordered_map<std::size_t, std::string> OE_UVMapData::id2name;
OE_Name2ID          OE_UVMapData::name2id = OE_Name2ID(&OE_UVMapData::id2name);

OE_UVMapData::OE_UVMapData(){

}

OE_UVMapData::OE_UVMapData(const size_t &ida, const string &name_a){
    this->id                        = ida;
    OE_UVMapData::id2name[this->id] = name_a;
}

OE_UVMapData::~OE_UVMapData(){
    
}

std::string OE_UVMapData::to_str() const{
    
    string output = outputTypeTag("UVMapData", {{"name", "\"" + id2name[this->id] + "\""}} );
    output.append("\n");
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    
    output.append(outputVar("num_of_uvs", convert(this->elements.size())));
    output.append("\n");
    
    output.append(outputList("elements", this->elements));
    output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("UVMapData"));
    return output;
}


OE_VertexStorage::OE_VertexStorage(){
    
}

OE_VertexStorage::~OE_VertexStorage(){
    
}

