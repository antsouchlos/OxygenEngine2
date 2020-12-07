#include <Renderer/NRE_RendererMain.h>
#include <OE_API.h>

using namespace std;
using namespace OE;

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

bool NRE_Renderer::existsRenderGroup(NRE_RenderGroup ren_group){
    for (auto x: this->render_groups){
        if ((x.camera == ren_group.camera) && (x.material == ren_group.material) && (x.vgroup == ren_group.vgroup) && (x.mesh == ren_group.mesh)){
            return true;
        }
    }
    return false;
}


bool NRE_Renderer::updateData(){
    assert (this->world != nullptr);
    
    // add/change any missing world data
    OE_Main->lockMutex();
    auto temp_scenes = OE_World::scenesList.copy();
    auto temp_objects = OE_World::objectsList.copy();
    auto temp_materials = OE_World::materialsList.copy();
    
    // DONT copy these clearDeleted() and reset() functions to the physics engine, because it will wreak havoc
    // Copy only those above (of course inside a OE_Main mutex)
    // The renderer is supposed to handle those deletions
    OE_World::scenesList.reset();
    OE_World::objectsList.reset();
    OE_World::materialsList.reset();
    
    OE_World::scenesList.clearDeleted();
    OE_World::objectsList.clearDeleted();
    OE_World::materialsList.clearDeleted();
    
    OE_Main->unlockMutex();
    
    if (this->screen->restart_renderer){
        
        // this also resets the GPU API resources
        this->init();
        
        
        // all objects must be gone through one time
        // regardless if they have been changed or not
        // in order to initialize GPU resources again
        for (auto x: temp_scenes.getKeys()){
            temp_scenes.changed[x] = true;
        }
        
        for (auto x: temp_materials.getKeys()){
            temp_materials.changed[x] = true;
        }
        
        for (auto x: temp_objects.getKeys()){
            temp_objects.changed[x] = true;
            if (temp_objects[x]->getType() == "MESH32"){
                
                // vbos/ibos must be regenerated
                auto mesh = static_cast<OE_Mesh32*>(temp_objects[x].get());
                mesh->data.vbo_exists = false;
                mesh->data.ibos_exist = false;
            }
        }
        
        
        this->screen->restart_renderer = false;
        this->setup_bbox_prog = false;
    }
    
    
    
    // remove any obsolete draw commands
    std::set<std::size_t, std::greater<std::size_t>> to_be_removed;
    for (size_t i=0; i<this->render_groups.size(); i++){
        if (!temp_materials.existsID(this->render_groups[i].material)){
            to_be_removed.insert(i);
        }
        if (!temp_objects.existsID(this->render_groups[i].camera)){
            to_be_removed.insert(i);
        }
        if (!temp_objects.existsID(this->render_groups[i].mesh)){
            to_be_removed.insert(i);
        }
    }
    for (auto x: to_be_removed){
        this->render_groups.erase(render_groups.begin() + x);
    }
    //TODO: remove any obsolete objects themselves
    //for (auto x : std::exchange(temp_objects.deleted, {})){
    //    
    //}    
    
    // PRELIMINARY WORK
    // only handle the first scene for now with 1 camera and -materials
    bool scene_done = false;
    for (auto scene: temp_scenes.getKeys()){
        
        vector<size_t> obj_ids;
        
        vector<size_t> camera_ids;
        
        for (auto material : temp_scenes(scene)->materials){
            if (temp_materials.hasChanged(material))
                this->handleMaterialData(material, temp_materials(material).get());
        }
        
        // first handle objects and lights
        for (auto obj : temp_scenes(scene)->objects){
            if (temp_objects(obj)->getType() == "MESH32"){
                if (temp_objects.hasChanged(obj)){
                    this->handleMeshData(obj, static_cast<OE_Mesh32*>(temp_objects(obj).get()));
                    obj_ids.push_back(obj);
                }
            }
            else if (temp_objects(obj)->getType() == "LIGHT"){
                if (temp_objects.hasChanged(obj))
                    this->handleLightData(obj, static_cast<OE_Light*>(temp_objects(obj).get()));
            }
            else{}
        }
        // THEN handle cameras
        for (auto obj : temp_scenes(scene)->objects){
            if (temp_objects(obj)->getType() == "CAMERA"){
                if(temp_objects.hasChanged(obj)){
                    this->handleCameraData(obj, static_cast<OE_Camera*>(temp_objects(obj).get()));
                    camera_ids.push_back(obj);
                }
            }
            else{}
        }
        
        if (scene_done) continue;
        
        size_t camera_id = 0;
        if(camera_ids.size() >= 1){
            camera_id = camera_ids[0];
        } else{
            continue;
        }
        
        for (auto mesh : this->meshes){
            for (auto vgroup : this->vgroups){
                if (vgroup.second.mesh_id == mesh.first){
                    auto render_data = NRE_RenderGroup();
                    render_data.camera = camera_id;
                    render_data.vgroup = vgroup.first;
                    render_data.mesh = mesh.first;
                    render_data.material = vgroup.second.material_id;
                    if (!this->existsRenderGroup(render_data)){
                        this->render_groups.push_back(render_data);
                    }
                }
            }
        }
        scene_done = true;
        
    }
    
    return true;
}

