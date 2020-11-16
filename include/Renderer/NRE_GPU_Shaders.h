#ifndef NRE_GPU_SHADERS_H
#define NRE_GPU_SHADERS_H

#define NRE_Shader(src) #src

#include <types/OE_TypesBase.h>
#include <OE_Math.h>

// possibility to extend with HLSL/Metal etc.
enum NRE_GPU_SHADER_BACKEND{ 
    NRE_GPU_UNDEFINED,
    NRE_GPU_GL,
    NRE_GPU_GLES
};

enum NRE_GPU_VS_TYPE{
    NRE_GPU_VS_UNDEFINED,
    NRE_GPU_VS_Z_PREPASS,
    NRE_GPU_VS_REGULAR
};

enum NRE_GPU_FS_TYPE{
    NRE_GPU_FS_UNDEFINED,
    NRE_GPU_FS_MATERIAL,
    NRE_GPU_FS_NORMALS,
    NRE_GPU_FS_SIMPLE_LIGHT,
    NRE_GPU_FS_DIR_LIGHT,
    NRE_GPU_FS_INDIR_LIGHTS
};

class NRE_GPU_ShaderBase{
public:
    
    static NRE_GPU_SHADER_BACKEND backend;
    static std::string shader_prefix;
    
    static void init(NRE_GPU_SHADER_BACKEND, int, int);
    
    virtual ~NRE_GPU_ShaderBase();
    virtual std::string genShader();
    
    
    
    
};

class NRE_GPU_VertexShader : public NRE_GPU_ShaderBase{
public:
    
    NRE_GPU_VertexShader();
    ~NRE_GPU_VertexShader();
    
    bool operator< (const NRE_GPU_VertexShader&) const;
    bool operator == (const NRE_GPU_VertexShader&) const;
    
    std::string genShader();
    
    bool fullscreenQuad{false};
    std::size_t num_of_uvs{0};
    NRE_GPU_VS_TYPE type{NRE_GPU_VS_UNDEFINED};
    
    
};

class NRE_GPU_PixelShader : public NRE_GPU_ShaderBase{
public:
    
    NRE_GPU_PixelShader();
    ~NRE_GPU_PixelShader();
    
    bool operator< (const NRE_GPU_PixelShader&) const;
    
    std::string genShader();
    
    std::size_t num_of_uvs{0};
    NRE_GPU_FS_TYPE type{NRE_GPU_FS_UNDEFINED};
};

#endif
