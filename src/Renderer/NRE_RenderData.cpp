#include <OE_Math.h>
#include <Renderer/NRE_RendererMain.h>
#include <OE_API.h>

using namespace std;
using namespace oe;

std::vector<float> NRE_MeshRenderData::genBoundingBoxVBO(){
    return OE_GetBoundingBoxVertexBuffer(max_x, min_x, max_y, min_y, max_z, min_z);
}


bool NRE_RenderGroup::operator < (const NRE_RenderGroup& other) const{
    
    // sort by camera
    if (this->camera < other.camera){
        return true;
    }
    else if (this->camera > other.camera){
        return false;
    }
    else{
        // sort by material (program) since pipeline/texture changes
        // are expensive
        if (this->material < other.material){
            return true;
        }
        else if (this->material > other.material){
            return false;
        }
        else{
            // sort by mesh, to minimize changing VAO
            // and Mesh Buffers
            if (this->mesh < other.mesh){
                return true;
            }
            else if (this->mesh > other.mesh){
                return false;
            }
            else{
                // finally sort by vgroup since changing Index Buffers should be cheap
                if (this->vgroup < other.vgroup){
                    return true;
                }
                else if (this->vgroup >= other.vgroup){
                    return false;
                }
            }
        }
    }
    return false;
}

bool NRE_SceneRenderData::existsRenderGroup(const NRE_RenderGroup& ren_group){
    return this->render_groups.contains(ren_group);
}

NRE_PointLightDrawCall::NRE_PointLightDrawCall(std::size_t id_in, float z_in, std::size_t priority_in){
    this->id = id_in;
    this->z = z_in;
    this->priority = priority_in;
}

bool NRE_PointLightDrawCall::operator > (const NRE_PointLightDrawCall& other) const{
    if (this->priority == other.priority){
        if (std::abs(this->z - other.z) < 0.0000001f){
            return this->id > other.id;
        }
        else{
            return this->z > other.z;
        }
    }
    else{
        return this->priority > other.priority;
    }
    return false;
}


bool NRE_Renderer::updateData(){
    assert (this->world != nullptr);
    
    // add/change any missing world data
    //OE_Main->lockMutex();
    
    vector<size_t> camera_ids;
    
    // this regenerates the renderer database
    
    if (this->screen->restart_renderer){
        
        //cout << OE_Main->world->to_str() << endl;
        
        // this also resets the GPU API resources
        this->destroy();
        this->init();
        
        for (auto mat : OE_World::materialsList){
            mat.flag_as_changed();
        }
        
        for (auto obj : OE_World::objectsList){
            if (obj.p_->getType() == OE_OBJECT_MESH){
                auto mesh = static_pointer_cast<OE_Mesh32>(obj.p_);
                mesh->data->vbo_exists = false;
                mesh->data->ibos_exist = false;
            }
            obj.flag_as_changed();
        }
        
        for (auto sce : OE_World::scenesList){
            sce.flag_as_changed();
        }
        
        for (auto vpc : OE_World::viewportsList){
            vpc.flag_as_changed();
        }
        
        this->screen->restart_renderer = false;
    }
    
    // Update element data
    
    for (auto mat : OE_World::materialsList.changed()){
        this->handleMaterialData(mat.id_, mat.p_);
    }
    
    this->has_dir_lights_changed = false;
    this->has_pt_lights_changed = false;
    
    for (auto obj : OE_World::objectsList.changed()){
        if (obj.p_->getType() == OE_OBJECT_MESH){
           this->handleMeshData(obj.id_, static_pointer_cast<OE_Mesh32>(obj.p_));
        }
        else if (obj.p_->getType() == OE_OBJECT_LIGHT){
            this->handleLightData(obj.id_, static_pointer_cast<OE_Light>(obj.p_));
        }
    }
    
    for (auto obj : OE_World::objectsList.changed()){
        if (obj.p_->getType() == OE_OBJECT_CAMERA){
           this->handleCameraData(obj.id_, static_pointer_cast<OE_Camera>(obj.p_));
           camera_ids.push_back(obj.id_);
        }
    }
    
    for (auto sce : OE_World::scenesList.changed()){
        this->handleSceneData(sce.id_, sce.p_);
    }
    
    for (auto vpc : OE_World::viewportsList.changed()){
        this->handleViewportData(vpc.id_, vpc.p_);
        this->loaded_viewport = OE_Main->world->loaded_viewport;
    }
    
    // handle deleted elements (should look if the element exists first)
    
    for (auto mat : OE_World::materialsList.deleted()){
        if (this->materials.count(mat.id_) == 1){
            this->scenes[this->materials[mat.id_].scene_id].render_groups.removeMaterial(mat.id_);
            this->deleted_materials.insert(mat.id_);
        }
        
    }

    for (auto obj : OE_World::objectsList.deleted()){
        
        if (this->meshes.count(obj.id_) == 1){
            this->scenes[this->meshes[obj.id_].scene_id].render_groups.removeMesh(obj.id_);
            this->deleted_meshes.insert(obj.id_);
        }
        else if (this->dir_lights.count(obj.id_) == 1){
            this->dir_lights.erase(obj.id_);
            this->has_dir_lights_changed = true;
        }
        else if (this->pt_lights.count(obj.id_) == 1){
            this->pt_lights.erase(obj.id_);
            this->has_pt_lights_changed = true;
        }
        else if (this->cameras.count(obj.id_) == 1){
            this->scenes[this->cameras[obj.id_].scene_id].render_groups.removeCamera(obj.id_);
            this->deleted_cameras.insert(obj.id_);
        }
        else {
            
        }
    }

    
    for (auto sce : OE_World::scenesList.deleted()){
        if (this->scenes.count(sce.id_) == 1){
            this->deleted_scenes.insert(sce.id_);
        }
    }
    
    for (auto vpc : OE_World::viewportsList.deleted()){
        if (this->viewports.count(vpc.id_) == 1){
            this->viewports.erase(vpc.id_);
            if (this->loaded_viewport == vpc.id_){
                this->loaded_viewport = 0; // needed for compatibility with older .csl with no viewportconfigs
            }
        }
    }
    
    //OE_Main->unlockMutex();
    return true;
}

