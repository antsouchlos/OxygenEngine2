#include <OE/Renderer/renderer_legacy.h>
#include <OE/math_oe.h>
#include <iostream>

using namespace std;

nre::renderer_legacy_t::renderer_legacy_t() : renderer_base_t(false, "renderer_legacy") {
}

nre::renderer_legacy_t::~renderer_legacy_t() {
}

bool nre::renderer_legacy_t::init(oe::renderer_init_info init_info, oe::renderer_update_info update_info,
                                  oe::winsys_output winsys_info) {


    // make sure there are no stored objects
    /*cout << "NRE Cameras: " << data_.cameras_.size() << endl;
    cout << "NRE Materials: " << data_.materials_.size() << endl;
    cout << "NRE Meshes: " << data_.meshes_.size() << endl;
    cout << "NRE Vgroups: " << data_.vgroups_.size() << endl;*/

    // make sure we use the right API
    this->init_info_ = init_info;
    nre::gpu::init(winsys_info.backend, winsys_info.major, winsys_info.minor);

    this->initOffscreenFrameBuffer();

    this->initFullscreenQuad();

    this->initGammaCorrectionProg();

    this->initGPUSphere();
    return true;
}

void nre::renderer_legacy_t::initOffscreenFrameBuffer() {
    // setup offscreen framebuffer

    this->framebuffer_  = nre::gpu::new_framebuffer();
    this->colortexture_ = nre::gpu::new_texture();
    this->depthrbo_     = nre::gpu::new_renderbuffer();

    if (this->use_HDR_ == false) {
        nre::gpu::set_texture_format(this->colortexture_, nre::gpu::RGBA, nre::gpu::NEAREST, res_x_, res_y_, 0);
        // nre::gpu::set_texture_format(this->depthtexture, nre::gpu::DEPTHSTENCIL, nre::gpu::NEAREST,
        // res_x_, res_y_, 0);
        nre::gpu::set_renderbuffer_textype(this->depthrbo_, nre::gpu::DEPTHSTENCIL, res_x_, res_y_);
    }
    else {
        nre::gpu::set_texture_format(this->colortexture_, nre::gpu::RGBA16F, nre::gpu::NEAREST, res_x_, res_y_, 0);
        // nre::gpu::set_texture_format(this->depthtexture, nre::gpu::DEPTHSTENCIL, nre::gpu::NEAREST,
        // res_x_, res_y_, 0);
        nre::gpu::set_renderbuffer_textype(this->depthrbo_, nre::gpu::DEPTHSTENCIL, res_x_, res_y_);
    }

    nre::gpu::set_framebuffer_texture(this->framebuffer_, this->colortexture_, 0);
    nre::gpu::set_framebuffer_renderbuffer(this->framebuffer_, this->depthrbo_, 0);
}

void nre::renderer_legacy_t::initFullscreenQuad() {
    // setup fullscreen quad
    this->vbo_fullscreen_quad_ = nre::gpu::new_vertex_buf();
    this->vao_fullscreen_quad_ = nre::gpu::new_vertex_layout();

    nre::gpu::set_vertex_buf_memory_and_data(this->vbo_fullscreen_quad_,
                                             {1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f},
                                             nre::gpu::STATIC);

    typedef nre::gpu::vertex_layout_input VLI; // for clarity
    std::vector<VLI>                      vao_quad_data;
    vao_quad_data.push_back(VLI(this->vbo_fullscreen_quad_, 0, 2, 2));
    nre::gpu::set_vertex_layout_format(this->vao_fullscreen_quad_, vao_quad_data);
}

void nre::renderer_legacy_t::initGammaCorrectionProg() {
    // setup gamma correction program
    this->gamma_cor_prog_ = nre::gpu::new_program();

    nre::gpu::vertex_shader_t vs_gamma;
    nre::gpu::pixel_shader_t  fs_gamma;

    vs_gamma.type           = nre::gpu::VS_UNDEFINED;
    vs_gamma.fullscreenQuad = true;
    fs_gamma.type           = nre::gpu::FS_GAMMA;

    nre::gpu::set_program_vertex_shader(this->gamma_cor_prog_, vs_gamma);
    nre::gpu::set_program_pixel_shader(this->gamma_cor_prog_, fs_gamma);

    nre::gpu::setup_program(this->gamma_cor_prog_);
    // nre::gpu::setProgramTextureSlot(this->gamma_cor_prog_, "tex_output", 0);
}

