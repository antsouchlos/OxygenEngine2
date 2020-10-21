#include <Renderer/NRE_GPU_API.h>

NRE_GPU_API::NRE_GPU_API(){
    
}

NRE_GPU_API::~NRE_GPU_API(){
    
}

std::size_t NRE_GPU_API::newVertexBuffer(){ return 0;}
std::size_t NRE_GPU_API::newVertexLayout(){ return 0;}
std::size_t NRE_GPU_API::newIndexBuffer(){ return 0;}
std::size_t NRE_GPU_API::newProgram(){ return 0;}
std::size_t NRE_GPU_API::newUniformBuffer(){ return 0;}


void NRE_GPU_API::setVertexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE){}
void NRE_GPU_API::setVertexBufferData(std::size_t, const std::vector<float>&, std::size_t){}
void NRE_GPU_API::deleteVertexBuffer(std::size_t){}
    
void NRE_GPU_API::setIndexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE){}
void NRE_GPU_API::setIndexBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t){}
void NRE_GPU_API::deleteIndexBuffer(std::size_t){}
    
void NRE_GPU_API::setUniformBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE){}
void NRE_GPU_API::setUniformBufferData(std::size_t, const std::vector<float>&, std::size_t){}
void NRE_GPU_API::setUniformBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t){}
void NRE_GPU_API::setProgramUniformSlot(std::size_t, std::size_t, int){}
void NRE_GPU_API::setUniformState(std::size_t, int, uint32_t, uint32_t){}
void NRE_GPU_API::deleteUniformBuffer(std::size_t){}
    
void NRE_GPU_API::setVertexLayoutFormat(std::size_t, std::vector<NRE_GPU_VertexLayoutInput>){}
void NRE_GPU_API::deleteVertexLayout(std::size_t){}

void NRE_GPU_API::setProgramVS(std::size_t, std::string){}
//void setProgramGS(std::size_t, FE_GPU_Shader){}
void NRE_GPU_API::setProgramFS(std::size_t, std::string){}
//void setProgramTCS(std::size_t, FE_GPU_Shader){}
//void setProgramTES(std::size_t, FE_GPU_Shader){}
void NRE_GPU_API::setupProgram(std::size_t){}

void NRE_GPU_API::draw(std::size_t, std::size_t, int, int){}
void NRE_GPU_API::draw(std::size_t, std::size_t){}
    
void NRE_GPU_API::draw(std::size_t, std::size_t, std::size_t, int, int){}
void NRE_GPU_API::draw(std::size_t, std::size_t, std::size_t){}