//-----------------------------------Pass individual element changes synchronously as fast as possible--------//

void NRE_Renderer::handleMeshData(std::size_t id, std::shared_ptr<OE_Mesh32> mesh){
    if (this->meshes.count(id) == 0){
        
        // make sure all index and vertex buffers are generated
        // On loading objects from disk they are not generated here
        // But on objects created on the fly they ARE generated here
        mesh->data->genVertexBufferInternally();
        mesh->data->genIndexBuffersInternally();
        
        // setup the Vertex Buffer
        this->meshes[id] = NRE_MeshRenderData(); this->meshes[id].id = id;
        this->meshes[id].uvmaps = mesh->data->vertices.uvmaps.size();
        this->meshes[id].size = mesh->data->vbo.size();
        
        // store the shared pointer
        this->meshes[id].mesh = mesh;        
        
        //setup the Uniform buffer holding the model matrix
        // but offload the actual OpenGL commands for later, since this runs in a performance
        // critical section
        
        this->meshes[id].model_mat = mesh->GetModelMatrix();
        
        //this->meshes[id].data = OE_Mat4x4ToSTDVector(model_mat);
        this->meshes[id].changed = true;
        
        // handle Vertex (Triangle) groups
        for (auto vgroup : mesh->data->triangle_groups){
            this->handleVGroupData(id, vgroup.first, mesh);
            this->meshes[id].vgroups.insert(vgroup.first);
        }
        
        // store bounding box
        
        //---- <TEMPORARY> ----//
        // This should not be done here but in the physics engine or interpreter
        mesh->calculateProperBoundingBox();
        //---- </TEMPORARY> ---//
        
        assert (mesh->data->vertices.calculatedBoundingBox);
        
        this->meshes[id].max_x = mesh->data->vertices.max_x;
        this->meshes[id].max_y = mesh->data->vertices.max_y;
        this->meshes[id].max_z = mesh->data->vertices.max_z;
        
        this->meshes[id].min_x = mesh->data->vertices.min_x;
        this->meshes[id].min_y = mesh->data->vertices.min_y;
        this->meshes[id].min_z = mesh->data->vertices.min_z;
        
        this->meshes[id].max_radius = mesh->data->vertices.max_radius;
        this->meshes[id].min_radius = mesh->data->vertices.min_radius;
    }
    else{
        this->meshes[id].model_mat = mesh->GetModelMatrix();
        
        bool render_bboxes = this->render_bounding_boxes.load(std::memory_order_relaxed);
        bool render_spheres = this->render_bounding_spheres.load(std::memory_order_relaxed);
        
        if (render_bboxes or render_spheres){
            //---- <TEMPORARY> ----//
            // This should not be done here but in the physics engine
            mesh->calculateProperBoundingBox();
            //---- </TEMPORARY> ---//
            
            assert (mesh->data->vertices.calculatedBoundingBox);
        
            this->meshes[id].max_x = mesh->data->vertices.max_x;
            this->meshes[id].max_y = mesh->data->vertices.max_y;
            this->meshes[id].max_z = mesh->data->vertices.max_z;
        
            this->meshes[id].min_x = mesh->data->vertices.min_x;
            this->meshes[id].min_y = mesh->data->vertices.min_y;
            this->meshes[id].min_z = mesh->data->vertices.min_z;
            
            this->meshes[id].max_radius = mesh->data->vertices.max_radius;
            this->meshes[id].min_radius = mesh->data->vertices.min_radius;
            
        }
        
        this->meshes[id].changed = true;
    }
}

