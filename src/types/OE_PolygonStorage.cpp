#include <types/OE_PolygonStorage.h>

using namespace std;

OE_Triangle32::OE_Triangle32(){
    
}

OE_Triangle32::~OE_Triangle32(){
    
}

string OE_Triangle32::to_str(const size_t &arraysize) const{
    string temp1 = outputTypeTag("Triangle", {});
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    //output.append("\n");
    
    vector<uint32_t> vertex(arraysize);
    
    std::copy(this->v1, this->v1+arraysize, vertex.begin());
    string temp2 = outputList("v1", vertex);
    //output.append("\n");
    
    std::copy(this->v2, this->v2+arraysize, vertex.begin());
    string temp3 = outputList("v2", vertex);
    //output.append("\n");
    
    std::copy(this->v3, this->v3+arraysize, vertex.begin());
    string temp4 = outputList("v3", vertex);
    //output.append("\n");
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    string temp5 = outputClosingTag("Triangle");
    return CSL_Join("\n", {temp1, temp2, temp3, temp4, temp5});
}


OE_PolygonStorage32::OE_PolygonStorage32(){
    this->vertices = OE_VertexStorage();
    this->num_of_uvs = 0;
}

OE_PolygonStorage32::~OE_PolygonStorage32(){
    for (auto &vgroup : this->triangle_groups){
        delete vgroup.second;
    }
    this->triangle_groups.clear();
    
    for (auto &index : this->vertex_buffer){
        delete[] index;
    }
}

void printArray(const uint32_t* x, const uint32_t& arrsize){
    cout << "{ ";
    for (size_t oa=0; oa < arrsize; oa++){
        cout << x[oa] << " ";
    }
    cout << "}";
}

// This should be BLAAAZING FAST
uint32_t* OE_PolygonStorage32::addTriangle(uint32_t* indices){
    uint32_t* output = indices;
    bool isFound = false;
    if (this->index_buffer.count(indices) != 0){
        //cout << "IS FOund ";
        //printArray(this->vertex_buffer[this->index_buffer[indices]], 2+num_of_uvs);
        //printArray(indices, 2+num_of_uvs);
        //cout << endl;
        isFound = true;
        output = this->vertex_buffer[this->index_buffer[indices]];
    }
    if (!isFound){
        this->index_buffer[indices] = this->vertex_buffer.size();
        this->vertex_buffer.push_back(indices);
    }
    return output;
}
