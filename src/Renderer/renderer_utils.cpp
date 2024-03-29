#include <OE/Renderer/renderer_legacy.h>
#include <OE/Renderer/renderer_main.h>

bool nre::render_group::operator<(const render_group& other) const {

    // sort by camera
    if (this->camera < other.camera) {
        return true;
    }
    else if (this->camera > other.camera) {
        return false;
    }
    else {
        // sort by material (program) since pipeline/texture changes
        // are expensive
        if (this->material < other.material) {
            return true;
        }
        else if (this->material > other.material) {
            return false;
        }
        else {
            // sort by mesh, to minimize changing VAO
            // and Mesh Buffers
            if (this->mesh < other.mesh) {
                return true;
            }
            else if (this->mesh > other.mesh) {
                return false;
            }
            else {
                // finally sort by vgroup since changing Index Buffers should be cheap
                if (this->vgroup < other.vgroup) {
                    return true;
                }
                else if (this->vgroup >= other.vgroup) {
                    return false;
                }
            }
        }
    }
    return false;
}

nre::point_light_drawcall::point_light_drawcall(std::size_t id_in, float z_in, std::size_t priority_in) {
    this->id       = id_in;
    this->z        = z_in;
    this->priority = priority_in;
}

bool nre::point_light_drawcall::operator>(const point_light_drawcall& other) const {
    if (this->priority == other.priority) {
        if (std::abs(this->z - other.z) < 0.0000001f) {
            return this->id > other.id;
        }
        else {
            return this->z > other.z;
        }
    }
    else {
        return this->priority > other.priority;
    }
    return false;
}

void nre::renderer_t::sortPointLights(std::size_t scene_id, std::size_t camera_id) {
    this->pt_visible_lights_.clear();

    auto persp_mat = data_.cameras_[camera_id].perspective_mat;

    // set limit to 255 lights
    // why you ask?
    // Well firstly 255 lights is already a LOT of lights inside the view frustum. More lights are not that good for performance
    // Secondly the minimum required OpenGL Uniform buffer limit can only contain 256 matrices,
    // so i can only place up to 255 lights (light with index 0 inside the shaders symbolizes the lack of lights)
    // Thirdly, spot lights will be handed in a separate pass with another texture

    for (auto l : data_.scenes_[scene_id].pt_lights) {

        // calculations happening in view space
        OE_Vec4 model_pos = OE_Vec4(data_.pt_lights_[l].model_mat[3].x, data_.pt_lights_[l].model_mat[3].y,
                                    data_.pt_lights_[l].model_mat[3].z, 1.0f);

        OE_Vec4 light_view_pos = data_.cameras_[camera_id].view_mat * model_pos;

        auto light_camview_radius =
            light_view_pos - OE_Vec4(data_.pt_lights_[l].range, data_.pt_lights_[l].range, data_.pt_lights_[l].range, 0.0f);

        bool is_behind_camera = light_camview_radius.z > (-data_.cameras_[camera_id].near);
        bool is_too_far_away  = (light_camview_radius.z + 2 * data_.pt_lights_[l].range) < (-data_.cameras_[camera_id].far);

        // calculations happening in clip space
        auto light_clip_radius_max_x =
            OE_GetClipCoords(persp_mat, light_view_pos + OE_Vec4(data_.pt_lights_[l].range, 0.0f, 0.0f, 0.0f));

        auto light_clip_radius_min_x =
            OE_GetClipCoords(persp_mat, light_view_pos - OE_Vec4(data_.pt_lights_[l].range, 0.0f, 0.0f, 0.0f));

        auto light_clip_radius_max_y =
            OE_GetClipCoords(persp_mat, light_view_pos + OE_Vec4(0.0f, data_.pt_lights_[l].range, 0.0f, 0.0f));

        auto light_clip_radius_min_y =
            OE_GetClipCoords(persp_mat, light_view_pos - OE_Vec4(0.0f, data_.pt_lights_[l].range, 0.0f, 0.0f));

        bool is_too_left  = light_clip_radius_max_x.x < -1.02f;
        bool is_too_right = light_clip_radius_min_x.x > 1.02f;

        bool is_too_above = light_clip_radius_max_y.y < -1.02f;
        bool is_too_below = light_clip_radius_min_y.y > 1.02f;

        // only choose lights that have effects inside the view frustum.
        // This means that up to 254 lights in range of the camera will be active at once
        // This means the scene can have many many more lights overall. All that matters
        // is what's VISIBLE
        if ((not is_behind_camera) and (not is_too_far_away) and (not is_too_left) and (not is_too_right) and
            (not is_too_above) and (not is_too_below)) {
            auto point_light = nre::point_light_drawcall(
                l, (data_.cameras_[camera_id].perspective_view_mat * data_.pt_lights_[l].model_mat[3])[2], 0);
            this->pt_visible_lights_.insert(point_light);
        }
    }
    // cout << "pt size " << this->pt_visible_lights.size() << endl;
}