//-----------------------------------Pass individual element changes synchronously as fast as possible--------//

void NRE_Renderer::handleMeshData(std::size_t id, OE_Mesh32* mesh){
    if (this->meshes.count(id) == 0){
        
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
        //this->api->setVertexBufferMemory(this->meshes[id].vbo, this->meshes[id].size, NRE_GPU_STATIC);
        //this->api->setVertexBufferData(this->meshes[id].vbo, mesh->data.vbo, 0);
        this->api->setVertexBufferMemoryData(this->meshes[id].vbo, mesh->data.vbo, NRE_GPU_STATIC);
        mesh->data.vbo.clear();
        
        // setup Vertex Layout Inputs
        // but offload the actual OpenGL commands for later, since this runs in a performance
        // critical section
        this->meshes[id].vao = this->api->newVertexLayout();
        typedef NRE_GPU_VertexLayoutInput VLI; // for clarity
        
        this->meshes[id].vao_input.clear();
        
        this->meshes[id].vao_input.push_back(VLI(this->meshes[id].vbo, 0, 3, 6+this->meshes[id].uvmaps*2));
        this->meshes[id].vao_input.push_back(VLI(this->meshes[id].vbo, 3, 3, 6+this->meshes[id].uvmaps*2));
        
        for (size_t i=0; i < this->meshes[id].uvmaps; i++){
            this->meshes[id].vao_input.push_back(VLI(this->meshes[id].vbo, 6+2*i, 2, 6+this->meshes[id].uvmaps*2));
        }
        
        //setup the Uniform buffer holding the model matrix
        // but offload the actual OpenGL commands for later, since this runs in a performance
        // critical section
        this->meshes[id].ubo = this->api->newUniformBuffer();
        
        auto model_mat = mesh->GetModelMatrix();
        
        this->meshes[id].data = OE_Mat4x4ToSTDVector(model_mat);
        this->meshes[id].changed = true;
        
        // handle Vertex (Triangle) groups
        for (auto vgroup : mesh->data.triangle_groups){
            this->handleVGroupData(id, vgroup.first, mesh);
        }
        mesh->data.ibos.clear();
        
        
        // store bounding box
        
        //---- <TEMPORARY> ----//
        // This should not be done here but in the physics engine
        mesh->calculateProperBoundingBox();
        //---- </TEMPORARY> ---//
        
        this->meshes[id].vbo_bbox = this->api->newVertexBuffer();
        this->api->setVertexBufferMemoryData(this->meshes[id].vbo_bbox, mesh->data.vertices.genBoundingBoxMesh(), NRE_GPU_STREAM);
        
        this->meshes[id].vao_bbox = this->api->newVertexLayout();
        std::vector<VLI> vao_bbox_data;
        vao_bbox_data.push_back(VLI(this->meshes[id].vbo_bbox, 0, 3, 6));
        vao_bbox_data.push_back(VLI(this->meshes[id].vbo_bbox, 3, 3, 6));
        
        this->api->setVertexLayoutFormat(this->meshes[id].vao_bbox, vao_bbox_data);
        
    }
    else{
        this->meshes[id].data = OE_Mat4x4ToSTDVector(mesh->GetModelMatrix());
        
        bool render_bboxes = this->render_bounding_boxes.load(std::memory_order_relaxed);
        if (render_bboxes){
            //---- <TEMPORARY> ----//
            // This should not be done here but in the physics engine
            mesh->calculateProperBoundingBox();
            //---- </TEMPORARY> ---//
        
            this->api->setVertexBufferData(this->meshes[id].vbo_bbox, mesh->data.vertices.genBoundingBoxMesh(), 0);
        }
        
        this->meshes[id].changed = true;
    }
}