void nre::renderer_legacy_t::initGPUSphere() {
    // the same VBO, VAO and IBO are used for every object
    // it holds a sphere with radius one

    this->vbo_sphere_ = nre::gpu::new_vertex_buf();
    this->ibo_sphere_ = nre::gpu::new_index_buf();

    auto sphere_vbo_data = OE_GetBoundingSphereVertexBuffer(1.0f, 1.0f, 16);
    auto sphere_ibo_data = OE_GetBoundingSphereIndexBuffer(1.0f, 1.0f, 16);

    nre::gpu::set_vertex_buf_memory_and_data(this->vbo_sphere_, sphere_vbo_data, nre::gpu::STATIC);
    nre::gpu::set_index_buf_memory_and_data(this->ibo_sphere_, sphere_ibo_data, nre::gpu::STATIC);

    typedef nre::gpu::vertex_layout_input VLI; // for clarity
    this->vao_sphere_ = nre::gpu::new_vertex_layout();

    std::vector<VLI> vao_sphere__data;
    vao_sphere__data.push_back(VLI(this->vbo_sphere_, 0, 3, 6));
    vao_sphere__data.push_back(VLI(this->vbo_sphere_, 3, 3, 6));

    nre::gpu::set_vertex_layout_format(this->vao_sphere_, vao_sphere__data);
}

//------------------------updateData---------------------------//

bool nre::renderer_legacy_t::update_data(oe::renderer_update_info update_info, oe::winsys_output winsys_info,
                                         bool has_renderer_restarted) {
    res_x_ = winsys_info.res_x;
    res_y_ = winsys_info.res_y;

    this->use_wireframe_           = update_info.use_wireframe;
    this->render_bounding_boxes_   = update_info.render_bounding_boxes;
    this->render_bounding_spheres_ = update_info.render_bounding_spheres;
    this->use_HDR_                 = update_info.use_hdr;
    this->use_z_prepass_           = update_info.use_z_prepass;
    bool temp_restart_renderer     = (this->shading_mode_ != update_info.shading_mode) or
                                 (use_software_vertex_shaders_ != update_info.use_software_vertex_shaders);

    if (temp_restart_renderer) {
        this->destroy();
        this->init(this->init_info_, update_info, winsys_info);
    }
    else if (has_renderer_restarted) {
        this->init(this->init_info_, update_info, winsys_info);
    }
    this->shading_mode_                = update_info.shading_mode;
    this->use_software_vertex_shaders_ = update_info.use_software_vertex_shaders;
    data_.update(temp_restart_renderer or has_renderer_restarted,
                 this->render_bounding_boxes_ or this->render_bounding_spheres_);

    return true;
}


void nre::renderer_legacy_t::update_software_vertex_shaders(size_t camera_id, size_t scene_id) {
    for (auto mesh : data_.scenes_[scene_id].meshes) {
        auto mvp_mat   = data_.cameras_[camera_id].perspective_view_mat * data_.meshes_[mesh].model_mat;
        auto model_mat = data_.meshes_[mesh].model_mat;
        auto updated_vertices =
            oe::math::vertex_shader_regular_sw(data_.meshes_[mesh].vbo_data, model_mat, mvp_mat, data_.meshes_[mesh].uvmaps);
        nre::gpu::set_vertex_buf_memory_and_data(data_.meshes_[mesh].vbo, updated_vertices, nre::gpu::STREAM);
    }
}

//------------------------updateSIngleThread-------------------//

