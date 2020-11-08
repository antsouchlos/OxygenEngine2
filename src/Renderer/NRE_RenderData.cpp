#include <Renderer/NRE_RendererMain.h>
#include <OE_API.h>

using namespace std;
using namespace OE;

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
    
    // remove any obsolete draw commands
    std::set<std::size_t, std::greater<std::size_t>> to_be_removed;
    for (size_t i=0; i < this->render_groups.size(); i++){
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
        this->render_groups.erase(this->render_groups.begin() + x);
    }
    //TODO: remove any obsolete objects themselves
    //for (auto x : std::exchange(temp_objects.deleted, {})){
    //    
    //}    
    
    // PRELIMINARY WORK
    // only handle the first scene for now with 1 camera and no materials
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

void NRE_Renderer::handleMeshData(std::size_t id, OE_Mesh32* mesh){
    if (this->meshes.count(id) == 0){
        
        //auto t = clock();
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
        this->meshes[id].vao = this->api->newVertexLayout();
        typedef NRE_GPU_VertexLayoutInput VLI; // for clarity
        vector<VLI> vao_arg;
        
        vao_arg.push_back(VLI(this->meshes[id].vbo, 0, 3, 6+this->meshes[id].uvmaps*2));
        vao_arg.push_back(VLI(this->meshes[id].vbo, 3, 3, 6+this->meshes[id].uvmaps*2));
        
        for (size_t i=0; i < this->meshes[id].uvmaps; i++){
            vao_arg.push_back(VLI(this->meshes[id].vbo, 6+2*i, 2, 6+this->meshes[id].uvmaps*2));
        }
        this->api->setVertexLayoutFormat(this->meshes[id].vao, vao_arg);
        
        //setup the Uniform buffer holding the model matrix
        this->meshes[id].ubo = this->api->newUniformBuffer();
        this->api->setUniformBufferMemory(this->meshes[id].ubo, 16, NRE_GPU_STREAM);
        
        auto model_mat = mesh->GetModelMatrix();
        
        this->api->setUniformBufferData(this->meshes[id].ubo, OE_Mat4x4ToSTDVector(model_mat),0);
        
        
        // handle Vertex (Triangle) groups
        for (auto vgroup : mesh->data.triangle_groups){
            this->handleVGroupData(id, vgroup.first, mesh);
        }
        mesh->data.ibos.clear();
        //cout << "NRE DATA BENCHMARK MESH: " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    }
    else{
        this->api->setUniformBufferData(this->meshes[id].ubo, OE_Mat4x4ToSTDVector(mesh->GetModelMatrix()),0);
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
        
        this->api->setUniformBufferMemory(this->materials[id].ubo, 12, NRE_GPU_DYNAMIC);
        this->api->setUniformBufferData(this->materials[id].ubo, mat->GetRendererData(), 0);
    } else{
        this->api->setUniformBufferData(this->materials[id].ubo, mat->GetRendererData(), 0);
    }
}

void NRE_Renderer::handleCameraData(std::size_t id, OE_Camera* camera){
    if (this->cameras.count(id) == 0){
        
        //setup the Uniform buffer holding the perspective/view matrix
        this->cameras[id].ubo = this->api->newUniformBuffer();
        this->api->setUniformBufferMemory(this->cameras[id].ubo, 16, NRE_GPU_STREAM);

        auto view_mat = camera->GetViewMatrix();
        auto perspective_mat = OE_Perspective(camera->fov, camera->aspect_ratio, (float)camera->near+0.1f, (float)camera->far*10.0f);
        
        this->api->setUniformBufferData(this->cameras[id].ubo, OE_Mat4x4ToSTDVector(perspective_mat*view_mat),0);
    }
    else {

        auto perspective_mat = OE_Perspective(camera->fov, camera->aspect_ratio, (float)camera->near+0.1f, (float)camera->far);
        auto view_mat = camera->GetViewMatrix();
        
        this->api->setUniformBufferData(this->cameras[id].ubo, OE_Mat4x4ToSTDVector(perspective_mat*view_mat),0);
    }
}

void NRE_Renderer::handleLightData(std::size_t id, OE_Light* light){
    
}
