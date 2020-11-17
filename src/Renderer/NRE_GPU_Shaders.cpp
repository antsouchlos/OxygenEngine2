#include <Renderer/NRE_GL3_Shaders.h>

using namespace std;

// General boilerplate

NRE_GPU_SHADER_BACKEND NRE_GPU_ShaderBase::backend = NRE_GPU_UNDEFINED;
std::string NRE_GPU_ShaderBase::shader_prefix = "";

NRE_GPU_ShaderBase::~NRE_GPU_ShaderBase(){
    
}

void NRE_GPU_ShaderBase::init(NRE_GPU_SHADER_BACKEND backend, int major, int minor){
    
    NRE_GPU_ShaderBase::backend = backend;
    
    if (backend == NRE_GPU_GLES){
        NRE_GPU_ShaderBase::shader_prefix = "#version 300 es \nprecision highp float; \n";
    }
    else if (backend == NRE_GPU_GL){
        
        if (major == 3 && minor == 1){
            NRE_GPU_ShaderBase::shader_prefix = "#version 140 \n"
                                 "#extension GL_ARB_explicit_attrib_location"
                                 ": require\n";
        } 
        else if (major == 3 && minor >= 2){
            NRE_GPU_ShaderBase::shader_prefix = "#version 150 core \n"
                                 "#extension GL_ARB_explicit_attrib_location"
                                 ": require\n";
        } 
        else {
            cout << "WTF" << endl;
        }
    }
    else {
        cout << "Undefined NRE GPU Shader backend. Nothing is rendered" << endl;
    }
    
}


std::string NRE_GPU_ShaderBase::genShader(){
    return "";
}

// Vertex Shader

NRE_GPU_VertexShader::NRE_GPU_VertexShader(){
    
}

NRE_GPU_VertexShader::~NRE_GPU_VertexShader(){
    
}

bool NRE_GPU_VertexShader::operator< (const NRE_GPU_VertexShader& other) const{
    if (this->num_of_uvs < other.num_of_uvs){
        return true;
    }
    else if (this->num_of_uvs > other.num_of_uvs){
        return false;
    }
    else {
        //both are equal
        if (this->type < other.type){
            return true;
        } 
        else if (this->type > other.type){
            return false;
        }
        else {
            return this->fullscreenQuad < other.fullscreenQuad;
        }
    }
    return false;
}

bool NRE_GPU_VertexShader::operator == (const NRE_GPU_VertexShader& other) const{
    return (this->fullscreenQuad == other.fullscreenQuad) && (this->type == other.type) && (this->num_of_uvs == other.num_of_uvs);
}

std::string NRE_GPU_VertexShader::genShader(){
    if (NRE_GPU_ShaderBase::backend == NRE_GPU_GL || NRE_GPU_ShaderBase::backend == NRE_GPU_GLES)
        return NRE_GenGL3VertexShader(*this);
    return "";
}

// Fragment/Pixel Shader

NRE_GPU_PixelShader::NRE_GPU_PixelShader(){
    
}

NRE_GPU_PixelShader::~NRE_GPU_PixelShader(){
    
}

bool NRE_GPU_PixelShader::operator< (const NRE_GPU_PixelShader& other) const{
    if (this->num_of_uvs < other.num_of_uvs){
        return true;
    }
    else if (this->num_of_uvs > other.num_of_uvs){
        return false;
    }
    else {
        //both are equal
        return this->type < other.type;
    }
    return false;
}

std::string NRE_GPU_PixelShader::genShader(){
    if (NRE_GPU_ShaderBase::backend == NRE_GPU_GL || NRE_GPU_ShaderBase::backend == NRE_GPU_GLES)
        return NRE_GenGL3PixelShader(*this);
    return "";
}