bool nre::renderer_legacy_t::update_single_thread(oe::renderer_update_info update_info, oe::winsys_output winsys_info) {

    res_x_ = winsys_info.res_x;
    res_y_ = winsys_info.res_y;

    this->use_wireframe_           = update_info.use_wireframe;
    this->render_bounding_boxes_   = update_info.render_bounding_boxes;
    this->render_bounding_spheres_ = update_info.render_bounding_spheres;
    this->use_HDR_                 = update_info.use_hdr;
    this->use_z_prepass_           = update_info.use_z_prepass;

    nre::gpu::update(res_x_, res_y_, update_info.sanity_checks);

    if (not this->use_HDR_) {
        nre::gpu::set_texture_format(this->colortexture_, nre::gpu::RGBA, nre::gpu::NEAREST, res_x_, res_y_, 0);
        // nre::gpu::set_texture_format(this->depthtexture, nre::gpu::DEPTHSTENCIL, nre::gpu::NEAREST,
        // res_x_, res_y_, 0);
        nre::gpu::set_renderbuffer_textype(this->depthrbo_, nre::gpu::DEPTHSTENCIL, res_x_, res_y_);
    }
    else {
        nre::gpu::set_texture_format(this->colortexture_, nre::gpu::RGBA16F, nre::gpu::NEAREST, res_x_, res_y_, 0);
        // nre::gpu::set_texture_format(this->depthtexture, nre::gpu::DEPTHSTENCIL, nre::gpu::NEAREST,
        // res_x_, res_y_, 0);
        nre::gpu::set_renderbuffer_textype(this->depthrbo_, nre::gpu::DEPTHSTENCIL, res_x_, res_y_);
    }
    // update light texture
    // nre::gpu::set_texture_format(this->tex_light_, nre::gpu::RGBA, nre::gpu::NEAREST, res_x_,
    // res_y_, 0);

    // generate draw calls
    this->generateDrawCalls();

    // upload all remaining data to the GPU
    // and clean up any unused data
    this->updateMeshGPUData();
    this->updateMaterialGPUData();
    this->updateCameraGPUData();


    // render viewport
    nre::gpu::use_wireframe(this->use_wireframe_);

    /*cout << "NRE Cameras: " << data_.cameras_.size() << endl;
    cout << "NRE Materials: " << data_.materials_.size() << endl;
    cout << "NRE Meshes: " << data_.meshes_.size() << endl;
    cout << "NRE Vgroups: " << data_.vgroups_.size() << endl;*/

    nre::gpu::use_framebuffer(this->framebuffer_);
    nre::gpu::clear_framebuffer(this->framebuffer_, nre::gpu::FBO_ALL, 1.0f);

    if (data_.loaded_viewport_ != 0) {

        auto lv = data_.loaded_viewport_;

        if (data_.viewports_[lv].cameras.size() == 0) {
            throw nre::incomplete_viewport(lv);
        }
        // TEMPORARY until multiple layers and cameras support is implemented
        else if (data_.viewports_[lv].cameras.size() > 1) {
            throw nre::unsupported_viewport(lv, "Too many (" + to_string(data_.viewports_[lv].cameras.size()) + ") cameras");
        }
        else {
        }

        // only one fullscreen camera is supported for now
        auto camera_id = data_.viewports_[lv].cameras[0];
        auto scene_id  = data_.cameras_[camera_id].scene_id;

        // draw everything required for the z prepass, which also populates the depth buffer
        // NOTE: Automatically disabled if software vertex shaders are in use
        if (update_info.use_z_prepass and not this->use_software_vertex_shaders_) {
            nre::gpu::set_render_mode(nre::gpu::Z_PREPASS_BACKFACE);
            for (auto x : this->sce_ren_groups_[scene_id]) {
                if (x.camera == camera_id) {
                    this->drawRenderGroupZPrePass(x);
                    this->sce_ren_groups_[scene_id].replace(x);
                }
            }
            this->sce_ren_groups_[scene_id].update();
        }

        // sort and draw lights
        this->sortPointLights(scene_id, camera_id);

        // draw everything normally
        nre::gpu::use_framebuffer(this->framebuffer_);
        if (update_info.use_z_prepass and not this->use_software_vertex_shaders_) {
            nre::gpu::set_render_mode(nre::gpu::AFTERPREPASS_BACKFACE);
        }
        else {
            nre::gpu::set_render_mode(nre::gpu::REGULAR_BACKFACE);
        }

        // precalculate software vertex shader if necessary
        if (this->use_software_vertex_shaders_) {
            this->update_software_vertex_shaders(camera_id, scene_id);
        }

        for (auto x : this->sce_ren_groups_[scene_id]) {
            if (x.camera == camera_id) {
                this->drawRenderGroup(x);
                this->sce_ren_groups_[scene_id].replace(x);
            }
        }
        this->sce_ren_groups_[scene_id].update();

        // optionally draw a bounding box/sphere for each object (in wireframe mode)
        bool render_bboxes  = this->render_bounding_boxes_;
        bool render_spheres = this->render_bounding_spheres_;
        nre::gpu::use_wireframe(true);

        if (render_bboxes) {
            nre::gpu::set_render_mode(nre::gpu::REGULAR_BOTH);
            if (!this->setup_bbox_prog_) {

                this->setupBoundingBoxProgram();
                this->setup_bbox_prog_ = true;
            }
            for (auto x : this->sce_ren_groups_[scene_id]) {
                if (x.camera == camera_id) {
                    this->drawRenderGroupBoundingBox(x);
                    this->sce_ren_groups_[scene_id].replace(x);
                }
            }
            this->sce_ren_groups_[scene_id].update();
        }
        if (render_spheres) {
            nre::gpu::set_render_mode(nre::gpu::REGULAR_BOTH);
            if (!this->setup_sphere_prog_) {

                this->setupBoundingSphereProgram();
                this->setup_sphere_prog_ = true;
            }
            for (auto x : this->sce_ren_groups_[scene_id]) {
                if (x.camera == camera_id) {
                    this->drawRenderGroupBoundingSphere(x);
                    this->sce_ren_groups_[scene_id].replace(x);
                }
            }
            this->sce_ren_groups_[scene_id].update();
        }
    }

    // nre::gpu::copyFrameBuffer(this->framebuffer_, 0, nre::gpu::FBO_DEPTHSTENCIL);

    nre::gpu::use_framebuffer(0);
    nre::gpu::use_wireframe(false);
    nre::gpu::set_render_mode(nre::gpu::FULLSCREEN_QUAD);

    nre::gpu::set_texture_slot(this->colortexture_, 0);

    nre::gpu::draw(this->gamma_cor_prog_, this->vao_fullscreen_quad_);

    nre::gpu::discard_framebuffer(this->framebuffer_);
    return true;
}

