#include <OE/Renderer/DataHandler/data_handler.h>
#include <OE/global_variables.h>

using namespace oe;
using namespace std;

bool nre::data_handler_t::update(bool restart_renderer, bool load_minmax_elements) {

    this->load_spheres_or_bboxes_ = load_minmax_elements;
    // add/change any missing world data
    // OE_Main->lockMutex();

    vector<size_t> camera_ids;

    // this regenerates the renderer database

    if (restart_renderer) {

        // cout << OE_Main->world->to_str() << endl;

        for (auto mat : oe::materials_list) {
            mat.flag_as_changed();
        }

        for (auto obj : oe::objects_list) {
            if (obj->getType() == OE_OBJECT_MESH) {
                auto mesh              = static_pointer_cast<OE_Mesh32>(obj.get_pointer());
                mesh->data->vbo_exists = false;
                mesh->data->ibos_exist = false;
            }
            obj.flag_as_changed();
        }

        for (auto sce : oe::scenes_list) {
            sce.flag_as_changed();
        }

        for (auto vpc : oe::viewports_list) {
            vpc.flag_as_changed();
        }
    }

    // Update element data

    for (auto mat : oe::materials_list.changed()) {
        this->handle_material_data(mat.get_id(), mat.get_pointer());
    }

    this->has_dir_lights_changed_ = false;
    this->has_pt_lights_changed_  = false;

    for (auto obj : oe::objects_list.changed()) {
        if (obj->getType() == OE_OBJECT_MESH) {
            this->handle_mesh_data(obj.get_id(), static_pointer_cast<OE_Mesh32>(obj.get_pointer()));
        }
        else if (obj->getType() == OE_OBJECT_LIGHT) {
            this->handle_light_data(obj.get_id(), static_pointer_cast<OE_Light>(obj.get_pointer()));
        }
    }

    for (auto obj : oe::objects_list.changed()) {
        if (obj->getType() == OE_OBJECT_CAMERA) {
            this->handle_camera_data(obj.get_id(), static_pointer_cast<OE_Camera>(obj.get_pointer()));
            camera_ids.push_back(obj.get_id());
        }
    }

    for (auto sce : oe::scenes_list.changed()) {
        this->handle_scene_data(sce.get_id(), sce.get_pointer());
    }

    for (auto vpc : oe::viewports_list.changed()) {
        this->handle_viewport_data(vpc.get_id(), vpc.get_pointer());
        this->loaded_viewport_ = oe::world->get_loaded_viewport();
    }

    // handle deleted elements (should look if the element exists first)

    for (auto mat : oe::materials_list.deleted()) {
        if (this->materials_.count(mat) == 1) {
            this->deleted_materials_.insert(mat);
        }
    }

    for (auto obj : oe::objects_list.deleted()) {

        if (this->meshes_.count(obj) == 1) {
            this->deleted_meshes_.insert(obj);
        }
        else if (this->dir_lights_.count(obj) == 1) {
            this->dir_lights_.erase(obj);
            this->has_dir_lights_changed_ = true;
        }
        else if (this->pt_lights_.count(obj) == 1) {
            this->pt_lights_.erase(obj);
            this->has_pt_lights_changed_ = true;
        }
        else if (this->cameras_.count(obj) == 1) {
            this->deleted_cameras_.insert(obj);
        }
        else {
        }
    }


    for (auto sce : oe::scenes_list.deleted()) {
        if (this->scenes_.count(sce) == 1) {
            this->deleted_scenes_.insert(sce);
        }
    }

    for (auto vpc : oe::viewports_list.deleted()) {
        if (this->viewports_.count(vpc) == 1) {
            this->viewports_.erase(vpc);
            if (this->loaded_viewport_ == vpc) {
                this->loaded_viewport_ = 0; // needed for compatibility with older .csl with no viewportconfigs
            }
        }
    }

    // OE_Main->unlockMutex();
    return true;
}

//-----------------------------------Pass individual element changes synchronously as fast as possible--------//

