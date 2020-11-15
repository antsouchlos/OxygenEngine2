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
    if (!this->screen->isES){
        NRE_GPU_ShaderBase::init(NRE_GPU_GL, this->screen->major, this->screen->minor);
        if (this->screen->major == 3 && this->screen->minor == 1){
            this->gl_shader_prefix = "#version 140 \n"
                                 "#extension GL_ARB_explicit_attrib_location"
                                 ": require\n";
        } else{
            this->gl_shader_prefix = "#version 150 \n"
                                 "#extension GL_ARB_explicit_attrib_location"
                                 ": require\n";
        }
        
    } else{
        NRE_GPU_ShaderBase::init(NRE_GPU_GLES, this->screen->major, this->screen->minor);
        this->gl_shader_prefix = "#version 300 es \nprecision highp float; \n";
    }
    
    return true;
}

bool NRE_Renderer::updateSingleThread(){
    
    // upload mesh data to the GPU
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
    
    //upload material data to the GPU
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
    
    //upload camera data to the GPU
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
    
    // draw everything
    for (auto &x: this->render_groups){
        this->drawRenderGroup(&x);
    }
    return true;
}

void NRE_Renderer::drawRenderGroup(NRE_RenderGroup *ren_group){
    
    if (!ren_group->isSetup){
       //cout << "Setting up Render group" << ren_group->camera << " " << ren_group->vgroup << " " << ren_group->mesh << endl;
        ren_group->program = this->api->newProgram();
        ren_group->isSetup = true;
        /*this->api->setProgramVS(ren_group->program, string(gl_shader_prefix + string(NRE_Shader(
            
            layout (location=0) in vec3 oe_position;
            layout (location=1) in vec3 oe_normals;
            
            out vec3 position;
            out vec3 normals;
            
            layout(std140) uniform OE_Camera{
                mat4 PV_Matrix;
            };
            
            layout(std140) uniform OE_Mesh32{
                mat4 Model_Matrix;
            };
            
            void main(){
                normals = mat3(Model_Matrix)*oe_normals;
                mat4 final_mat = PV_Matrix*Model_Matrix;
                
                vec4 temp_position = Model_Matrix*vec4(oe_position, 1.0);
                position = temp_position.xyz;
                gl_Position = final_mat*vec4(oe_position, 1.0);
            }
            
        ))));
        this->api->setProgramFS(ren_group->program,  string(gl_shader_prefix +  string(NRE_Shader(
            
            in vec3 position;
            in vec3 normals;
            
            out vec4 fragColor;
            
            layout (std140) uniform OE_Material{
                vec4 mat_diffuse;
                vec4 mat_specular;
                float mat_specular_intensity;
                float mat_specular_hardness;
                float mat_translucency;
                float mat_illuminosity;
            };
            
            const vec3 light_pos = vec3( 13.37035561, -12.76134396, 10.10574818);
            
            void main(){
                
                vec3 s = normalize(light_pos-position);
                vec3 v = normalize(-position);
                
                vec3 specular = vec3(0.0);
                
                float sDotN = abs(dot(s, normals));
                
                if (sDotN > 0.0){
                    //specular = vec3(0.5);
                    specular = vec3(pow(sDotN,2.0/ mat_specular_hardness));
                }
                
                vec3 dif_output = clamp(mat_diffuse.rgb*max(sDotN, 0.2), 0.01, 1.0);
                
                //fragColor = vec4(abs(normals), 1.0);
                fragColor = vec4(dif_output, 1.0);
            }
            
        ))));*/
        ren_group->vs = NRE_GPU_VertexShader();
        ren_group->vs.type = NRE_GPU_VS_REGULAR;
        ren_group->vs.num_of_uvs = this->meshes[ren_group->mesh].uvmaps;
        
        ren_group->fs = NRE_GPU_PixelShader();
        ren_group->fs.type = NRE_GPU_FS_MATERIAL;
        ren_group->fs.num_of_uvs = this->meshes[ren_group->mesh].uvmaps;
        
        this->api->setProgramVS(ren_group->program, ren_group->vs.genShader());
        this->api->setProgramFS(ren_group->program, ren_group->fs.genShader());
        
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


bool NRE_Renderer::updateMultiThread(OE_Task*, int){
    return false;
}

void NRE_Renderer::destroy(){
    if (api != nullptr)
        delete api;
}    