void nre::renderer_legacy_t::drawRenderGroup(nre::render_group& ren_group) {

    if (!ren_group.isSetup) {
        // cout << "Setting up Render group" << ren_group.camera << " " << ren_group.vgroup << " " << ren_group.mesh << endl;

        ren_group.isSetup = true;

        ren_group.vs = nre::gpu::vertex_shader_t();
        if (not this->use_software_vertex_shaders_) {
            ren_group.vs.type = nre::gpu::VS_REGULAR;
        }
        else {
            ren_group.vs.type = nre::gpu::VS_REGULAR_SOFTWARE;
        }
        ren_group.vs.num_of_uvs = data_.meshes_[ren_group.mesh].uvmaps;


        // choose shading mode
        ren_group.fs = nre::gpu::pixel_shader_t();
        lockMutex();
        switch (this->shading_mode_) {
        case oe::RENDERER_NORMALS_SHADING:
            ren_group.fs.type = nre::gpu::FS_NORMALS;
            break;
        case oe::RENDERER_NO_LIGHTS_SHADING:
            ren_group.fs.type = nre::gpu::FS_MATERIAL;
            break;
        case oe::RENDERER_DIR_LIGHTS_SHADING:

            break;
        case oe::RENDERER_INDEXED_LIGHTS_SHADING:

            break;
        case oe::RENDERER_REGULAR_SHADING:
            ren_group.fs.type = nre::gpu::FS_MATERIAL;
            break;
        }
        unlockMutex();
        ren_group.fs.num_of_uvs = data_.meshes_[ren_group.mesh].uvmaps;

        ren_group.program = nre::gpu::new_program();
        nre::gpu::set_program_vertex_shader(ren_group.program, ren_group.vs);
        nre::gpu::set_program_pixel_shader(ren_group.program, ren_group.fs);

        nre::gpu::setup_program(ren_group.program);
    }


    auto mvp_mat   = data_.cameras_[ren_group.camera].perspective_view_mat * data_.meshes_[ren_group.mesh].model_mat;
    auto model_mat = data_.meshes_[ren_group.mesh].model_mat;
    // SOFTWARE VERTEX SHADER PREPROCESSING
    if (not this->use_software_vertex_shaders_) {
        nre::gpu::set_program_uniform_data(ren_group.program, "Model_Matrix", OE_Mat4x4ToSTDVector(model_mat));
        nre::gpu::set_program_uniform_data(ren_group.program, "MVP_Matrix", OE_Mat4x4ToSTDVector(mvp_mat));
    }


    if (nre::gpu::get_program_uniform_slot(ren_group.program, "mat_diffuse") != -2) {
        nre::gpu::set_program_uniform_data(ren_group.program, "mat_diffuse",
                                           OE_Vec4ToSTDVector(data_.materials_[ren_group.material].get_mat_diffuse()));
    }
    if (nre::gpu::get_program_uniform_slot(ren_group.program, "camera_pos") != -2) {
        nre::gpu::set_program_uniform_data(ren_group.program, "camera_pos",
                                           OE_Vec4ToSTDVector(data_.cameras_[ren_group.camera].get_position()));
    }
    if (nre::gpu::get_program_uniform_slot(ren_group.program, "mat_specular_hardness") != -2) {
        nre::gpu::set_program_uniform_data(ren_group.program, "mat_specular_hardness",
                                           data_.materials_[ren_group.material].get_mat_specular_hardness());
    }

    nre::gpu::draw(ren_group.program, data_.meshes_[ren_group.mesh].vao, data_.vgroups_[ren_group.vgroup].ibo);
}

