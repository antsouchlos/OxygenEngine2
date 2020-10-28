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
        
        auto t = clock();
        // make sure all index and vertex buffers are generated
        // On loading objects from disk they are not generated here
        // But on objects created on the fly they ARE generated here
        mesh->data.genVertexBufferInternally();
        mesh->data.genIndexBuffersInternally();
        
        // setup the Vertex Buffer
        this->meshes[id] = NRE_MeshRenderData(); this->meshes[id].id = id;
        this->meshes[id].uvmaps = mesh->data.vertices.uvmaps.size();
        this->meshes[id].size = mesh->data.vbo.size();
        
        this->meshes[id].vbo = this->api->newVertexBuffer();
        this->api->setVertexBufferMemory(this->meshes[id].vbo, this->meshes[id].size, NRE_GPU_STATIC);
        this->api->setVertexBufferData(this->meshes[id].vbo, mesh->data.vbo, 0);
        mesh->data.vbo.clear();
        
        // setup Vertex Layout Inputs
        this->meshes[id].vao = this->api->newVertexLayout();
        typedef NRE_GPU_VertexLayoutInput VLI; // for clarity
        vector<VLI> vao_arg;
        
        vao_arg.push_back(VLI(this->meshes[id].vbo, 0, 3, 6+this->meshes[id].uvmaps));
        vao_arg.push_back(VLI(this->meshes[id].vbo, 3, 3, 6+this->meshes[id].uvmaps));
        
        for (size_t i=0; i < this->meshes[id].uvmaps; i++){
            vao_arg.push_back(VLI(this->meshes[id].vbo, 6+2*i, 2, 6+this->meshes[id].uvmaps));
        }
        this->api->setVertexLayoutFormat(this->meshes[id].vao, vao_arg);
        
        //setup the Uniform buffer holding the model matrix
        this->meshes[id].ubo = this->api->newUniformBuffer();
        this->api->setUniformBufferMemory(this->meshes[id].ubo, 16, NRE_GPU_STREAM);
        
        OE_Mat4x4 model_mat(1.0f);
        OE_Quat rot_quat = OE_Quat(mesh->current_state.rot_w, mesh->current_state.rot_x, mesh->current_state.rot_y, mesh->current_state.rot_z);
        OE_Vec3 translation_vec = OE_Vec3(mesh->current_state.pos_x, mesh->current_state.pos_y, mesh->current_state.pos_z);
        model_mat = OE_Quat2Mat4x4(rot_quat) * OE_Translate(model_mat, translation_vec);
        
        this->api->setUniformBufferData(this->meshes[id].ubo, OE_Mat4x4ToSTDVector(model_mat),0);
        
        
        // handle Vertex (Triangle) groups
        for (auto vgroup : mesh->data.triangle_groups){
            this->handleVGroupData(id, vgroup.first, mesh);
        }
        
        cout << "NRE DATA BENCHMARK MESH: " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
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
    if (this->cameras.count(id) == 0){
        
        //setup the Uniform buffer holding the model matrix
        this->cameras[id].ubo = this->api->newUniformBuffer();
        this->api->setUniformBufferMemory(this->cameras[id].ubo, 16, NRE_GPU_STREAM);
        
        OE_Mat4x4 model_mat(1.0f);
        OE_Quat rot_quat = OE_Quat(camera->current_state.rot_w, camera->current_state.rot_x, camera->current_state.rot_y, camera->current_state.rot_z);
        cout << camera->current_state.rot_w << camera->current_state.rot_x << camera->current_state.rot_y << camera->current_state.rot_z << endl;
        OE_Vec3 translation_vec = OE_Vec3(camera->current_state.pos_x, camera->current_state.pos_y, camera->current_state.pos_z);
        model_mat = OE_Quat2Mat4x4(rot_quat) * OE_Translate(model_mat, translation_vec);
        
        this->api->setUniformBufferData(this->cameras[id].ubo, OE_Mat4x4ToSTDVector(model_mat),0);
    }
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