void NRE_Renderer::handleVGroupData(std::size_t mesh_id, std::size_t id, std::shared_ptr<OE_Mesh32> mesh){
    if (this->vgroups.count(id) == 0){
        
        this->vgroups[id] = NRE_VGroupRenderData(); this->vgroups[id].id = id; this->vgroups[id].mesh_id = mesh_id;
        this->vgroups[id].bone_mat = OE_Mat4x4(1.0f);
        this->vgroups[id].ibo = this->api->newIndexBuffer();
        this->vgroups[id].material_id = mesh->data->triangle_groups[id]->material_id;        
    }
}

void NRE_Renderer::handleMaterialData(std::size_t id, std::shared_ptr<OE_Material> mat){
    if (this->materials.count(id) == 0){
        this->materials[id] = NRE_MaterialRenderData(); this->materials[id].id = id;
    }
    
    this->materials[id].data = mat->GetRendererData();
    this->materials[id].changed = true;
}

void NRE_Renderer::handleCameraData(std::size_t id, std::shared_ptr<OE_Camera> camera){
    if (this->cameras.count(id) == 0){
        
        //setup the Uniform buffer holding the perspective/view matrix
        // but offload the actual OpenGL commands for later, since this runs in a performance
        // critical section
        this->cameras[id] = NRE_CameraRenderData(); this->cameras[id].id = id;
    }
    
    this->cameras[id].near = (float)camera->near+0.3f;
    this->cameras[id].far = (float)camera->far + 50.0f;
    
    auto view_mat = camera->GetViewMatrix();
    auto perspective_mat = OE_Perspective(camera->fov, camera->aspect_ratio, this->cameras[id].near, this->cameras[id].far);
    
    this->cameras[id].view_mat = view_mat;
    this->cameras[id].perspective_mat = perspective_mat;
    this->cameras[id].model_mat = camera->GetModelMatrix();
    this->cameras[id].perspective_view_mat = perspective_mat*view_mat;
    
    this->cameras[id].data = OE_Mat4x4ToSTDVector(this->cameras[id].perspective_view_mat);

    this->cameras[id].data.push_back(this->cameras[id].model_mat[3].x);
    this->cameras[id].data.push_back(this->cameras[id].model_mat[3].y);
    this->cameras[id].data.push_back(this->cameras[id].model_mat[3].z);
    this->cameras[id].data.push_back(1.0f);
    this->cameras[id].changed = true;
}

