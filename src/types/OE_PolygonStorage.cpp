#include <types/OE_PolygonStorage.h>
#include <Carbon/CSL_Interpreter.h>

using namespace std;


// These should be empty

OE_IndexBufferWrapperBase::OE_IndexBufferWrapperBase(){

}

OE_IndexBufferWrapperBase::~OE_IndexBufferWrapperBase(){
    
}

uint32_t& OE_IndexBufferWrapperBase::operator[](uint32_t* arg){
    
    return this->dummy_var;
}

std::size_t OE_IndexBufferWrapperBase::count(uint32_t* arg){
    return 0;
}
    
std::size_t OE_IndexBufferWrapperBase::size(){
    return 0;
}

    
// These should NOT by empty

OE_IndexBufferUnorderedMap::OE_IndexBufferUnorderedMap(OE_Mesh32* mesh){
    auto lambda_hash = [mesh](const uint32_t* lhs){
        
            // Make sure we are in a 64 bit system
            static_assert(sizeof(size_t) == 8, "Not in 64 bit? What is this? An ancient system?"); 
        
            std::bitset<64> lhs_bits;
        
            for (size_t i=0; i < 2+ mesh->data->num_of_uvs; i++){
                std::bitset<64> temp(lhs[i]);
                if (i%4 >= 2){
                    OE_ReverseBitset(temp);
                }
                if(i%2 != 0){
                    temp = temp << 32;
                }
                lhs_bits |= temp;
            }
            return lhs_bits.to_ullong();
    };
    
    auto lambda_equals = [mesh](const uint32_t* lhs, const uint32_t* rhs){
        for (size_t i=0; i < 2+ mesh->data->num_of_uvs; i++){
            if(lhs[i] != rhs[i])
                return false;
        }
        return true;
    };
    
    this->isInit = true;
    this->data = unordered_map<uint32_t*, uint32_t, std::function<size_t(uint32_t*)>, std::function<bool(uint32_t*, uint32_t*)>>(2, lambda_hash, lambda_equals);
}

OE_IndexBufferUnorderedMap::~OE_IndexBufferUnorderedMap(){
    
}

uint32_t& OE_IndexBufferUnorderedMap::operator[](uint32_t* arg){
    return this->data[arg];
}

std::size_t OE_IndexBufferUnorderedMap::count(uint32_t* arg){
    return this->data.count(arg);
}
    
std::size_t OE_IndexBufferUnorderedMap::size(){
    return this->data.size();
}

OE_IndexBufferMap::OE_IndexBufferMap(OE_Mesh32* mesh){
    auto lambda_func = [mesh](const uint32_t* lhs, const uint32_t* rhs) {
        for(size_t i=0; i< 2+ mesh->data->num_of_uvs; i++){
            if(lhs[i] < rhs[i]){
                return true;
            }
            else if (lhs[i] > rhs[i]){
                return false;
            }
            else continue;
        }
        return false;
    };
    
    this->isInit = true;
    
    //NOTE: The comparator IS VITAL for the algorithm to work correctly and efficiently
    this->data = map<uint32_t*, uint32_t, std::function<bool(uint32_t*, uint32_t*)>>(lambda_func);
}

OE_IndexBufferMap::~OE_IndexBufferMap(){
    
}

uint32_t& OE_IndexBufferMap::operator[](uint32_t* arg){
    return this->data[arg];
}

std::size_t OE_IndexBufferMap::count(uint32_t* arg){
    return this->data.count(arg);
}
    
std::size_t OE_IndexBufferMap::size(){
    return this->data.size();
}



// Now with the triangles

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

void printArray(const uint32_t* x, const uint32_t& arrsize){
    cout << "{ ";
    for (size_t oa=0; oa < arrsize; oa++){
        cout << x[oa] << " ";
    }
    cout << "}";
}


OE_PolygonStorage32::OE_PolygonStorage32(){
    this->vertices = OE_VertexStorage();
    this->num_of_uvs = 0;
    
    this->vbo_mutex.lockMutex();
    this->vbo_mutex.unlockMutex();
    this->ibos_mutex.lockMutex();
    this->ibos_mutex.unlockMutex();
}

OE_PolygonStorage32::~OE_PolygonStorage32(){
    this->triangle_groups.clear();
    
    for (auto &index : this->vertex_buffer){
        delete[] index;
    }
}