void nre::renderer_legacy_t::drawRenderGroupZPrePass(nre::render_group& ren_group) {

    if (!ren_group.isZPrePassSetup) {

        ren_group.isZPrePassSetup = true;

        ren_group.vs_z_prepass            = nre::gpu::vertex_shader_t();
        ren_group.vs_z_prepass.type       = nre::gpu::VS_Z_PREPASS;
        ren_group.vs_z_prepass.num_of_uvs = data_.meshes_[ren_group.mesh].uvmaps;

        ren_group.z_prepass_program = nre::gpu::new_program();

        nre::gpu::set_program_vertex_shader(ren_group.z_prepass_program, ren_group.vs_z_prepass);

        nre::gpu::setup_program(ren_group.z_prepass_program);
    }
    nre::gpu::set_program_uniform_data(
        ren_group.z_prepass_program, "MVP_Matrix",
        OE_Mat4x4ToSTDVector(data_.cameras_[ren_group.camera].perspective_view_mat * data_.meshes_[ren_group.mesh].model_mat));
    nre::gpu::draw(ren_group.z_prepass_program, data_.meshes_[ren_group.mesh].vao, data_.vgroups_[ren_group.vgroup].ibo);
}

void nre::renderer_legacy_t::drawRenderGroupBoundingBox(nre::render_group& ren_group) {

    nre::gpu::set_program_uniform_data(this->prog_bbox_, "Model_Matrix", data_.meshes_[ren_group.mesh].data);
    nre::gpu::set_program_uniform_data(this->prog_bbox_, "PV_Matrix",
                                       OE_Mat4x4ToSTDVector(data_.cameras_[ren_group.camera].perspective_view_mat));
    nre::gpu::set_program_uniform_data(this->prog_bbox_, "scaling_min_data",
                                       data_.meshes_[ren_group.mesh].get_scaling_min_data());
    nre::gpu::set_program_uniform_data(this->prog_bbox_, "scaling_max_data",
                                       data_.meshes_[ren_group.mesh].get_scaling_max_data());
    nre::gpu::draw(this->prog_bbox_, this->vao_bbox_);
}

