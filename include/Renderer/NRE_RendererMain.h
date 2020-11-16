#ifndef NRE_RENDERERMAIN_H
#define NRE_RENDERERMAIN_H

#include <Renderer/NRE_RenderData.h>
#include <OE_DummyClasses.h>
#include <Renderer/NRE_GL3_API.h>

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
    
    std::map<std::size_t, NRE_CameraRenderData> cameras;
    std::map<std::size_t, NRE_MaterialRenderData> materials;
    std::map<std::size_t, NRE_VGroupRenderData> vgroups;
    std::map<std::size_t, NRE_MeshRenderData> meshes;
    std::map<std::size_t, NRE_LightRenderData> lights;
    std::vector<NRE_RenderGroup> render_groups;

protected:
    
    void handleMeshData(std::size_t, OE_Mesh32*);
    void handleMaterialData(std::size_t, OE_Material*);
    void handleCameraData(std::size_t, OE_Camera*);
    void handleLightData(std::size_t, OE_Light*);
    void handleVGroupData(std::size_t, std::size_t, OE_Mesh32*);
    
    
    void updateMeshGPUData();
    void updateMaterialGPUData();
    void updateCameraGPUData();
    
    
    void drawRenderGroup(NRE_RenderGroup*);
    void drawRenderGroupZPrePass(NRE_RenderGroup*);
    
    bool existsRenderGroup(NRE_RenderGroup);
};



#endif