void nre::data_handler_t::handle_mesh_data(std::size_t id, std::shared_ptr<OE_Mesh32> mesh) {
    if (this->meshes_.count(id) == 0) {

        // make sure all index and vertex buffers are generated
        // On loading objects from disk they are not generated here
        // But on objects created on the fly they ARE generated here
        mesh->data->genVertexBufferInternally();
        mesh->data->genIndexBuffersInternally();

        // setup the Vertex Buffer
        this->meshes_[id]        = nre::mesh_render_data_t();
        this->meshes_[id].id     = id;
        this->meshes_[id].uvmaps = mesh->data->vertices.uvmaps.size();
        this->meshes_[id].size   = mesh->data->vbo.size();

        // handle Vertex (Triangle) groups
        for (auto vgroup : mesh->data->triangle_groups) {
            this->handle_vgroup_data(id, vgroup.first, mesh);
            this->meshes_[id].vgroups.insert(vgroup.first);
        }

        // store mesh vbo and index buffers
        mesh->data->vbo_mutex.lockMutex();
        this->meshes_[id].vbo_data = std::move(mesh->data->vbo);
        mesh->data->vbo_mutex.unlockMutex();

        mesh->data->ibos_mutex.lockMutex();
        for (auto vg : this->meshes_[id].vgroups) {
            this->meshes_[id].ibos_data[vg] = std::move(mesh->data->ibos[vg].data);
        }
        mesh->data->ibos_mutex.unlockMutex();
        // setup the Uniform buffer holding the model matrix
        //  but offload the actual OpenGL commands for later, since this runs in a performance
        //  critical section

        this->meshes_[id].model_mat = mesh->GetModelMatrix();

        // this->meshes_[id].data = OE_Mat4x4ToSTDVector(model_mat);
        this->meshes_[id].changed = true;

        // store bounding box

        //---- <TEMPORARY> ----//
        // This should not be done here but in the physics engine or interpreter
        mesh->calculateProperBoundingBox();
        //---- </TEMPORARY> ---//

        assert(mesh->data->vertices.calculatedBoundingBox);

        this->meshes_[id].max_x = mesh->data->vertices.max_x;
        this->meshes_[id].max_y = mesh->data->vertices.max_y;
        this->meshes_[id].max_z = mesh->data->vertices.max_z;

        this->meshes_[id].min_x = mesh->data->vertices.min_x;
        this->meshes_[id].min_y = mesh->data->vertices.min_y;
        this->meshes_[id].min_z = mesh->data->vertices.min_z;

        this->meshes_[id].max_radius = mesh->data->vertices.max_radius;
        this->meshes_[id].min_radius = mesh->data->vertices.min_radius;
    }
    else {
        this->meshes_[id].model_mat = mesh->GetModelMatrix();

        if (this->load_spheres_or_bboxes_) {
            //---- <TEMPORARY> ----//
            // This should not be done here but in the physics engine
            mesh->calculateProperBoundingBox();
            //---- </TEMPORARY> ---//

            assert(mesh->data->vertices.calculatedBoundingBox);

            this->meshes_[id].max_x = mesh->data->vertices.max_x;
            this->meshes_[id].max_y = mesh->data->vertices.max_y;
            this->meshes_[id].max_z = mesh->data->vertices.max_z;

            this->meshes_[id].min_x = mesh->data->vertices.min_x;
            this->meshes_[id].min_y = mesh->data->vertices.min_y;
            this->meshes_[id].min_z = mesh->data->vertices.min_z;

            this->meshes_[id].max_radius = mesh->data->vertices.max_radius;
            this->meshes_[id].min_radius = mesh->data->vertices.min_radius;
        }

        this->meshes_[id].changed = true;
    }
}