void nre::renderer_legacy_t::drawRenderGroupBoundingSphere(nre::render_group& ren_group) {

    nre::gpu::set_program_uniform_data(this->prog_sphere_, "Model_Matrix", data_.meshes_[ren_group.mesh].data);
    nre::gpu::set_program_uniform_data(this->prog_sphere_, "PV_Matrix",
                                       OE_Mat4x4ToSTDVector(data_.cameras_[ren_group.camera].perspective_view_mat));
    nre::gpu::set_program_uniform_data(this->prog_sphere_, "scaling_min_data",
                                       data_.meshes_[ren_group.mesh].get_scaling_min_data());
    nre::gpu::set_program_uniform_data(this->prog_sphere_, "scaling_max_data",
                                       data_.meshes_[ren_group.mesh].get_scaling_max_data());
    nre::gpu::draw(this->prog_sphere_, this->vao_sphere_, this->ibo_sphere_);
}


void nre::renderer_legacy_t::setupBoundingBoxProgram() {

    // the same vbo and vao are used for every object
    // The vbo holds a cube with side 1 that is scaled appropriately in the shader
    // through the uniform block variable 'scaling_data'

    this->vbo_bbox_ = nre::gpu::new_vertex_buf();
    auto bbox_data  = OE_GetBoundingBoxVertexBuffer(1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f);
    nre::gpu::set_vertex_buf_memory_and_data(this->vbo_bbox_, bbox_data, nre::gpu::STATIC);

    typedef nre::gpu::vertex_layout_input VLI; // for clarity
    this->vao_bbox_ = nre::gpu::new_vertex_layout();

    std::vector<VLI> vao_bbox__data;
    vao_bbox__data.push_back(VLI(this->vbo_bbox_, 0, 3, 6));
    vao_bbox__data.push_back(VLI(this->vbo_bbox_, 3, 3, 6));

    nre::gpu::set_vertex_layout_format(this->vao_bbox_, vao_bbox__data);

    this->prog_bbox_ = nre::gpu::new_program();

    nre::gpu::vertex_shader_t vs_bbox;
    nre::gpu::pixel_shader_t  fs_bbox;

    vs_bbox.type       = nre::gpu::VS_BOUNDING_BOX;
    vs_bbox.num_of_uvs = 0;

    fs_bbox.type       = nre::gpu::FS_NORMALS;
    fs_bbox.num_of_uvs = 0;

    nre::gpu::set_program_vertex_shader(this->prog_bbox_, vs_bbox);
    nre::gpu::set_program_pixel_shader(this->prog_bbox_, fs_bbox);

    nre::gpu::setup_program(this->prog_bbox_);
}

void nre::renderer_legacy_t::setupBoundingSphereProgram() {

    this->prog_sphere_ = nre::gpu::new_program();

    nre::gpu::vertex_shader_t vs_sphere;
    nre::gpu::pixel_shader_t  fs_sphere;

    vs_sphere.type       = nre::gpu::VS_BOUNDING_SPHERE;
    vs_sphere.num_of_uvs = 0;

    fs_sphere.type       = nre::gpu::FS_NORMALS;
    fs_sphere.num_of_uvs = 0;

    nre::gpu::set_program_vertex_shader(this->prog_sphere_, vs_sphere);
    nre::gpu::set_program_pixel_shader(this->prog_sphere_, fs_sphere);

    nre::gpu::setup_program(this->prog_sphere_);
}

//---------------------------------Update actual GPU data------------------------------//

