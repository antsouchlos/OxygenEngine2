#include <Renderer/NRE_GL3_Shaders.h>
#include <iostream>
#include <sstream>

using namespace std;

// General boilerplate

NRE_GPU_SHADER_BACKEND NRE_GPU_ShaderBase::backend = NRE_GPU_UNDEFINED;
std::string NRE_GPU_ShaderBase::shader_prefix = "";

NRE_GPU_ShaderBase::~NRE_GPU_ShaderBase(){
    
}

void NRE_GPU_ShaderBase::init(NRE_GPU_SHADER_BACKEND backend, int major, int minor){
    
    NRE_GPU_ShaderBase::backend = backend;
    
    if (backend == NRE_GPU_GLES){
        NRE_GPU_ShaderBase::shader_prefix = "#version 300 es \n";
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

std::string NRE_GPU_ShaderBase::info(){
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

std::string NRE_GPU_VertexShader::info(){
    
    stringstream ss;
    switch(this->type){
        case NRE_GPU_VS_UNDEFINED:
            ss << "VS_UNDEFINED";
            break;
        case NRE_GPU_VS_Z_PREPASS:
            ss << "VS_Z_PREPASS";
            break;
        case NRE_GPU_VS_REGULAR:
            ss << "VS_REGULAR";
            break;
        case NRE_GPU_VS_BOUNDING_BOX:
            ss << "VS_BOUNDING_BOX";
            break;
        case NRE_GPU_VS_BOUNDING_SPHERE:
            ss << "VS_BOUNDING_SPHERE";
            break;
        case NRE_GPU_VS_LIGHT:
            ss << "VS_LIGHT";
            break;
    }
    
    return ss.str();
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

std::string NRE_GPU_PixelShader::info(){
    stringstream ss;
    switch(this->type){
        case NRE_GPU_FS_UNDEFINED:
            ss << "FS_UNDEFINED";
            break;
        case NRE_GPU_FS_SIMPLE:
            ss << "FS_SIMPLE";
            break;
        case NRE_GPU_FS_GAMMA:
            ss << "FS_GAMMA";
            break;
        case NRE_GPU_FS_MATERIAL:
            ss << "FS_MATERIAL";
            break;
        case NRE_GPU_FS_NORMALS:
            ss << "FS_NORMALS";
            break;
        case NRE_GPU_FS_SIMPLE_LIGHT:
            ss << "FS_SIMPLE_LIGHT";
            break;
        case NRE_GPU_FS_DIR_LIGHT:
            ss << "FS_DIR_LIGHT";
            break;
        case NRE_GPU_FS_INDIR_LIGHTS:
            ss << "FS_INDIR_LIGHTS";
            break;
        case NRE_GPU_FS_LIGHT_INDEX:
            ss << "FS_LIGHT_INDEX";
            break;
    }
    
    return ss.str();
}
