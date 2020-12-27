#ifndef NRE_RENDERDATA_H
#define NRE_RENDERDATA_H

#include <types/OE_World.h>
#include <Renderer/NRE_GPU_API.h>
#include <OE_Math.h>

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
    
    std::size_t vbo_bbox;
    std::size_t vao_bbox;   
    
    float max_x{0.0f}, min_x{0.0f}, max_y{0.0f}, min_y{0.0f}, max_z{0.0f}, min_z{0.0f};
    
    std::vector<float> genBoundingBoxVBO();
};

struct NRE_LightRenderData : public NRE_BaseObject{
    OE_Mat4x4       model_mat;
    std::size_t     id{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_RenderGroup{
    
    // data for the z prepass
    NRE_GPU_VertexShader vs_z_prepass;
    std::size_t     z_prepass_program{0};
    bool isZPrePassSetup{false};
    
    // data for normal render
    NRE_GPU_VertexShader vs;
    NRE_GPU_PixelShader fs;    
    std::size_t     program{0};
    bool isSetup{false};
    
    // draw call data
    std::size_t     camera{0};
    std::size_t     material{0};
    std::size_t     vgroup{0};
    std::size_t     mesh{0};  
    
    std::vector<std::size_t>     lights;
    
    // for sorting draw calls
    bool operator < (const NRE_RenderGroup&) const;
};

#endif // FE_MESHRENDERDATA_H