void nre::renderer_legacy_t::updateMeshGPUData() {
    for (auto mesh : data_.meshes_) {

        // first time buffer initialization
        if (!data_.meshes_[mesh.first].has_init) {
            data_.meshes_[mesh.first].vbo      = nre::gpu::new_vertex_buf();
            data_.meshes_[mesh.first].vao      = nre::gpu::new_vertex_layout();
            data_.meshes_[mesh.first].has_init = true;
        }


        // initialize data itself
        if (!data_.meshes_[mesh.first].vao_initialized) {

            /// vertex buffer
            if (!this->use_software_vertex_shaders_) {
                nre::gpu::set_vertex_buf_memory_and_data(data_.meshes_[mesh.first].vbo, data_.meshes_[mesh.first].vbo_data,
                                                         nre::gpu::STATIC);
            }
            else {
                long int offset          = 6 + data_.meshes_[mesh.first].uvmaps * 2;
                long int num_of_vertices = data_.meshes_[mesh.first].vbo_data.size() / offset;
                nre::gpu::set_vertex_buf_memory(data_.meshes_[mesh.first].vbo, num_of_vertices * (offset + 4),
                                                nre::gpu::STREAM);
            }

            /// index buffers
            for (auto vg : mesh.second.vgroups) {
                data_.vgroups_[vg].ibo = nre::gpu::new_index_buf();
                nre::gpu::set_index_buf_memory_and_data(data_.vgroups_[vg].ibo, data_.meshes_[mesh.first].ibos_data[vg],
                                                        nre::gpu::STATIC);
            }

            /// vertex layout
            typedef nre::gpu::vertex_layout_input VLI; // for clarity

            // delete data_.meshes_[mesh.first].mesh->data->index_buffer;
            // data_.meshes_[mesh.first].mesh->data->index_buffer = nullptr;

            data_.meshes_[mesh.first].vao_input.clear();

            int offset = 6;

            if (this->use_software_vertex_shaders_) {
                offset = offset + 4;
            }

            data_.meshes_[mesh.first].vao_input.push_back(
                VLI(data_.meshes_[mesh.first].vbo, 0, 3, offset + data_.meshes_[mesh.first].uvmaps * 2));
            data_.meshes_[mesh.first].vao_input.push_back(
                VLI(data_.meshes_[mesh.first].vbo, 3, 3, offset + data_.meshes_[mesh.first].uvmaps * 2));

            if (this->use_software_vertex_shaders_) {
                data_.meshes_[mesh.first].vao_input.push_back(
                    VLI(data_.meshes_[mesh.first].vbo, 6, 4, offset + data_.meshes_[mesh.first].uvmaps * 2));
            }

            for (size_t i = 0; i < data_.meshes_[mesh.first].uvmaps; i++) {
                data_.meshes_[mesh.first].vao_input.push_back(
                    VLI(data_.meshes_[mesh.first].vbo, offset + 2 * i, 2, offset + data_.meshes_[mesh.first].uvmaps * 2));
            }

            nre::gpu::set_vertex_layout_format(data_.meshes_[mesh.first].vao, data_.meshes_[mesh.first].vao_input);
            data_.meshes_[mesh.first].vao_initialized = true;
        }

        // update per frame data
        if (data_.meshes_[mesh.first].changed) {


            data_.meshes_[mesh.first].data = OE_Mat4x4ToSTDVector(data_.meshes_[mesh.first].model_mat);

            // populate scaling_max_data
            data_.meshes_[mesh.first].data.push_back(data_.meshes_[mesh.first].max_x);
            data_.meshes_[mesh.first].data.push_back(data_.meshes_[mesh.first].max_y);
            data_.meshes_[mesh.first].data.push_back(data_.meshes_[mesh.first].max_z);

            // final element is sphere radius candidate
            data_.meshes_[mesh.first].data.push_back(data_.meshes_[mesh.first].max_radius);

            // populate scaling_min_data
            data_.meshes_[mesh.first].data.push_back(data_.meshes_[mesh.first].min_x);
            data_.meshes_[mesh.first].data.push_back(data_.meshes_[mesh.first].min_y);
            data_.meshes_[mesh.first].data.push_back(data_.meshes_[mesh.first].min_z);

            // final element is sphere radius candidate
            data_.meshes_[mesh.first].data.push_back(data_.meshes_[mesh.first].min_radius);

            if (data_.meshes_[mesh.first].size != data_.meshes_[mesh.first].data.size()) {
                data_.meshes_[mesh.first].size = data_.meshes_[mesh.first].data.size();
            }

            data_.meshes_[mesh.first].changed = false;
        }
    }

    for (auto cam : data_.deleted_meshes_) {
        this->sce_ren_groups_[data_.meshes_[cam].scene_id].removeMesh(cam);
        data_.delete_mesh(cam);
    }
    data_.deleted_meshes_.clear();
}

