#ifndef NRE_RENDERDATA_H
#define NRE_RENDERDATA_H

#include <types/OE_World.h>
//#include "FE_GPU_API.h"
#include <OE_Math.h>
#include <Renderer/NRE_GPU_API.h>

struct NRE_BaseObject{
    bool changed{false};
    std::vector<float> data;
};

struct NRE_CameraRenderData : public NRE_BaseObject{
    OE_Mat4x4       perspective_view_mat;
    std::size_t     id{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_MaterialRenderData : public NRE_BaseObject{
    std::size_t     id{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_VGroupRenderData{
    OE_Mat4x4       bone_mat;
    std::size_t     id{0};
    std::size_t     ibo{0};
    std::size_t     material_id{0};
    std::size_t     mesh_id{0};
    
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_MeshRenderData : public NRE_BaseObject{
    OE_Mat4x4       model_mat;
    std::size_t     id{0};
    
    unsigned int    uvmaps{0};
    unsigned int    bones{0};
    
    std::size_t     vbo{0};
    std::size_t     vao{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
    
    bool vao_initialized{false};
    
    std::vector<NRE_GPU_VertexLayoutInput> vao_input;
};

struct NRE_LightRenderData : public NRE_BaseObject{
    OE_Mat4x4       model_mat;
    std::size_t     id{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_RenderGroup{
    
    NRE_GPU_VertexShader vs;
    NRE_GPU_PixelShader fs;    
    
    std::size_t     camera{0};
    std::size_t     material{0};
    std::size_t     vgroup{0};
    std::size_t     mesh{0};
    
    std::size_t     program{0};
    bool isSetup{false};
    std::vector<std::size_t>     lights;
};

#endif // FE_MESHRENDERDATA_H
