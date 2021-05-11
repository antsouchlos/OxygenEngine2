#include <OE_Math.h>
#include <Renderer/NRE_RendererMain.h>
#include <Renderer/NRE_GL3_API.h>
#include <iostream>

using namespace std;

NRE_Renderer::NRE_Renderer(){
    
}

NRE_Renderer::~NRE_Renderer(){
   
}
    
bool NRE_Renderer::init(){
    
    
    // make sure there are no stored objects
    /*cout << "NRE Cameras: " << this->cameras.size() << endl;
    cout << "NRE Materials: " << this->materials.size() << endl;
    cout << "NRE Meshes: " << this->meshes.size() << endl;
    cout << "NRE Vgroups: " << this->vgroups.size() << endl;*/
    
    
    this->cameras.clear();
    this->materials.clear();
    this->vgroups.clear();
    this->meshes.clear();
    this->dir_lights.clear();
    this->pt_lights.clear();
    this->scenes.clear();
    this->viewports.clear();
    
    this->setup_bbox_prog = false;
    this->setup_sphere_prog = false;
    
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
    
    this->initOffscreenFrameBuffer();
    this->initFullscreenQuad();
    this->initGammaCorrectionProg();
    this->initGPUSphere();
    this->initLightUBOProgramFBO();    
    
    return true;
}

