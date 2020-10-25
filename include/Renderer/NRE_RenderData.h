#ifndef NRE_RENDERDATA_H
#define NRE_RENDERDATA_H

#include <types/OE_World.h>
//#include "FE_GPU_API.h"
#include <OE_Math.h>

#define NRE_GL32_Shader(src) "#version 150 \n" #src

struct NRE_CameraRenderData{
    OE_Mat4x4       perspective_view_mat;
    std::size_t     id{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_MaterialRenderData{
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
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_MeshRenderData{
    OE_Mat4x4       model_mat;
    std::size_t     id{0};
    
    unsigned int    uvmaps{0};
    unsigned int    bones{0};
    
    std::size_t     vbo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_LightRenderData{
    OE_Mat4x4       model_mat;
    std::size_t     id{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_RenderGroup{
    std::size_t     camera{0};
    std::size_t     material{0};
    std::size_t     vgroup{0};
    std::size_t     mesh{0};
    
    std::vector<std::size_t>     lights;
};

#endif // FE_MESHRENDERDATA_H