void NRE_Renderer::handleLightData(std::size_t id, std::shared_ptr<OE_Light> light){
    if (light->light_type == 1){
        //POINT LIGHT
        if (this->pt_lights.count(id) == 0){
            this->pt_lights[id] = NRE_PointLightRenderData(); this->pt_lights[id].id = id;
            this->has_pt_lights_changed = true;
        }
        
        this->pt_lights[id].model_mat = light->GetModelMatrix();
        this->pt_lights[id].color     = light->color;
        this->pt_lights[id].intensity = light->intensity;
        this->pt_lights[id].range     = light->range/20.0f;
            
        //this->pt_lights[id].data = light->GetLightGPUData();
        this->pt_lights[id].data = OE_Mat4x4ToSTDVector(this->pt_lights[id].model_mat);
        /*this->pt_lights[id].data.push_back(1.0f);
        this->pt_lights[id].data.push_back(1.0f);
        this->pt_lights[id].data.push_back(1.0f);         
        this->pt_lights[id].data.push_back(light->range/20.0f);
        this->pt_lights[id].data.push_back(1.0f);
        this->pt_lights[id].data.push_back(1.0f);
        this->pt_lights[id].data.push_back(1.0f);       
        this->pt_lights[id].data.push_back(light->range/20.0f);*/
        this->pt_lights[id].data[15] = light->range/20.0f;
            
        this->pt_lights[id].changed = true;
    }
    else if (light->light_type == 2){
        //DIRECTIONAL LIGHT
    }
    else if (light->light_type == 3){
        // SPOTLIGHT
    }
    else{
        //NOTHING
    }
}

void NRE_Renderer::handleSceneData(std::size_t id, std::shared_ptr<OE_Scene> scene){
    if (this->scenes.count(id) == 0){
        this->scenes[id] = NRE_SceneRenderData(); this->scenes[id].id = id;
    }
    else{
        this->scenes[id].cameras.clear();
        this->scenes[id].dir_lights.clear();
        this->scenes[id].pt_lights.clear();
        this->scenes[id].meshes.clear();
    }
    
    // group objects
    for (auto x: scene->objects){
        if (this->cameras.count(x) != 0){
            this->scenes[id].cameras.insert(x);
        }
        else if (this->meshes.count(x) != 0){
            this->scenes[id].meshes.insert(x);
        }
        else if (this->dir_lights.count(x) != 0){
            this->scenes[id].dir_lights.insert(x);
        }
        else if (this->pt_lights.count(x) != 0){
            this->scenes[id].pt_lights.insert(x);
        }
    }
        
    // store the scene each camera is in
    for (auto cam : this->scenes[id].cameras){
        this->cameras[cam].scene_id = id;
        this->cameras[cam].changed = true;
    }
    // store the scene each mesh is in
    for (auto mat : this->scenes[id].materials){
        this->materials[mat].scene_id = id;
        this->materials[mat].changed = true;
    }
    // store the scene each material is in
    for (auto mesh : this->scenes[id].meshes){
        this->meshes[mesh].scene_id = id;
        this->meshes[mesh].changed = true;
    }
    
    this->scenes[id].materials = scene->materials;
    this->scenes[id].changed = true;
}

void NRE_Renderer::handleViewportData(std::size_t id, std::shared_ptr<OE_ViewportConfig> vp_config){
    if (this->viewports.count(id) == 0){
        this->viewports[id] = NRE_ViewportRenderData(); this->viewports[id].id = id;
        this->viewports[id].has_init = true;
    }
    
    this->viewports[id].layers = vp_config->layers;
    this->viewports[id].cameras = vp_config->cameras;
    this->viewports[id].camera_modes = vp_config->camera_modes;
    this->viewports[id].layer_combine_modes = vp_config->layer_combine_modes;
    this->viewports[id].split_screen_positions = vp_config->split_screen_positions;
    this->viewports[id].changed = true;
}

//---------------------------------Update actual GPU data------------------------------//