void NRE_Renderer::initOffscreenFrameBuffer(){
    // setup offscreen framebuffer
    this->framebuffer = this->api->newFrameBuffer();
    this->colortexture = this->api->newTexture();
    this->depthtexture = this->api->newTexture();
    
    if (this->use_HDR.load(std::memory_order_relaxed) == false){
        this->api->setTextureFormat(this->colortexture, NRE_GPU_RGB10_A2, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
        this->api->setTextureFormat(this->depthtexture, NRE_GPU_DEPTHSTENCIL, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
    } else {
        this->api->setTextureFormat(this->colortexture, NRE_GPU_RGBA16F, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
        this->api->setTextureFormat(this->depthtexture, NRE_GPU_DEPTHSTENCIL, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
    }
    
    this->api->setFrameBufferTexture(this->framebuffer, this->colortexture, 0);
    this->api->setFrameBufferTexture(this->framebuffer, this->depthtexture, 0);
}

void NRE_Renderer::initFullscreenQuad(){
    // setup fullscreen quad
    this->vbo_fullscreen_quad = this->api->newVertexBuffer();
    this->vao_fullscreen_quad = this->api->newVertexLayout();
    
    this->api->setVertexBufferMemoryData(this->vbo_fullscreen_quad, {1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,-1.0f, 1.0f}, NRE_GPU_STATIC);
    
    typedef NRE_GPU_VertexLayoutInput VLI; // for clarity
    std::vector<VLI> vao_quad_data;
    vao_quad_data.push_back(VLI(this->vbo_fullscreen_quad, 0, 2, 2));
    this->api->setVertexLayoutFormat(this->vao_fullscreen_quad, vao_quad_data);
}

void NRE_Renderer::initGammaCorrectionProg(){
    // setup gamma correction program
    this->gamma_cor_prog = this->api->newProgram();
        
    NRE_GPU_VertexShader vs_gamma;
    NRE_GPU_PixelShader fs_gamma;
        
    vs_gamma.type = NRE_GPU_VS_UNDEFINED;
    vs_gamma.fullscreenQuad = true;
    fs_gamma.type = NRE_GPU_FS_GAMMA;
        
    this->api->setProgramVS(this->gamma_cor_prog, vs_gamma);
    this->api->setProgramFS(this->gamma_cor_prog, fs_gamma);
        
    this->api->setupProgram(this->gamma_cor_prog);
    this->api->setProgramTextureSlot(this->gamma_cor_prog, "tex_output", 0);
}

void NRE_Renderer::initGPUSphere(){
    // the same VBO, VAO and IBO are used for every object
    // it holds a sphere with radius one
    
    this->vbo_sphere = this->api->newVertexBuffer();
    this->ibo_sphere = this->api->newIndexBuffer();
    
    auto sphere_vbo_data = OE_GetBoundingSphereVertexBuffer(1.0f, 1.0f, 16);
    auto sphere_ibo_data = OE_GetBoundingSphereIndexBuffer(1.0f, 1.0f, 16);
    
    this->api->setVertexBufferMemoryData(this->vbo_sphere, sphere_vbo_data, NRE_GPU_STATIC);
    this->api->setIndexBufferMemoryData(this->ibo_sphere, sphere_ibo_data, NRE_GPU_STATIC);
    
    typedef NRE_GPU_VertexLayoutInput VLI; // for clarity
    this->vao_sphere = this->api->newVertexLayout();
    
    std::vector<VLI> vao_sphere_data;
    vao_sphere_data.push_back(VLI(this->vbo_sphere, 0, 3, 6));
    vao_sphere_data.push_back(VLI(this->vbo_sphere, 3, 3, 6));
    
    this->api->setVertexLayoutFormat(this->vao_sphere, vao_sphere_data);
}

void NRE_Renderer::initLightUBOProgramFBO(){
    
    // setup point light ubo
    this->pt_light_ubo = this->api->newUniformBuffer();
    
    // setup light program
    this->prog_light = this->api->newProgram();
        
    NRE_GPU_VertexShader vs_light;
    NRE_GPU_PixelShader fs_light;
        
    vs_light.type = NRE_GPU_VS_LIGHT;
    vs_light.num_of_uvs = 0;
        
    fs_light.type = NRE_GPU_FS_LIGHT_INDEX;
    fs_light.num_of_uvs = 0;
        
    this->api->setProgramVS(this->prog_light, vs_light);
    this->api->setProgramFS(this->prog_light, fs_light);
        
    this->api->setupProgram(this->prog_light);
    this->api->setProgramUniformBlockSlot(this->prog_light, "OE_Camera", 0);
    this->api->setProgramUniformBlockSlot(this->prog_light, "OE_Lights", 1);
    
    // setup light own texture and framebuffer to be used for storing the indices
    this->tex_light = this->api->newTexture();
    
    this->api->setTextureFormat(this->tex_light, NRE_GPU_RGBA, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
    
    this->fbo_light = this->api->newFrameBuffer();
    this->api->setFrameBufferTexture(this->fbo_light, this->tex_light, 0);
    this->api->setFrameBufferTexture(this->fbo_light, this->depthtexture, 0);
}

//------------------------updateSIngleThread-------------------//

bool NRE_Renderer::updateSingleThread(){
    
    this->api->update(this->screen->resolution_x, this->screen->resolution_y);
    
    if (this->use_HDR.load(std::memory_order_relaxed) == false){
        this->api->setTextureFormat(this->colortexture, NRE_GPU_RGB10_A2, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
        this->api->setTextureFormat(this->depthtexture, NRE_GPU_DEPTHSTENCIL, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
    } else {
        this->api->setTextureFormat(this->colortexture, NRE_GPU_RGBA16F, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
        this->api->setTextureFormat(this->depthtexture, NRE_GPU_DEPTHSTENCIL, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
    }
    // update light texture
    this->api->setTextureFormat(this->tex_light, NRE_GPU_RGBA, NRE_GPU_NEAREST, this->screen->resolution_x, this->screen->resolution_y, 0);
    
    // generate draw calls
    this->generateDrawCalls();
    
    // upload all remaining data to the GPU 
    // and clean up any unused data
    this->updateMeshGPUData();
    this->updateMaterialGPUData();
    this->updateCameraGPUData();
    
    
    // render viewport
    this->api->use_wireframe = this->use_wireframe.load(std::memory_order_relaxed);
    
    /*cout << "NRE Cameras: " << this->cameras.size() << endl;
    cout << "NRE Materials: " << this->materials.size() << endl;
    cout << "NRE Meshes: " << this->meshes.size() << endl;
    cout << "NRE Vgroups: " << this->vgroups.size() << endl;*/
    
    this->api->useFrameBuffer(this->framebuffer);
    this->api->clearFrameBuffer(this->framebuffer, NRE_GPU_FBO_ALL, 1.0f);

    if (this->loaded_viewport != 0){

        auto lv = this->loaded_viewport;
        
        if (this->viewports[lv].cameras.size() == 0){
            throw nre::incomplete_viewport(lv);
        }
        // TEMPORARY until multiple layers and cameras support is implemented
        else if (this->viewports[lv].cameras.size() > 1){
            throw nre::unsupported_viewport(lv, "Too many (" + to_string(this->viewports[lv].cameras.size()) + ") cameras");
        }
        else{
            
        }
        
        // only one fullscreen camera is supported for now
        auto camera_id = this->viewports[lv].cameras[0];
        auto scene_id = this->cameras[camera_id].scene_id;
        
        // draw everything required for the z prepass, which also populates the depth buffer
        this->api->setRenderMode(NRE_GPU_Z_PREPASS_BACKFACE);
        for (auto x: this->scenes[scene_id].render_groups){
            if (x.camera == camera_id){
                this->drawRenderGroupZPrePass(x);
                this->scenes[scene_id].render_groups.replace(x);
            }
        }
        this->scenes[scene_id].render_groups.update();
        
        //sort point lights
        this->sortPointLights(scene_id, camera_id);
        this->updateLightGPUData();
        
        this->api->useFrameBuffer(this->fbo_light);
        this->api->clearFrameBuffer(this->fbo_light, NRE_GPU_FBO_COLORSTENCIL, 0.0f);
        
        this->api->setRenderMode(NRE_GPU_LIGHT_PREPASS_2);
        
        this->api->setUniformBlockState(this->pt_light_ubo, this->prog_light, 1, 0, 0);
        this->api->setUniformBlockState(this->cameras[camera_id].ubo, this->prog_light, 0, 0, 0);        
        this->api->draw_instanced(this->prog_light, this->vao_sphere, this->ibo_sphere, this->pt_visible_lights.size());
        
        this->api->setRenderMode(NRE_GPU_LIGHT_AFTERPASS_RG);
        this->api->draw_instanced(this->prog_light, this->vao_sphere, this->ibo_sphere, this->pt_visible_lights.size());
        
        this->api->setRenderMode(NRE_GPU_LIGHT_AFTERPASS_BA);
        this->api->draw_instanced(this->prog_light, this->vao_sphere, this->ibo_sphere, this->pt_visible_lights.size());
        
        // draw everything normally
        this->api->useFrameBuffer(this->framebuffer);
        this->api->setRenderMode(NRE_GPU_AFTERPREPASS_BACKFACE);
        for (auto x: this->scenes[scene_id].render_groups){
            if (x.camera == camera_id){
                this->drawRenderGroup(x);
                this->scenes[scene_id].render_groups.replace(x);
            }
        }
        this->scenes[scene_id].render_groups.update();
    
        // optionally draw a bounding box/sphere for each object (in wireframe mode)
        bool temp = this->api->use_wireframe;
        bool render_bboxes = this->render_bounding_boxes.load(std::memory_order_relaxed);
        bool render_spheres = this->render_bounding_spheres.load(std::memory_order_relaxed);
        this->api->use_wireframe = true;
    
        if (render_bboxes){
            this->api->setRenderMode(NRE_GPU_REGULAR_BOTH);
            if (!this->setup_bbox_prog){
        
                this->setupBoundingBoxProgram();
                this->setup_bbox_prog = true;
            }
            for (auto x: this->scenes[scene_id].render_groups){
                if (x.camera == camera_id){
                    this->drawRenderGroupBoundingBox(x);
                    this->scenes[scene_id].render_groups.replace(x);
                }
            }
            this->scenes[scene_id].render_groups.update();
        }
        if (render_spheres){
            this->api->setRenderMode(NRE_GPU_REGULAR_BOTH);
            if (!this->setup_sphere_prog){
        
                this->setupBoundingSphereProgram();
                this->setup_sphere_prog = true;
            }
            for (auto x: this->scenes[scene_id].render_groups){
                if (x.camera == camera_id){
                    this->drawRenderGroupBoundingSphere(x);
                    this->scenes[scene_id].render_groups.replace(x);
                }
            }
            this->scenes[scene_id].render_groups.update();
        }
        
        //temporary TEST LIGHTS
        //this->api->setRenderMode(NRE_GPU_REGULAR_BOTH);

        

        ///////////////////*/
        
        this->api->use_wireframe = temp;
    }
    
    this->api->copyFrameBuffer(this->framebuffer, 0, NRE_GPU_FBO_DEPTHSTENCIL);
    
    this->api->useFrameBuffer(0);
    this->api->use_wireframe = false;
    this->api->setRenderMode(NRE_GPU_FULLSCREEN_QUAD);
    
    this->api->setTextureSlot(this->colortexture, 0);
    
    this->api->draw(this->gamma_cor_prog, this->vao_fullscreen_quad);
    
    return true;
}

void NRE_Renderer::sortPointLights(std::size_t scene_id, std::size_t camera_id){
    this->pt_visible_lights.clear();
    
    auto persp_mat = this->cameras[camera_id].perspective_mat;
    
    for (auto l: this->scenes[scene_id].pt_lights){
        
        // calculations happening in view space
        OE_Vec4 model_pos = OE_Vec4(this->pt_lights[l].model_mat[3].x, this->pt_lights[l].model_mat[3].y, this->pt_lights[l].model_mat[3].z, 1.0f);
        
        OE_Vec4 light_view_pos = this->cameras[camera_id].view_mat*model_pos;
            
        auto light_camview_radius = light_view_pos - OE_Vec4(this->pt_lights[l].range, this->pt_lights[l].range, this->pt_lights[l].range, 0.0f);
        
        bool is_behind_camera = light_camview_radius.z > (-this->cameras[camera_id].near);
        bool is_too_far_away = (light_camview_radius.z + 2*this->pt_lights[l].range) < (-this->cameras[camera_id].far);
        
        //calculations happening in clip space        
        auto light_clip_radius_max_x = OE_GetClipCoords(persp_mat, light_view_pos + OE_Vec4(this->pt_lights[l].range, 0.0f, 0.0f, 0.0f));
        
        auto light_clip_radius_min_x = OE_GetClipCoords(persp_mat, light_view_pos - OE_Vec4(this->pt_lights[l].range, 0.0f, 0.0f, 0.0f));
        
        auto light_clip_radius_max_y = OE_GetClipCoords(persp_mat, light_view_pos + OE_Vec4(0.0f, this->pt_lights[l].range, 0.0f, 0.0f));
        
        auto light_clip_radius_min_y = OE_GetClipCoords(persp_mat, light_view_pos - OE_Vec4(0.0f, this->pt_lights[l].range, 0.0f, 0.0f));
        
        bool is_too_left  = light_clip_radius_max_x.x < -1.02f;
        bool is_too_right = light_clip_radius_min_x.x > 1.02f;

        bool is_too_above = light_clip_radius_max_y.y < -1.02f;
        bool is_too_below = light_clip_radius_min_y.y > 1.02f;
        
        // only choose lights that have effects inside the view frustum.
        // This means that up to 254 lights in range of the camera will be active at once
        // This means the scene can have many many more lights overall. All that matters
        // is what's VISIBLE
        if ((not is_behind_camera) and (not is_too_far_away) and (not is_too_left) and (not is_too_right) and (not is_too_above) and (not is_too_below)){
        auto point_light = NRE_PointLightDrawCall(l, (this->cameras[camera_id].perspective_view_mat*this->pt_lights[l].model_mat[3])[2], 0);
        this->pt_visible_lights.insert(point_light);
        }
        
        // set limit to 255 lights
        // why you ask?
        // Well firstly 255 lights is already a LOT of lights inside the view frustum. More lights are not that good for performance
        // Secondly the minimum required OpenGL Uniform buffer limit can only contain 256 matrices,
        // so i can only place up to 255 lights (light with index 0 inside the shaders symbolizes the lack of lights)
        // Thirdly, spot lights will be handed in a separate pass with another texture
        if (this->pt_visible_lights.size() >= 255)
            break;
    }
    //cout << "pt size " << this->pt_visible_lights.size() << endl;
}

void NRE_Renderer::drawRenderGroup(NRE_RenderGroup &ren_group){
    
    if (!ren_group.isSetup){
       //cout << "Setting up Render group" << ren_group.camera << " " << ren_group.vgroup << " " << ren_group.mesh << endl;
        
        ren_group.isSetup = true;
        
        ren_group.vs = NRE_GPU_VertexShader();
        ren_group.vs.type = NRE_GPU_VS_REGULAR;
        ren_group.vs.num_of_uvs = this->meshes[ren_group.mesh].uvmaps;
        
        
        // choose shading mode
        ren_group.fs = NRE_GPU_PixelShader();
        lockMutex();
        switch (this->shading_mode){
            case OE_RENDERER_NORMALS_SHADING:
                ren_group.fs.type = NRE_GPU_FS_NORMALS;
                break;
            case OE_RENDERER_NO_LIGHTS_SHADING:
                 ren_group.fs.type = NRE_GPU_FS_MATERIAL;
                break;
            case OE_RENDERER_DIR_LIGHTS_SHADING:
                
                break;
            case OE_RENDERER_INDEXED_LIGHTS_SHADING:
                
                break;
            case OE_RENDERER_REGULAR_SHADING:
                ren_group.fs.type = NRE_GPU_FS_MATERIAL;
                break;
        }
        unlockMutex();
        ren_group.fs.num_of_uvs = this->meshes[ren_group.mesh].uvmaps;
        
        ren_group.program = this->api->newProgram();
        this->api->setProgramVS(ren_group.program, ren_group.vs);
        this->api->setProgramFS(ren_group.program, ren_group.fs);
        
        this->api->setupProgram(ren_group.program);
        this->api->setProgramUniformBlockSlot(ren_group.program, "OE_Camera", 0);
        this->api->setProgramUniformBlockSlot(ren_group.program, "OE_Mesh32", 1);
        if (this->api->getProgramUniformBlockSlot(ren_group.program, "OE_Material") != -2){
            this->api->setProgramUniformBlockSlot(ren_group.program, "OE_Material", 2);
        }
    }
    
    this->api->setUniformBlockState(this->meshes[ren_group.mesh].ubo, ren_group.program, 1, 0, 0);
    this->api->setUniformBlockState(this->cameras[ren_group.camera].ubo, ren_group.program, 0, 0, 0);
    this->api->setUniformBlockState(this->materials[ren_group.material].ubo, ren_group.program, 2, 0, 0);
    this->api->draw(ren_group.program, this->meshes[ren_group.mesh].vao, this->vgroups[ren_group.vgroup].ibo);
}

void NRE_Renderer::drawRenderGroupZPrePass(NRE_RenderGroup& ren_group){
    
    if (!ren_group.isZPrePassSetup){
        
        ren_group.isZPrePassSetup = true;
        
        ren_group.vs_z_prepass = NRE_GPU_VertexShader();
        ren_group.vs_z_prepass.type = NRE_GPU_VS_Z_PREPASS;
        ren_group.vs_z_prepass.num_of_uvs = this->meshes[ren_group.mesh].uvmaps;
        
        ren_group.z_prepass_program = this->api->newProgram();
        
        this->api->setProgramVS(ren_group.z_prepass_program, ren_group.vs_z_prepass);
        
        this->api->setupProgram(ren_group.z_prepass_program);
        this->api->setProgramUniformBlockSlot(ren_group.z_prepass_program, "OE_Camera", 0);
        this->api->setProgramUniformBlockSlot(ren_group.z_prepass_program, "OE_Mesh32", 1);
    }
    
    this->api->setUniformBlockState(this->meshes[ren_group.mesh].ubo, ren_group.z_prepass_program, 1, 0, 0);
    this->api->setUniformBlockState(this->cameras[ren_group.camera].ubo, ren_group.z_prepass_program, 0, 0, 0);
    this->api->draw(ren_group.z_prepass_program, this->meshes[ren_group.mesh].vao, this->vgroups[ren_group.vgroup].ibo);
    
}

void NRE_Renderer::drawRenderGroupBoundingBox(NRE_RenderGroup& ren_group){
     
    this->api->setUniformBlockState(this->meshes[ren_group.mesh].ubo, this->prog_bbox, 1, 0, 0);
    this->api->setUniformBlockState(this->cameras[ren_group.camera].ubo, this->prog_bbox, 0, 0, 0);        
    this->api->draw(this->prog_bbox, this->vao_bbox);
}

void NRE_Renderer::drawRenderGroupBoundingSphere(NRE_RenderGroup& ren_group){
    
    this->api->setUniformBlockState(this->meshes[ren_group.mesh].ubo, this->prog_sphere, 1, 0, 0);
    this->api->setUniformBlockState(this->cameras[ren_group.camera].ubo, this->prog_sphere, 0, 0, 0);        
    this->api->draw(this->prog_sphere, this->vao_sphere, this->ibo_sphere);
}


void NRE_Renderer::setupBoundingBoxProgram(){
    
    // the same vbo and vao are used for every object
    // The vbo holds a cube with side 1 that is scaled appropriately in the shader
    // through the uniform block variable 'scaling_data'
    
    this->vbo_bbox = this->api->newVertexBuffer();
    auto bbox_data = OE_GetBoundingBoxVertexBuffer(1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f);
    this->api->setVertexBufferMemoryData(this->vbo_bbox, bbox_data, NRE_GPU_STATIC);
    
    typedef NRE_GPU_VertexLayoutInput VLI; // for clarity
    this->vao_bbox = this->api->newVertexLayout();
    
    std::vector<VLI> vao_bbox_data;
    vao_bbox_data.push_back(VLI(this->vbo_bbox, 0, 3, 6));
    vao_bbox_data.push_back(VLI(this->vbo_bbox, 3, 3, 6));
    
    this->api->setVertexLayoutFormat(this->vao_bbox, vao_bbox_data);
    
    this->prog_bbox = this->api->newProgram();
        
    NRE_GPU_VertexShader vs_bbox;
    NRE_GPU_PixelShader fs_bbox;
        
    vs_bbox.type = NRE_GPU_VS_BOUNDING_BOX;
    vs_bbox.num_of_uvs = 0;
        
    fs_bbox.type = NRE_GPU_FS_NORMALS;
    fs_bbox.num_of_uvs = 0;
        
    this->api->setProgramVS(this->prog_bbox, vs_bbox);
    this->api->setProgramFS(this->prog_bbox, fs_bbox);
        
    this->api->setupProgram(this->prog_bbox);
    this->api->setProgramUniformBlockSlot(this->prog_bbox, "OE_Camera", 0);
    this->api->setProgramUniformBlockSlot(this->prog_bbox, "OE_Mesh32", 1);
    if (this->api->getProgramUniformBlockSlot(this->prog_bbox, "OE_Material") != -2){
        this->api->setProgramUniformBlockSlot(this->prog_bbox, "OE_Material", 2);
    }
}

void NRE_Renderer::setupBoundingSphereProgram(){
    
    this->prog_sphere = this->api->newProgram();
        
    NRE_GPU_VertexShader vs_sphere;
    NRE_GPU_PixelShader fs_sphere;
        
    vs_sphere.type = NRE_GPU_VS_BOUNDING_SPHERE;
    vs_sphere.num_of_uvs = 0;
        
    fs_sphere.type = NRE_GPU_FS_NORMALS;
    fs_sphere.num_of_uvs = 0;
        
    this->api->setProgramVS(this->prog_sphere, vs_sphere);
    this->api->setProgramFS(this->prog_sphere, fs_sphere);
        
    this->api->setupProgram(this->prog_sphere);
    this->api->setProgramUniformBlockSlot(this->prog_sphere, "OE_Camera", 0);
    this->api->setProgramUniformBlockSlot(this->prog_sphere, "OE_Mesh32", 1);
    if (this->api->getProgramUniformBlockSlot(this->prog_sphere, "OE_Material") != -2){
        this->api->setProgramUniformBlockSlot(this->prog_sphere, "OE_Material", 2);
    }
    
}

void NRE_Renderer::generateDrawCalls(){
    
    // delete all remaining draw call programs from deleted scenes
    for (auto &sce : deleted_scenes){
        for (auto ren_group : this->scenes[sce].render_groups){
        
            if (ren_group.z_prepass_program != 0)
                this->api->deleteProgram(ren_group.z_prepass_program);
            if (ren_group.program != 0)
                this->api->deleteProgram(ren_group.program);
        }
        this->scenes.erase(sce);
    }
    this->deleted_scenes.clear();
    
    for (auto &scene: scenes){
        
        // delete remaining draw call programs from existing scenes
        for (auto ren_group : scene.second.render_groups.to_be_deleted_){
        
            if (ren_group.z_prepass_program != 0)
                this->api->deleteProgram(ren_group.z_prepass_program);
            if (ren_group.program != 0)
                this->api->deleteProgram(ren_group.program);
        }
        scene.second.render_groups.cleanupPrograms();
        
        // gemerate draw calls anew
        for (auto cam : scene.second.cameras){
            for (auto mesh : scene.second.meshes){
                for (auto vgroup : this->meshes[mesh].vgroups){
                    auto render_data = NRE_RenderGroup();
                    render_data.camera = cam;
                    render_data.vgroup = vgroup;
                    render_data.mesh = mesh;
                    render_data.material = this->vgroups[vgroup].material_id;
                    if (!scene.second.existsRenderGroup(render_data)){
                        scene.second.render_groups.insert(render_data);
                    }
                }
            }
        }
    }
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
