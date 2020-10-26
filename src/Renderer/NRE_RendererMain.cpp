#include <Renderer/NRE_RendererMain.h>

using namespace std;

NRE_Renderer::NRE_Renderer(){
    
}

NRE_Renderer::~NRE_Renderer(){
   
}
    
bool NRE_Renderer::init(){
    if (api != nullptr)
        delete api;
    this->api = new NRE_GL3_API();
    return true;
}

bool NRE_Renderer::updateSingleThread(){
    
    return true;
}

bool NRE_Renderer::updateData(){
    assert (this->world != nullptr);
    
    // add/change any missing world data
    
    for (auto scene: this->world->scenes){
        
        for (auto material : scene.second->materials){
            this->handleMaterialData(material.first, material.second);
        }
        
        // first handle objects and lights
        for (auto obj : scene.second->objects){
            if (obj.second->getType() == "MESH32"){
                this->handleMeshData(obj.first, static_cast<OE_Mesh32*>(obj.second));
            }
            else if (obj.second->getType() == "LIGHT"){
                this->handleLightData(obj.first, static_cast<OE_Light*>(obj.second));
            }
            else{}
        }
        // THEN handle cameras
        for (auto obj : scene.second->objects){
            if (obj.second->getType() == "CAMERA"){
                this->handleCameraData(obj.first, static_cast<OE_Camera*>(obj.second));
            }
            else{}
        }
    }
    
    // remove any obsolete world data
    
    for (auto obj : this->meshes){
        if (OE_Object::id2name.count(obj.first) == 0){
            this->api->deleteVertexBuffer(this->meshes[obj.first].vbo);
            this->meshes.erase(obj.first);
        }
    }
    
    return true;
}

void NRE_Renderer::handleMeshData(std::size_t id, OE_Mesh32* mesh){
    if (this->meshes.count(id) == 0){
        mesh->data.genVertexBufferInternally();
        mesh->data.genIndexBuffersInternally();
        
        this->meshes[id] = NRE_MeshRenderData(); this->meshes[id].id = id;
        this->meshes[id].uvmaps = mesh->data.vertices.uvmaps.size();
        this->meshes[id].size = mesh->data.vbo.size();
        
        this->meshes[id].vbo = this->api->newVertexBuffer();
        this->api->setVertexBufferMemory(this->meshes[id].vbo, this->meshes[id].size, NRE_GPU_STATIC);
        this->api->setVertexBufferData(this->meshes[id].vbo, mesh->data.vbo, 0);
        mesh->data.vbo.clear();
        
        for (auto vgroup : mesh->data.triangle_groups){
            this->handleVGroupData(id, vgroup.first, mesh);
        }
    }
}

void NRE_Renderer::handleVGroupData(std::size_t mesh_id, std::size_t id, OE_Mesh32* mesh){
    if (this->vgroups.count(id) == 0){
        
        this->vgroups[id] = NRE_VGroupRenderData(); this->vgroups[id].id = id;
        this->vgroups[id].bone_mat = OE_Mat4x4(1.0f);
        this->vgroups[id].ibo = this->api->newIndexBuffer();
        this->api->setIndexBufferMemory(this->vgroups[id].ibo, mesh->data.ibos[id].data.size(), NRE_GPU_STATIC);
        this->api->setIndexBufferData(this->vgroups[id].ibo, mesh->data.ibos[id].data, 0);
        mesh->data.ibos[id].data.clear();
        
    }
}

void NRE_Renderer::handleMaterialData(std::size_t id, OE_Material* mat){
    
}

void NRE_Renderer::handleCameraData(std::size_t id, OE_Camera* camera){
    
}

void NRE_Renderer::handleLightData(std::size_t id, OE_Light* light){
    
}

bool NRE_Renderer::updateMultiThread(OE_Task*, int){
    // UNUSED on OpenGL
    return false;
}

void NRE_Renderer::destroy(){
    if (api != nullptr)
        delete api;
}    