void nre::data_handler_t::handle_vgroup_data(std::size_t mesh_id, std::size_t id, std::shared_ptr<OE_Mesh32> mesh) {
    if (this->vgroups_.count(id) == 0) {

        this->vgroups_[id]             = nre::vgroup_render_data_t();
        this->vgroups_[id].id          = id;
        this->vgroups_[id].mesh_id     = mesh_id;
        this->vgroups_[id].bone_mat    = OE_Mat4x4(1.0f);
        this->vgroups_[id].ibo         = 0;
        this->vgroups_[id].material_id = mesh->data->triangle_groups[id]->material_id;
    }
}

void nre::data_handler_t::handle_material_data(std::size_t id, std::shared_ptr<OE_Material> mat) {
    if (this->materials_.count(id) == 0) {
        this->materials_[id]    = nre::material_render_data_t();
        this->materials_[id].id = id;
    }

    this->materials_[id].data    = mat->GetRendererData();
    this->materials_[id].changed = true;
}

void nre::data_handler_t::handle_camera_data(std::size_t id, std::shared_ptr<OE_Camera> camera) {
    if (this->cameras_.count(id) == 0) {

        // setup the Uniform buffer holding the perspective/view matrix
        //  but offload the actual OpenGL commands for later, since this runs in a performance
        //  critical section
        this->cameras_[id]    = nre::camera_render_data_t();
        this->cameras_[id].id = id;
    }

    this->cameras_[id].near = (float)camera->near + 0.3f;
    this->cameras_[id].far  = (float)camera->far + 50.0f;

    auto view_mat        = camera->GetViewMatrix();
    auto perspective_mat = OE_Perspective(camera->fov, camera->aspect_ratio, this->cameras_[id].near, this->cameras_[id].far);

    this->cameras_[id].view_mat             = view_mat;
    this->cameras_[id].perspective_mat      = perspective_mat;
    this->cameras_[id].model_mat            = camera->GetModelMatrix();
    this->cameras_[id].perspective_view_mat = perspective_mat * view_mat;

    this->cameras_[id].data = OE_Mat4x4ToSTDVector(this->cameras_[id].perspective_view_mat);

    this->cameras_[id].data.push_back(this->cameras_[id].model_mat[3].x);
    this->cameras_[id].data.push_back(this->cameras_[id].model_mat[3].y);
    this->cameras_[id].data.push_back(this->cameras_[id].model_mat[3].z);
    this->cameras_[id].data.push_back(1.0f);
    this->cameras_[id].changed = true;
}

void nre::data_handler_t::handle_light_data(std::size_t id, std::shared_ptr<OE_Light> light) {
    if (light->light_type == 1) {
        // POINT LIGHT
        if (this->pt_lights_.count(id) == 0) {
            this->pt_lights_[id]         = nre::point_light_render_data_t();
            this->pt_lights_[id].id      = id;
            this->has_pt_lights_changed_ = true;
        }

        this->pt_lights_[id].model_mat = light->GetModelMatrix();
        this->pt_lights_[id].color     = light->color;
        this->pt_lights_[id].intensity = light->intensity;
        this->pt_lights_[id].range     = light->range / 20.0f;

        // this->pt_lights_[id].data = light->GetLightGPU_data();
        this->pt_lights_[id].data = OE_Mat4x4ToSTDVector(this->pt_lights_[id].model_mat);
        /*this->pt_lights_[id].data.push_back(1.0f);
        this->pt_lights_[id].data.push_back(1.0f);
        this->pt_lights_[id].data.push_back(1.0f);
        this->pt_lights_[id].data.push_back(light->range/20.0f);
        this->pt_lights_[id].data.push_back(1.0f);
        this->pt_lights_[id].data.push_back(1.0f);
        this->pt_lights_[id].data.push_back(1.0f);
        this->pt_lights_[id].data.push_back(light->range/20.0f);*/
        this->pt_lights_[id].data[15] = light->range / 20.0f;

        this->pt_lights_[id].changed = true;
    }
    else if (light->light_type == 2) {
        // DIRECTIONAL LIGHT
    }
    else if (light->light_type == 3) {
        // SPOTLIGHT
    }
    else {
        // NOTHING
    }
}

