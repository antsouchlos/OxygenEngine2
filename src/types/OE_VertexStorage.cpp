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

/*****************BOUNDING BOX****************************/
void OE_VertexStorage::calculateNaiveBoundingBox(){
    
    assert(this->positions.size() >= 3);
    
    // find min/max position on each axis
    this->max_x = this->positions[0];
    this->max_y = this->positions[1];
    this->max_z = this->positions[2];
    
    this->min_x = this->positions[0];
    this->min_y = this->positions[1];
    this->min_z = this->positions[2];
    
    for (size_t i=0; i < this->positions.size(); i++){
        if (i % 3 == 0){
            if (this->positions[i] < this->min_x){
                this->min_x = this->positions[i];
                continue;
            }
            if (this->positions[i] > this->max_x){
                this->max_x = this->positions[i];
                continue;
            }
        } 
        else if (i % 3 == 1){
            if (this->positions[i] < this->min_y){
                this->min_y = this->positions[i];
                continue;
            }
            if (this->positions[i] > this->max_y){
                this->max_y = this->positions[i];
                continue;
            }
        } 
        else {
            if (this->positions[i] < this->min_z){
                this->min_z = this->positions[i];
                continue;
            }
            if (this->positions[i] > this->max_z){
                this->max_z = this->positions[i];
                continue;
            }
        }
    }
    
    this->calculatedBoundingBox = true;
}

std::vector<float> OE_VertexStorage::genBoundingBoxMesh(){
    
    std::vector<float> output;
    
    if (!this->calculatedBoundingBox)
        return output;
    
    output.reserve(216);
    
    // max_x
    output.push_back(max_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    
    output.push_back(max_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    
    // min_x
    output.push_back(min_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    
    // max_y
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    
    output.push_back(min_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    
    // min_y
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    
    output.push_back(max_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    
    // max_z
    output.push_back(min_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    
    // min_z
    output.push_back(max_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    
    return output;
}