void NRE_Renderer::updateMeshGPUData(){
    for (auto mesh: this->meshes){
        
        // first time buffer initialization
        if (!this->meshes[mesh.first].has_init){
            this->meshes[mesh.first].vbo      = this->api->newVertexBuffer();
            this->meshes[mesh.first].vao      = this->api->newVertexLayout();
            this->meshes[mesh.first].ubo      = this->api->newUniformBuffer();
            this->meshes[mesh.first].has_init = true;
        }
        
        
        // initialize data itself
        if (!this->meshes[mesh.first].vao_initialized){
            
            /// vertex buffer
            this->meshes[mesh.first].mesh->data->vbo_mutex.lockMutex();
            this->api->setVertexBufferMemoryData(this->meshes[mesh.first].vbo, this->meshes[mesh.first].mesh->data->vbo, NRE_GPU_STATIC);
            this->meshes[mesh.first].mesh->data->vbo.clear();
            this->meshes[mesh.first].mesh->data->vbo_mutex.unlockMutex();
            
            
            /// index buffers
            this->meshes[mesh.first].mesh->data->ibos_mutex.lockMutex();
            for (auto vg : mesh.second.vgroups){
                this->api->setIndexBufferMemoryData(this->vgroups[vg].ibo, this->meshes[mesh.first].mesh->data->ibos[vg].data, NRE_GPU_STATIC);
                this->meshes[mesh.first].mesh->data->ibos[vg].data.clear();
            }
            
            this->meshes[mesh.first].mesh->data->ibos_mutex.unlockMutex();
            
            /// vertex layout
            typedef NRE_GPU_VertexLayoutInput VLI; // for clarity
            
            this->meshes[mesh.first].mesh->data->ibos.clear();
            //delete this->meshes[mesh.first].mesh->data->index_buffer;
            //this->meshes[mesh.first].mesh->data->index_buffer = nullptr;
            
            this->meshes[mesh.first].vao_input.clear();
        
            this->meshes[mesh.first].vao_input.push_back(VLI(this->meshes[mesh.first].vbo, 0, 3, 6+this->meshes[mesh.first].uvmaps*2));
            this->meshes[mesh.first].vao_input.push_back(VLI(this->meshes[mesh.first].vbo, 3, 3, 6+this->meshes[mesh.first].uvmaps*2));
        
            for (size_t i=0; i < this->meshes[mesh.first].uvmaps; i++){
                this->meshes[mesh.first].vao_input.push_back(VLI(this->meshes[mesh.first].vbo, 6+2*i, 2, 6+this->meshes[mesh.first].uvmaps*2));
            }
            
            this->api->setVertexLayoutFormat(this->meshes[mesh.first].vao, this->meshes[mesh.first].vao_input);
            this->meshes[mesh.first].vao_initialized = true;

        }
        
        // update per frame data
        if (this->meshes[mesh.first].changed){
            
            
            this->meshes[mesh.first].data = OE_Mat4x4ToSTDVector(this->meshes[mesh.first].model_mat);
            
            // populate scaling_max_data
            this->meshes[mesh.first].data.push_back(this->meshes[mesh.first].max_x);
            this->meshes[mesh.first].data.push_back(this->meshes[mesh.first].max_y);
            this->meshes[mesh.first].data.push_back(this->meshes[mesh.first].max_z);
            
            // final element is sphere radius candidate            
            this->meshes[mesh.first].data.push_back(this->meshes[mesh.first].max_radius);
            
            // populate scaling_min_data
            this->meshes[mesh.first].data.push_back(this->meshes[mesh.first].min_x);
            this->meshes[mesh.first].data.push_back(this->meshes[mesh.first].min_y);
            this->meshes[mesh.first].data.push_back(this->meshes[mesh.first].min_z);
            
            // final element is sphere radius candidate            
            this->meshes[mesh.first].data.push_back(this->meshes[mesh.first].min_radius);
            
            
            if (this->meshes[mesh.first].size != this->meshes[mesh.first].data.size()){
                this->meshes[mesh.first].size = this->meshes[mesh.first].data.size();
                this->api->setUniformBufferMemory(this->meshes[mesh.first].ubo, this->meshes[mesh.first].size, NRE_GPU_DYNAMIC);
            }
            
            this->api->setUniformBufferData(this->meshes[mesh.first].ubo, this->meshes[mesh.first].data, 0);
            
            this->meshes[mesh.first].changed = false;
        }
    }
    
    for (auto cam: this->deleted_meshes){
        this->deleteMesh(cam);
    }
    this->deleted_meshes.clear();    
}

void NRE_Renderer::updateMaterialGPUData(){
    for (auto mat: this->materials){
        
        if (!this->materials[mat.first].has_init){
            this->materials[mat.first].ubo = this->api->newUniformBuffer();
            this->materials[mat.first].has_init = true;
        }
        
        if (this->materials[mat.first].changed){
            
            if (this->materials[mat.first].size != this->materials[mat.first].data.size()){
                this->materials[mat.first].size = this->materials[mat.first].data.size();
                this->api->setUniformBufferMemory(this->materials[mat.first].ubo, this->materials[mat.first].size, NRE_GPU_DYNAMIC);
            }
            
            this->api->setUniformBufferData(this->materials[mat.first].ubo, this->materials[mat.first].data, 0);
            this->materials[mat.first].changed = false;
        }
    }
    
    for (auto cam: this->deleted_materials){
        this->deleteMaterial(cam);
    }
    this->deleted_materials.clear();
}