void NRE_Renderer::handleVGroupData(std::size_t mesh_id, std::size_t id, OE_Mesh32* mesh){
    if (this->vgroups.count(id) == 0){
        
        this->vgroups[id] = NRE_VGroupRenderData(); this->vgroups[id].id = id; this->vgroups[id].mesh_id = mesh_id;
        this->vgroups[id].bone_mat = OE_Mat4x4(1.0f);
        this->vgroups[id].ibo = this->api->newIndexBuffer();
        this->vgroups[id].material_id = mesh->data.triangle_groups[id]->material_id;
        //this->api->setIndexBufferMemory(this->vgroups[id].ibo, mesh->data.ibos[id].data.size(), NRE_GPU_STATIC);
        //this->api->setIndexBufferData(this->vgroups[id].ibo, mesh->data.ibos[id].data, 0);
        this->api->setIndexBufferMemoryData(this->vgroups[id].ibo, mesh->data.ibos[id].data, NRE_GPU_STATIC);
        mesh->data.ibos[id].data.clear();
        
    }
}

void NRE_Renderer::handleMaterialData(std::size_t id, OE_Material* mat){
    if (this->materials.count(id) == 0){
        this->materials[id] = NRE_MaterialRenderData(); this->materials[id].id = id;
        this->materials[id].ubo = this->api->newUniformBuffer();
        this->materials[id].data = mat->GetRendererData();
        this->materials[id].changed = true;
    } 
    else{
        this->materials[id].data = mat->GetRendererData();
        this->materials[id].changed = true;
    }
}

void NRE_Renderer::handleCameraData(std::size_t id, OE_Camera* camera){
    if (this->cameras.count(id) == 0){
        
        //setup the Uniform buffer holding the perspective/view matrix
        // but offload the actual OpenGL commands for later, since this runs in a performance
        // critical section
        this->cameras[id].ubo = this->api->newUniformBuffer();

        auto view_mat = camera->GetViewMatrix();
        auto perspective_mat = OE_Perspective(camera->fov, camera->aspect_ratio, (float)camera->near+1.0f, (float)camera->far);
        
        this->cameras[id].data = OE_Mat4x4ToSTDVector(perspective_mat*view_mat);
        this->cameras[id].changed = true;
        
    }
    else {

        auto perspective_mat = OE_Perspective(camera->fov, camera->aspect_ratio, (float)camera->near+0.4f, (float)camera->far);
        auto view_mat = camera->GetViewMatrix();
        
        this->cameras[id].data = OE_Mat4x4ToSTDVector(perspective_mat*view_mat);
        this->cameras[id].changed = true;
        
    }
}

void NRE_Renderer::handleLightData(std::size_t id, OE_Light* light){
    
}

//---------------------------------Update actual GPU data------------------------------//

void NRE_Renderer::updateMeshGPUData(){
    for (auto mesh: this->meshes){
        if (!this->meshes[mesh.first].vao_initialized){
            this->api->setVertexLayoutFormat(this->meshes[mesh.first].vao, this->meshes[mesh.first].vao_input);
            this->meshes[mesh.first].vao_initialized = true;
        }
        if (this->meshes[mesh.first].changed){
            
            if (this->meshes[mesh.first].size != this->meshes[mesh.first].data.size()){
                this->meshes[mesh.first].size = this->meshes[mesh.first].data.size();
                this->api->setUniformBufferMemory(this->meshes[mesh.first].ubo, this->meshes[mesh.first].size, NRE_GPU_STREAM);
            }
            
            this->api->setUniformBufferData(this->meshes[mesh.first].ubo, this->meshes[mesh.first].data, 0);
            this->meshes[mesh.first].changed = false;
        }
    }
}

void NRE_Renderer::updateMaterialGPUData(){
    for (auto mat: this->materials){
        if (this->materials[mat.first].changed){
            
            if (this->materials[mat.first].size != this->materials[mat.first].data.size()){
                this->materials[mat.first].size = this->materials[mat.first].data.size();
                this->api->setUniformBufferMemory(this->materials[mat.first].ubo, this->materials[mat.first].size, NRE_GPU_DYNAMIC);
            }
            
            this->api->setUniformBufferData(this->materials[mat.first].ubo, this->materials[mat.first].data, 0);
            this->materials[mat.first].changed = false;
        }
    }
}

void NRE_Renderer::updateCameraGPUData(){
    for (auto cam: this->cameras){
        if (this->cameras[cam.first].changed){
            
            if (this->cameras[cam.first].size != this->cameras[cam.first].data.size()){
                this->cameras[cam.first].size = this->cameras[cam.first].data.size();
                this->api->setUniformBufferMemory(this->cameras[cam.first].ubo, this->cameras[cam.first].size, NRE_GPU_STREAM);
            }
            
            this->api->setUniformBufferData(this->cameras[cam.first].ubo, this->cameras[cam.first].data, 0);
            this->cameras[cam.first].changed = false;
        }
    }
}