void nre::renderer_t::generateDrawCalls() {

    // delete all remaining draw call programs from deleted scenes
    for (auto& sce : data_.deleted_scenes_) {
        for (auto ren_group : this->sce_ren_groups_[sce]) {

            if (ren_group.z_prepass_program != 0) nre::gpu::del_program(ren_group.z_prepass_program);
            if (ren_group.program != 0) nre::gpu::del_program(ren_group.program);
        }
        data_.scenes_.erase(sce);
        this->sce_ren_groups_.erase(sce);
    }
    data_.deleted_scenes_.clear();

    for (auto& scene : data_.scenes_) {

        if (!this->sce_ren_groups_.count(scene.first)) {
            this->sce_ren_groups_[scene.first] = nre::drawcall_container_t();
        }

        // delete remaining draw call programs from existing scenes
        for (auto ren_group : this->sce_ren_groups_[scene.first].deleted()) {

            if (ren_group.z_prepass_program != 0) nre::gpu::del_program(ren_group.z_prepass_program);
            if (ren_group.program != 0) nre::gpu::del_program(ren_group.program);
        }
        this->sce_ren_groups_[scene.first].cleanupPrograms();

        // gemerate draw calls anew
        for (auto cam : scene.second.cameras) {
            for (auto mesh : scene.second.meshes) {
                for (auto vgroup : data_.meshes_[mesh].vgroups) {
                    auto render_data     = nre::render_group();
                    render_data.camera   = cam;
                    render_data.vgroup   = vgroup;
                    render_data.mesh     = mesh;
                    render_data.material = data_.vgroups_[vgroup].material_id;
                    if (!this->sce_ren_groups_[scene.first].contains(render_data)) {
                        this->sce_ren_groups_[scene.first].insert(render_data);
                    }
                }
            }
        }
    }
}

// legacy renderer

