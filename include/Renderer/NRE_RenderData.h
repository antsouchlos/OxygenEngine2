#ifndef NRE_RENDERDATA_H
#define NRE_RENDERDATA_H

#include <types/OE_World.h>
#include <Renderer/NRE_GPU_API.h>
#include <Renderer/NRE_DrawCallContainer.h>
#include <memory>

class OE_Mat4x4;

struct NRE_BaseObject{
    
    std::size_t     id{0};
    bool changed{false};
    bool has_init{false};
    std::vector<float> data;
};

struct NRE_CameraRenderData : public NRE_BaseObject{
    OE_Mat4x4       perspective_view_mat;
    OE_Mat4x4       perspective_mat;
    OE_Mat4x4       view_mat;
    OE_Mat4x4       model_mat;
    
    float           near{0.0f};
    float           far{0.0f};
    
    std::size_t     scene_id{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_MaterialRenderData : public NRE_BaseObject{
    
    std::size_t     scene_id{0};
    
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_VGroupRenderData{
    
    std::size_t     id{0};
    OE_Mat4x4       bone_mat;
    
    std::size_t     ibo{0};
    std::size_t     material_id{0};
    std::size_t     mesh_id{0};
    
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_MeshRenderData : public NRE_BaseObject{
    
    std::size_t     scene_id{0};
    
    OE_Mat4x4       model_mat;
    
    unsigned int    uvmaps{0};
    unsigned int    bones{0};
    
    std::size_t     vbo{0};
    std::size_t     vbo_size{0};
    
    std::size_t     vao{0};
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
    
    bool vao_initialized{false};
    
    std::vector<nre::gpu::vertex_layout_input> vao_input;
    
    float max_x{0.0f}, min_x{0.0f}, max_y{0.0f}, min_y{0.0f}, max_z{0.0f}, min_z{0.0f}, max_radius{0.0f}, min_radius{0.0f};
    
    std::vector<float> genBoundingBoxVBO();
    
    // the mesh is used too fetch vertices only
    std::shared_ptr<OE_Mesh32> mesh{nullptr};
    
    std::set<size_t> vgroups;
};

struct NRE_PointLightRenderData : public NRE_BaseObject{
    OE_Mat4x4       model_mat;
    
    OE_RGBColor                 color;
    float                       intensity{0.0f};
    float                       range{0.0f};
    
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_PointLightDrawCall{
    
    NRE_PointLightDrawCall() = default;
    NRE_PointLightDrawCall(std::size_t, float, std::size_t);
    std::size_t id{0};
    float       z{0.0f};
    std::size_t priority{0};
    
    bool operator > (const NRE_PointLightDrawCall&) const;
};

struct NRE_DirectionalLightRenderData : public NRE_BaseObject{
    OE_Mat4x4       model_mat;
    OE_RGBColor                 color;
    float                       intensity;
    
    
    std::size_t     ubo{0};
    unsigned int    offset{0};
    unsigned int    size{0};
};

struct NRE_RenderGroup;

struct NRE_SceneRenderData : public NRE_BaseObject{
    
    std::set<std::size_t> cameras;
    std::set<std::size_t> meshes;
    std::set<std::size_t> dir_lights;
    std::set<std::size_t> pt_lights;
    std::set<std::size_t> materials;
    
    NRE_DrawCallContainer render_groups;
    
    bool existsRenderGroup(const NRE_RenderGroup&);
    
};

struct NRE_ViewportRenderData : public NRE_BaseObject{
    
    std::vector<std::size_t>    layers;
    std::vector<std::size_t>    cameras;
    
    std::vector<int>            camera_modes; 
    
    std::vector<int>            layer_combine_modes;
        // two floats for each layer, but only useful when there is a split screen in eye coordinates
    std::vector<float>          split_screen_positions;
    
};

struct NRE_RenderGroup{
    
    // data for the z prepass
    nre::gpu::vertex_shader vs_z_prepass;
    std::size_t     z_prepass_program{0};
    bool isZPrePassSetup{false};
    
    // data for normal render
    nre::gpu::vertex_shader vs;
    nre::gpu::pixel_shader fs;    
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