void nre::renderer_legacy_t::updateMaterialGPUData() {
    for (auto mat : data_.materials_) {

        if (!data_.materials_[mat.first].has_init) {
            data_.materials_[mat.first].has_init = true;
        }

        if (data_.materials_[mat.first].changed) {

            if (data_.materials_[mat.first].size != data_.materials_[mat.first].data.size()) {
                data_.materials_[mat.first].size = data_.materials_[mat.first].data.size();
            }
            data_.materials_[mat.first].changed = false;
        }
    }

    for (auto cam : data_.deleted_materials_) {
        this->sce_ren_groups_[data_.materials_[cam].scene_id].removeMaterial(cam);
        data_.delete_material(cam);
    }
    data_.deleted_materials_.clear();
}

void nre::renderer_legacy_t::updateCameraGPUData() {
    for (auto cam : data_.cameras_) {

        if (!data_.cameras_[cam.first].has_init) {
            data_.cameras_[cam.first].has_init = true;
        }
        if (data_.cameras_[cam.first].changed) {

            if (data_.cameras_[cam.first].size != data_.cameras_[cam.first].data.size()) {
                data_.cameras_[cam.first].size = data_.cameras_[cam.first].data.size();
            }
            data_.cameras_[cam.first].changed = false;
        }
    }

    for (auto cam : data_.deleted_cameras_) {
        this->sce_ren_groups_[data_.cameras_[cam].scene_id].removeCamera(cam);
        data_.delete_camera(cam);
    }
    data_.deleted_cameras_.clear();
}
void nre::renderer_legacy_t::updateLightGPUData() {
    /*for (auto l: data_.pt_lights){

        if (!data_.pt_lights[l.first].has_init){
            data_.pt_lights[l.first].ubo = nre::gpu::new_uniform_buf();
            data_.pt_lights[l.first].has_init = true;
        }
        if (data_.pt_lights[l.first].changed){

            if (data_.pt_lights[l.first].size != data_.pt_lights[l.first].data.size()){
                data_.pt_lights[l.first].size = data_.pt_lights[l.first].data.size();
                nre::gpu::set_uniform_buf_memory(data_.pt_lights[l.first].ubo, data_.pt_lights[l.first].size,
    nre::gpu::DYNAMIC);
            }

            nre::gpu::set_uniform_buf_data(data_.pt_lights[l.first].ubo, data_.pt_lights[l.first].data, 0);
            data_.pt_lights[l.first].changed = false;
        }
    }*/
    std::vector<float> pt_light_data;
    pt_light_data.reserve(16 * 255);
    int count = 0;
    for (auto l : this->pt_visible_lights_) {
        pt_light_data.insert(pt_light_data.end(), data_.pt_lights_[l.id].data.begin(), data_.pt_lights_[l.id].data.end());

        count++;
        if (count > 255) break;
    }

    // fill the rest with zeros since webgl complains otherwise
    for (int i = 0; i < 16 * (255 - count); i++) {
        pt_light_data.push_back(0.0f);
    }
}

void nre::renderer_legacy_t::destroy() {

    this->setup_bbox_prog_   = false;
    this->setup_sphere_prog_ = false;

    data_.clear();
    this->sce_ren_groups_.clear();

    nre::gpu::destroy();
}
