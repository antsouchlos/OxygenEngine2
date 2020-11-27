#include <Renderer/NRE_RendererMain.h>

using namespace std;

NRE_Renderer::NRE_Renderer(){
    
}

NRE_Renderer::~NRE_Renderer(){
   
}
    
bool NRE_Renderer::init(){
    
    
    // make sure there are no stored objects
    this->cameras.clear();
    this->materials.clear();
    this->vgroups.clear();
    this->meshes.clear();
    this->lights.clear();
    this->render_groups.clear();
    
    // reset all GPU data
    if (api != nullptr){
        api->destroy();
        delete api;
    }
    
    // make sure we use the right API
    this->api = new NRE_GL3_API();
    if (!this->screen->isES){
        NRE_GPU_ShaderBase::init(NRE_GPU_GL, this->screen->major, this->screen->minor);
    } 
    else{
        NRE_GPU_ShaderBase::init(NRE_GPU_GLES, this->screen->major, this->screen->minor);
    }
    
    return true;
}

bool NRE_Renderer::updateSingleThread(){
    
    // upload all remaining data to the GPU 
    this->updateMeshGPUData();
    this->updateMaterialGPUData();
    this->updateCameraGPUData();
    
    // sort draw calls
    auto comp_lambda = [] (const NRE_RenderGroup& r1, const NRE_RenderGroup& r2) -> bool { return r1 < r2; };
    std::sort(this->render_groups.begin(), this->render_groups.end(), comp_lambda);
    
    this->api->use_wireframe = this->use_wireframe.load(std::memory_order_relaxed);
    
    // draw everything required for the z prepass, which also populates the depth buffer
    this->api->setRenderMode(NRE_GPU_Z_PREPASS_BACKFACE);
    for (auto &x: this->render_groups){
        this->drawRenderGroupZPrePass(&x);
    }
    
    // draw everything normally
    this->api->setRenderMode(NRE_GPU_AFTERPREPASS_BACKFACE);
    for (auto &x: this->render_groups){
        this->drawRenderGroup(&x);
    }
    return true;
}

void NRE_Renderer::drawRenderGroup(NRE_RenderGroup *ren_group){
    
    if (!ren_group->isSetup){
       //cout << "Setting up Render group" << ren_group->camera << " " << ren_group->vgroup << " " << ren_group->mesh << endl;
        
        ren_group->isSetup = true;
        
        ren_group->vs = NRE_GPU_VertexShader();
        ren_group->vs.type = NRE_GPU_VS_REGULAR;
        ren_group->vs.num_of_uvs = this->meshes[ren_group->mesh].uvmaps;
        
        
        // choose shading mode
        ren_group->fs = NRE_GPU_PixelShader();
        lockMutex();
        switch (this->shading_mode){
            case OE_RENDERER_NORMALS_SHADING:
                ren_group->fs.type = NRE_GPU_FS_NORMALS;
                break;
            case OE_RENDERER_NO_LIGHTS_SHADING:
                
                break;
            case OE_RENDERER_DIR_LIGHTS_SHADING:
                
                break;
            case OE_RENDERER_INDEXED_LIGHTS_SHADING:
                
                break;
            case OE_RENDERER_REGULAR_SHADING:
                ren_group->fs.type = NRE_GPU_FS_MATERIAL;
                break;
        }
        unlockMutex();
        ren_group->fs.num_of_uvs = this->meshes[ren_group->mesh].uvmaps;
        
        ren_group->program = this->api->newProgram();
        this->api->setProgramVS(ren_group->program, ren_group->vs);
        this->api->setProgramFS(ren_group->program, ren_group->fs);
        
        this->api->setupProgram(ren_group->program);
        this->api->setProgramUniformSlot(ren_group->program, "OE_Camera", 0);
        this->api->setProgramUniformSlot(ren_group->program, "OE_Mesh32", 1);
        this->api->setProgramUniformSlot(ren_group->program, "OE_Material", 2);
        
    }
    
    this->api->setUniformState(this->meshes[ren_group->mesh].ubo, ren_group->program, 1, 0, 0);
    this->api->setUniformState(this->cameras[ren_group->camera].ubo, ren_group->program, 0, 0, 0);
    this->api->setUniformState(this->materials[ren_group->material].ubo, ren_group->program, 2, 0, 0);
    this->api->draw(ren_group->program, this->meshes[ren_group->mesh].vao, this->vgroups[ren_group->vgroup].ibo);
}

void NRE_Renderer::drawRenderGroupZPrePass(NRE_RenderGroup* ren_group){
    
    if (!ren_group->isZPrePassSetup){
        
        ren_group->isZPrePassSetup = true;
        
        ren_group->vs_z_prepass = NRE_GPU_VertexShader();
        ren_group->vs_z_prepass.type = NRE_GPU_VS_Z_PREPASS;
        ren_group->vs_z_prepass.num_of_uvs = this->meshes[ren_group->mesh].uvmaps;
        
        ren_group->z_prepass_program = this->api->newProgram();
        
        this->api->setProgramVS(ren_group->z_prepass_program, ren_group->vs_z_prepass);
        
        this->api->setupProgram(ren_group->z_prepass_program);
        this->api->setProgramUniformSlot(ren_group->z_prepass_program, "OE_Camera", 0);
        this->api->setProgramUniformSlot(ren_group->z_prepass_program, "OE_Mesh32", 1);
    }
    
    this->api->setUniformState(this->meshes[ren_group->mesh].ubo, ren_group->z_prepass_program, 1, 0, 0);
    this->api->setUniformState(this->cameras[ren_group->camera].ubo, ren_group->z_prepass_program, 0, 0, 0);
    this->api->draw(ren_group->z_prepass_program, this->meshes[ren_group->mesh].vao, this->vgroups[ren_group->vgroup].ibo);
    
}

bool NRE_Renderer::updateMultiThread(OE_Task*, int){
    return false;
}

void NRE_Renderer::destroy(){
    if (api != nullptr){
        api->destroy();
        delete api;
    }
}    