void nre::data_handler_t::handle_scene_data(std::size_t id, std::shared_ptr<OE_Scene> scene) {
    if (this->scenes_.count(id) == 0) {
        this->scenes_[id]    = nre::scene_render_data_t();
        this->scenes_[id].id = id;
    }
    else {
        this->scenes_[id].cameras.clear();
        this->scenes_[id].dir_lights.clear();
        this->scenes_[id].pt_lights.clear();
        this->scenes_[id].meshes.clear();
    }

    // group objects
    for (auto x : scene->objects) {
        if (this->cameras_.count(x) != 0) {
            this->scenes_[id].cameras.insert(x);
        }
        else if (this->meshes_.count(x) != 0) {
            this->scenes_[id].meshes.insert(x);
        }
        else if (this->dir_lights_.count(x) != 0) {
            this->scenes_[id].dir_lights.insert(x);
        }
        else if (this->pt_lights_.count(x) != 0) {
            this->scenes_[id].pt_lights.insert(x);
        }
    }

    // store the scene each camera is in
    for (auto cam : this->scenes_[id].cameras) {
        this->cameras_[cam].scene_id = id;
        this->cameras_[cam].changed  = true;
    }
    // store the scene each mesh is in
    for (auto mat : this->scenes_[id].materials) {
        this->materials_[mat].scene_id = id;
        this->materials_[mat].changed  = true;
    }
    // store the scene each material is in
    for (auto mesh : this->scenes_[id].meshes) {
        this->meshes_[mesh].scene_id = id;
        this->meshes_[mesh].changed  = true;
    }

    this->scenes_[id].materials = scene->materials;
    this->scenes_[id].changed   = true;
}

void nre::data_handler_t::handle_viewport_data(std::size_t id, std::shared_ptr<OE_ViewportConfig> vp_config) {
    if (this->viewports_.count(id) == 0) {
        this->viewports_[id]          = nre::viewport_render_data_t();
        this->viewports_[id].id       = id;
        this->viewports_[id].has_init = true;
    }

    this->viewports_[id].layers                 = vp_config->layers;
    this->viewports_[id].cameras                = vp_config->cameras;
    this->viewports_[id].camera_modes           = vp_config->camera_modes;
    this->viewports_[id].layer_combine_modes    = vp_config->layer_combine_modes;
    this->viewports_[id].split_screen_positions = vp_config->split_screen_positions;
    this->viewports_[id].changed                = true;
}

// deletes

void nre::data_handler_t::delete_camera(std::size_t id) {

    if (this->cameras_[id].ubo != 0) {
        nre::gpu::del_uniform_buf(this->cameras_[id].ubo);
    }

    this->cameras_.erase(id);
}

void nre::data_handler_t::delete_material(std::size_t id) {

    if (this->materials_[id].ubo != 0) {
        nre::gpu::del_uniform_buf(this->materials_[id].ubo);
    }

    this->materials_.erase(id);
}

void nre::data_handler_t::delete_mesh(std::size_t id) {

    // delete all buffers
    if (this->meshes_[id].vbo != 0) {
        nre::gpu::del_vertex_buf(this->meshes_[id].vbo);
    }
    if (this->meshes_[id].vao != 0) {
        nre::gpu::del_vertex_layout(this->meshes_[id].vao);
    }
    if (this->meshes_[id].ubo != 0) {
        nre::gpu::del_uniform_buf(this->meshes_[id].ubo);
    }

    // delete buffers of vertex groups
    for (auto vgroup : this->meshes_[id].vgroups) {
        if (this->vgroups_[vgroup].ibo != 0) {
            nre::gpu::del_index_buf(this->vgroups_[vgroup].ibo);
        }
        this->vgroups_.erase(vgroup);
    }
    this->meshes_.erase(id);
}

void nre::data_handler_t::clear() {

    this->cameras_.clear();
    this->materials_.clear();
    this->vgroups_.clear();
    this->meshes_.clear();
    this->dir_lights_.clear();
    this->pt_lights_.clear();
    this->scenes_.clear();
    this->viewports_.clear();
}