void NRE_Renderer::updateCameraGPUData(){
    for (auto cam: this->cameras){
        
        if (!this->cameras[cam.first].has_init){
            this->cameras[cam.first].ubo = this->api->newUniformBuffer();
            this->cameras[cam.first].has_init = true;
        }
        if (this->cameras[cam.first].changed){
            
            if (this->cameras[cam.first].size != this->cameras[cam.first].data.size()){
                this->cameras[cam.first].size = this->cameras[cam.first].data.size();
                this->api->setUniformBufferMemory(this->cameras[cam.first].ubo, this->cameras[cam.first].size, NRE_GPU_DYNAMIC);
            }
            
            this->api->setUniformBufferData(this->cameras[cam.first].ubo, this->cameras[cam.first].data, 0);
            this->cameras[cam.first].changed = false;
        }
    }
    
    for (auto cam: this->deleted_cameras){
        this->deleteCamera(cam);
    }
    this->deleted_cameras.clear();
}
void NRE_Renderer::updateLightGPUData(){
    /*for (auto l: this->pt_lights){
        
        if (!this->pt_lights[l.first].has_init){
            this->pt_lights[l.first].ubo = this->api->newUniformBuffer();
            this->pt_lights[l.first].has_init = true;
        }
        if (this->pt_lights[l.first].changed){
            
            if (this->pt_lights[l.first].size != this->pt_lights[l.first].data.size()){
                this->pt_lights[l.first].size = this->pt_lights[l.first].data.size();
                this->api->setUniformBufferMemory(this->pt_lights[l.first].ubo, this->pt_lights[l.first].size, NRE_GPU_DYNAMIC);
            }
            
            this->api->setUniformBufferData(this->pt_lights[l.first].ubo, this->pt_lights[l.first].data, 0);
            this->pt_lights[l.first].changed = false;
        }
    }*/
    std::vector<float> pt_light_data;
    pt_light_data.reserve(16*255);
    int count = 0;
    for (auto l: this->pt_visible_lights){
        pt_light_data.insert(pt_light_data.end(), this->pt_lights[l.id].data.begin(), this->pt_lights[l.id].data.end());
        
        count++;
        if (count > 255)
            break;
    }
    
    // fill the rest with zeros since webgl complains otherwise
    for (int i=0; i< 16*(255-count); i++){
        pt_light_data.push_back(0.0f);
    }
    this->api->setUniformBufferMemory(this->pt_light_ubo, pt_light_data.size(), NRE_GPU_STREAM);
    this->api->setUniformBufferData(this->pt_light_ubo, pt_light_data, 0);
}

// deletes

void NRE_Renderer::deleteCamera(std::size_t id){
    
    if (this->cameras[id].ubo != 0){
        this->api->deleteUniformBuffer(this->cameras[id].ubo);
    }
    
    this->cameras.erase(id);
}

void NRE_Renderer::deleteMaterial(std::size_t id){
    
    if (this->materials[id].ubo != 0){
        this->api->deleteUniformBuffer(this->materials[id].ubo);
    }
    
    this->materials.erase(id);
}

void NRE_Renderer::deleteMesh(std::size_t id){
    
    // delete all buffers
    if (this->meshes[id].vbo !=0){
        this->api->deleteVertexBuffer(this->meshes[id].vbo);
    }
    if (this->meshes[id].vao !=0){
        this->api->deleteVertexLayout(this->meshes[id].vao);
    }
    if (this->meshes[id].ubo !=0){
        this->api->deleteUniformBuffer(this->meshes[id].ubo);
    }
    
    // delete buffers of vertex groups
    for (auto vgroup : this->meshes[id].vgroups){
        if (this->vgroups[vgroup].ibo != 0){
            this->api->deleteIndexBuffer(this->vgroups[vgroup].ibo);
        }
        this->vgroups.erase(vgroup);
    }    
    this->meshes.erase(id);
}





