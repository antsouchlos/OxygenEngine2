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
            NRE_GPU_ShaderBase::shader_prefix = "#version 150 \n"
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

std::string NRE_GPU_PixelShader::genShader(){
    if (NRE_GPU_ShaderBase::backend == NRE_GPU_GL || NRE_GPU_ShaderBase::backend == NRE_GPU_GLES)
        return NRE_GenGL3PixelShader(*this);
    return "";
}
