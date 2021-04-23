#ifndef NRE_RENDERERMAIN_H
#define NRE_RENDERERMAIN_H

#include <Renderer/NRE_RenderData.h>
#include <OE_DummyClasses.h>
#include <Renderer/NRE_GPU_API.h>

class NRE_Renderer : public OE_RendererBase{
public:
    NRE_Renderer();
    ~NRE_Renderer();
    
    bool init();
    bool updateSingleThread();
    bool updateData();
    
    bool updateMultiThread(OE_Task*, int);
    void destroy();

    NRE_GPU_API* api{nullptr};
    
    std::size_t loaded_viewport{0};
    std::map<std::size_t, NRE_CameraRenderData> cameras;
    std::map<std::size_t, NRE_MaterialRenderData> materials;
    std::map<std::size_t, NRE_VGroupRenderData> vgroups;
    std::map<std::size_t, NRE_MeshRenderData> meshes;
    std::map<std::size_t, NRE_DirectionalLightRenderData> dir_lights;
    std::map<std::size_t, NRE_PointLightRenderData> pt_lights;
    std::map<std::size_t, NRE_SceneRenderData> scenes;
    std::map<std::size_t, NRE_ViewportRenderData> viewports;    
    
    std::set<std::size_t> deleted_meshes;
    std::set<std::size_t> deleted_materials;
    std::set<std::size_t> deleted_cameras;
    std::set<std::size_t> deleted_scenes;
    
    
    // bounding box data
    std::size_t prog_bbox{0};
    std::size_t vbo_bbox{0};
    std::size_t vao_bbox{0};
    
    std::size_t prog_sphere{0};
    std::size_t vbo_sphere{0};
    std::size_t vao_sphere{0};
    std::size_t ibo_sphere{0};
    
    bool setup_bbox_prog{false};
    bool setup_sphere_prog{false};
    
protected:
    
    void handleMeshData(std::size_t, std::shared_ptr<OE_Mesh32>);
    void handleMaterialData(std::size_t, std::shared_ptr<OE_Material>);
    void handleCameraData(std::size_t, std::shared_ptr<OE_Camera>);
    void handleLightData(std::size_t, std::shared_ptr<OE_Light>);
    void handleVGroupData(std::size_t, std::size_t, std::shared_ptr<OE_Mesh32>);
    
    void handleSceneData(std::size_t, std::shared_ptr<OE_Scene>);
    void handleViewportData(std::size_t,std::shared_ptr<OE_ViewportConfig>);
    
    
    void updateMeshGPUData();
    void updateMaterialGPUData();
    void updateCameraGPUData();
    
    void deleteCamera(std::size_t);
    void deleteMaterial(std::size_t);
    void deleteMesh(std::size_t);
    
    void generateDrawCalls();
    
    void drawRenderGroup(NRE_RenderGroup&);
    void drawRenderGroupZPrePass(NRE_RenderGroup&);
    void drawRenderGroupBoundingBox(NRE_RenderGroup&);
    void drawRenderGroupBoundingSphere(NRE_RenderGroup&);
    
    void setupBoundingBoxProgram();
    void setupBoundingSphereProgram();
};

#endif