// This should be BLAAAZING FAST
uint32_t* OE_PolygonStorage32::addTriangle(uint32_t* indices){
    uint32_t* output = indices;
    bool isFound = false;
    if (this->index_buffer->count(indices) != 0){
        //cout << "IS FOund ";
        //printArray(this->vertex_buffer[this->index_buffer[indices]], 2+num_of_uvs);
        //printArray(indices, 2+num_of_uvs);
        //cout << endl;
        isFound = true;
        output = this->vertex_buffer[(*(this->index_buffer))[indices]];
    }
    if (!isFound){
        (*(this->index_buffer))[indices] = this->vertex_buffer.size();
        this->vertex_buffer.push_back(indices);
    }
    return output;
}

// These two should also be BLAAAZING FAST
// They are VITAL for good renderer performance

std::vector<float> OE_PolygonStorage32::genVertexBuffer(){
    
    //auto t=clock();
    
    vector<float> output;
    // do the expensive allocation at the start
    output.reserve(vertex_buffer.size()*(6+this->num_of_uvs*2));
    
    for (const auto&vertex : this->vertex_buffer){

        output.push_back(this->vertices.positions[vertex[0]*3]);
        output.push_back(this->vertices.positions[vertex[0]*3+1]);
        output.push_back(this->vertices.positions[vertex[0]*3+2]);
        
        output.push_back(this->vertices.normals[vertex[1]*3]);
        output.push_back(this->vertices.normals[vertex[1]*3+1]);
        output.push_back(this->vertices.normals[vertex[1]*3+2]);
        
        for (size_t i=0; i < this->num_of_uvs; i++){
            output.push_back(this->vertices.uvmaps[i].elements[vertex[2+i]*2]);
            output.push_back(this->vertices.uvmaps[i].elements[vertex[2+i]*2+1]);
        }
    } 
    
    this->vertices.calculateNaiveBoundingBox();
    
    //cout << "NRE VERTEX BUFFER: " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    return output;
}

std::vector<uint32_t> OE_PolygonStorage32::genIndexBuffer(const std::size_t &vgroup_id){
	// TODO: Custom error handling
    auto vgroup = this->triangle_groups.at(vgroup_id);

    std::vector<uint32_t> output;
    
    // sort optimizing for vertex cache 
    /*uint32_t vertex_count = this->index_buffer->size();
    std::vector<float> vertex_score; vertex_score.reserve(vertex_count);
    
    vertex_score.push_back(0.75);
    vertex_score.push_back(0.75);
    vertex_score.push_back(0.75);
    
    for (size_t i=3; i< vertex_score.size(); i++){
        
    }*/
    
    
    // do the expensive allocation at the start
    output.reserve(vgroup->polygons.size()*3);
    
    for (const auto& tri : vgroup->polygons){ 
        output.push_back((*(this->index_buffer))[this->triangles[tri].v1]);
        output.push_back((*(this->index_buffer))[this->triangles[tri].v2]);
        output.push_back((*(this->index_buffer))[this->triangles[tri].v3]);
    }
    
    return output;
}
        
/*********************************************/
        
void OE_PolygonStorage32::initUnorderedIB(OE_Mesh32* mesh){
    this->index_buffer = std::make_shared<OE_IndexBufferUnorderedMap>(mesh);
}

void OE_PolygonStorage32::initOrderedIB(OE_Mesh32* mesh){
    this->index_buffer = std::make_shared<OE_IndexBufferMap>(mesh);
}

void OE_PolygonStorage32::genVertexBufferInternally(){
    if (!vbo_exists){
        this->vbo_mutex.lockMutex();
        this->vbo = genVertexBuffer();
        this->vbo_mutex.unlockMutex();
    }
    vbo_exists = true;
    
}

void OE_PolygonStorage32::genIndexBuffersInternally(){
    
    if (!ibos_exist){
        
        //auto t=clock();
        this->ibos_mutex.lockMutex();
        for (auto vgroup : this->triangle_groups){
            this->ibos[vgroup.first] = OE_IndexBufferReady();
            this->ibos[vgroup.first].data = this->genIndexBuffer(vgroup.first);
        }
        this->ibos_mutex.unlockMutex();
        
        //cout << "NRE INDEX BUFFER: " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    }
    ibos_exist = true;
}