void nre::renderer_legacy_t::sortPointLights(std::size_t scene_id, std::size_t camera_id) {
    this->pt_visible_lights_.clear();

    auto persp_mat = data_.cameras_[camera_id].perspective_mat;

    // set limit to 255 lights
    // why you ask?
    // Well firstly 255 lights is already a LOT of lights inside the view frustum. More lights are not that good for performance
    // Secondly the minimum required OpenGL Uniform buffer limit can only contain 256 matrices,
    // so i can only place up to 255 lights (light with index 0 inside the shaders symbolizes the lack of lights)
    // Thirdly, spot lights will be handed in a separate pass with another texture

    for (auto l : data_.scenes_[scene_id].pt_lights) {

        // calculations happening in view space
        OE_Vec4 model_pos = OE_Vec4(data_.pt_lights_[l].model_mat[3].x, data_.pt_lights_[l].model_mat[3].y,
                                    data_.pt_lights_[l].model_mat[3].z, 1.0f);

        OE_Vec4 light_view_pos = data_.cameras_[camera_id].view_mat * model_pos;

        auto light_camview_radius =
            light_view_pos - OE_Vec4(data_.pt_lights_[l].range, data_.pt_lights_[l].range, data_.pt_lights_[l].range, 0.0f);

        bool is_behind_camera = light_camview_radius.z > (-data_.cameras_[camera_id].near);
        bool is_too_far_away  = (light_camview_radius.z + 2 * data_.pt_lights_[l].range) < (-data_.cameras_[camera_id].far);

        // calculations happening in clip space
        auto light_clip_radius_max_x =
            OE_GetClipCoords(persp_mat, light_view_pos + OE_Vec4(data_.pt_lights_[l].range, 0.0f, 0.0f, 0.0f));

        auto light_clip_radius_min_x =
            OE_GetClipCoords(persp_mat, light_view_pos - OE_Vec4(data_.pt_lights_[l].range, 0.0f, 0.0f, 0.0f));

        auto light_clip_radius_max_y =
            OE_GetClipCoords(persp_mat, light_view_pos + OE_Vec4(0.0f, data_.pt_lights_[l].range, 0.0f, 0.0f));

        auto light_clip_radius_min_y =
            OE_GetClipCoords(persp_mat, light_view_pos - OE_Vec4(0.0f, data_.pt_lights_[l].range, 0.0f, 0.0f));

        bool is_too_left  = light_clip_radius_max_x.x < -1.02f;
        bool is_too_right = light_clip_radius_min_x.x > 1.02f;

        bool is_too_above = light_clip_radius_max_y.y < -1.02f;
        bool is_too_below = light_clip_radius_min_y.y > 1.02f;

        // only choose lights that have effects inside the view frustum.
        // This means that up to 254 lights in range of the camera will be active at once
        // This means the scene can have many many more lights overall. All that matters
        // is what's VISIBLE
        if ((not is_behind_camera) and (not is_too_far_away) and (not is_too_left) and (not is_too_right) and
            (not is_too_above) and (not is_too_below)) {
            auto point_light = nre::point_light_drawcall(
                l, (data_.cameras_[camera_id].perspective_view_mat * data_.pt_lights_[l].model_mat[3])[2], 0);
            this->pt_visible_lights_.insert(point_light);
        }
    }
    // cout << "pt size " << this->pt_visible_lights.size() << endl;
}

void nre::renderer_legacy_t::generateDrawCalls() {

    // delete all remaining draw call programs from deleted scenes
    for (auto& sce : data_.deleted_scenes_) {
        for (auto ren_group : this->sce_ren_groups_[sce]) {

            if (ren_group.z_prepass_program != 0) nre::gpu::del_program(ren_group.z_prepass_program);
            if (ren_group.program != 0) nre::gpu::del_program(ren_group.program);
        }
        data_.scenes_.erase(sce);
        this->sce_ren_groups_.erase(sce);
    }
    data_.deleted_scenes_.clear();

    for (auto& scene : data_.scenes_) {

        if (!this->sce_ren_groups_.count(scene.first)) {
            this->sce_ren_groups_[scene.first] = nre::drawcall_container_t();
        }

        // delete remaining draw call programs from existing scenes
        for (auto ren_group : this->sce_ren_groups_[scene.first].deleted()) {

            if (ren_group.z_prepass_program != 0) nre::gpu::del_program(ren_group.z_prepass_program);
            if (ren_group.program != 0) nre::gpu::del_program(ren_group.program);
        }
        this->sce_ren_groups_[scene.first].cleanupPrograms();

        // gemerate draw calls anew
        for (auto cam : scene.second.cameras) {
            for (auto mesh : scene.second.meshes) {
                for (auto vgroup : data_.meshes_[mesh].vgroups) {
                    auto render_data     = nre::render_group();
                    render_data.camera   = cam;
                    render_data.vgroup   = vgroup;
                    render_data.mesh     = mesh;
                    render_data.material = data_.vgroups_[vgroup].material_id;
                    if (!this->sce_ren_groups_[scene.first].contains(render_data)) {
                        this->sce_ren_groups_[scene.first].insert(render_data);
                    }
                }
            }
        }
    }
}
