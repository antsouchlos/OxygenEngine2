#include <Renderer/NRE_GL3_Shaders.h>
#include <iostream>

using namespace std;

std::string NRE_GenGL3VertexShader(NRE_GPU_VertexShader vs){
    
    std::string output = "\n";
    
    if (NRE_GPU_ShaderBase::backend == NRE_GPU_GLES){
        output.append("precision highp float; \n");
    }
    
    output.append("invariant gl_Position;\n");
    
    if (vs.fullscreenQuad){
        output = NRE_Shader(
            layout (location=0) in vec2 oe_position;
            
            out vec2 position;
            
            void main(){
                position = oe_position;
                gl_Position = vec4(oe_position, 0.0, 1.0);
            }
        );
    }
    else {
        
        if (vs.type == NRE_GPU_VS_Z_PREPASS){
            output = NRE_Shader(
                layout (location = 0) in vec3 oe_position;
                
                layout(std140) uniform OE_Camera{
                    mat4 PV_Matrix;
                    vec4 camera_pos;
                };
            
                layout(std140) uniform OE_Mesh32{
                    mat4 Model_Matrix;
                    vec4 scaling_max_data;
                    vec4 scaling_min_data;
                };
                
                void main(){
                    mat4 final_mat = PV_Matrix*Model_Matrix;
                    gl_Position = final_mat*vec4(oe_position, 1.0);
                }
            );
        } 
        else if ((vs.type == NRE_GPU_VS_REGULAR) || (vs.type == NRE_GPU_VS_BOUNDING_BOX) || (vs.type == NRE_GPU_VS_BOUNDING_SPHERE)){
            
            // setup inputs
            output = NRE_Shader(
                layout (location = 0) in vec3 oe_position;
                layout (location = 1) in vec3 oe_normals;
            );
            output.append("\n");
            for (size_t i=0; i< vs.num_of_uvs; i++){
                output.append("layout (location=" + to_string(2+i) + ") in vec2 oe_uvs" + to_string(i) + ";");
                output.append("\n");
            }
            
            // setup outputs
            output.append(NRE_Shader(
                out vec3 position;
                out vec3 normals;
            ));
            output.append("\n");
            for (size_t i=0; i< vs.num_of_uvs; i++){
                output.append("out vec2 uvs" + to_string(i) + ";");
                output.append("\n");
            }
            
            // setup uniforms
            output.append(NRE_Shader(
                layout(std140) uniform OE_Camera{
                    mat4 PV_Matrix;
                    vec4 camera_pos;
                };
            
                layout(std140) uniform OE_Mesh32{
                    mat4 Model_Matrix;
                    vec4 scaling_max_data;
                    vec4 scaling_min_data;
                };
            ));
            
            // setup final logic
            if (vs.type == NRE_GPU_VS_REGULAR){
                
                // handle regular case
                output.append(NRE_Shader(
                    void main(){
                        normals = mat3(Model_Matrix)*oe_normals;
                        mat4 final_mat = PV_Matrix*Model_Matrix;
                
                        vec4 temp_position = Model_Matrix*vec4(oe_position, 1.0);
                        position = temp_position.xyz;
                        gl_Position = final_mat*vec4(oe_position, 1.0);
                    }
                ));
            }
            else if (vs.type == NRE_GPU_VS_BOUNDING_BOX) {
                
                // handle bounding box case
                // here one does not need to consider rotation, since the (non-naive) bounding box
                // already accounts for rotation, but needs to handle scale
                output.append(NRE_Shader(
                    void main(){
                        normals = oe_normals;
                        mat4 final_mat = PV_Matrix;
                        
                        vec3 delta_pos = vec3(Model_Matrix[3][0], Model_Matrix[3][1], Model_Matrix[3][2]);
                        vec4 temp_position = vec4(oe_position*scaling_max_data.xyz+delta_pos, 1.0);
                        if (oe_position.x*abs(oe_position.x) < 0.0){
                            temp_position.x = abs(oe_position.x)*scaling_min_data.x+delta_pos.x;
                        }
                        if (oe_position.y*abs(oe_position.y) < 0.0){
                            temp_position.y = abs(oe_position.y)*scaling_min_data.y+delta_pos.y;
                        }
                        if (oe_position.z*abs(oe_position.z) < 0.0){
                            temp_position.z = abs(oe_position.z)*scaling_min_data.z+delta_pos.z;
                        }
                        
                        position = temp_position.xyz;
                        gl_Position = final_mat*temp_position;
                    }
                ));
            }
            else if (vs.type == NRE_GPU_VS_BOUNDING_SPHERE) {
                
                // handle bounding sphere case
                // here one does not need to consider rotation, since the sphere
                // already accounts for rotation, but needs appropriate scaling
                                
                output.append(NRE_Shader(
                    void main(){
                        normals = oe_normals;
                        mat4 final_mat = PV_Matrix;
                        
                        vec3 delta_pos = vec3(Model_Matrix[3][0], Model_Matrix[3][1], Model_Matrix[3][2]);
                        vec4 temp_position = vec4(oe_position*max(abs(scaling_max_data.w), abs(scaling_min_data.w))+delta_pos, 1.0);
                        
                        position = temp_position.xyz;
                        gl_Position = final_mat*temp_position;
                    }
                ));
            }
            else {
            }
        }
        
    }

    
    
    return NRE_GPU_ShaderBase::shader_prefix + output;
}

std::string NRE_GenGL3PixelShader(NRE_GPU_PixelShader fs){
    
    std::string output = "\n";
    
    if (NRE_GPU_ShaderBase::backend == NRE_GPU_GLES){
        output.append("precision mediump float; \n");
    }
    
    if ((fs.type == NRE_GPU_FS_UNDEFINED) or (fs.type == NRE_GPU_FS_SIMPLE)){
        output.append(NRE_Shader(
            in vec2 position;
            out vec4 fragColor;
            
            void main(){
                fragColor = vec4(vec3(0.5), 1.0);
            }
        ));
        return NRE_GPU_ShaderBase::shader_prefix + output;
    }
    else if (fs.type == NRE_GPU_FS_GAMMA){
        output.append(NRE_Shader(
            in vec2 position;
            out vec4 fragColor;
            
            uniform sampler2D tex_output;
            
            void main(){
                fragColor = pow(abs(texture(tex_output, position/2.0 +0.5)), vec4(1.0/2.2));
            }
        ));
        return NRE_GPU_ShaderBase::shader_prefix + output;
    }
    
    output.append(NRE_Shader(
        in vec3 position;
        in vec3 normals;
    ));
    output.append("\n");
    
    // setup inputs
    for (size_t i=0; i< fs.num_of_uvs; i++){
        output.append("in vec2 uvs" + to_string(i) + ";");
        output.append("\n");
    }
    
    // setup uniforms
    
    output.append(NRE_Shader(
        layout (std140) uniform OE_Material{
            vec4 mat_diffuse;
            vec4 mat_specular;
            float mat_specular_intensity;
            float mat_specular_hardness;
            float mat_translucency;
            float mat_illuminosity;
        };
    ));
    output.append("\n");
    
    if (fs.type == NRE_GPU_FS_MATERIAL){
        
        // setup uniforms
        output.append(NRE_Shader(
            layout(std140) uniform OE_Camera{
                mat4 PV_Matrix;
                vec4 camera_pos;
            };
            
            layout(std140) uniform OE_Mesh32{
                mat4 Model_Matrix;
                vec4 scaling_max_data;
                vec4 scaling_min_data;
            };
            
            out vec4 fragColor;
        ));
        output.append("\n");
        
        
            
        output.append(NRE_Shader(
            
            void main(){
                
                vec3 light_pos = vec3( camera_pos);
                
                vec3 s = normalize(light_pos-position);
                vec3 v = normalize(-position);
                
                vec3 specular = vec3(0.0);
                
                float sDotN = abs(dot(s, normals));
                
                if (sDotN > 0.0){
                    //specular = vec3(0.5);
                    specular = vec3(pow(sDotN,2.0/ mat_specular_hardness));
                }
                
                vec3 dif_output = clamp(mat_diffuse.rgb*max(sDotN, 0.2), 0.0, 1.0);
                
                //fragColor = vec4(abs(normals), 1.0);
                fragColor = vec4(dif_output, 1.0);
            }
        ));
        
    }
    else if (fs.type == NRE_GPU_FS_NORMALS){
        
            output.append(NRE_Shader(
            
            out vec4 fragColor;
            
            //const vec3 light_pos = vec3( 13.37035561, -12.76134396, 10.10574818);
            
            void main(){
                
                fragColor = vec4(abs(normals), 1.0);
            }
            ));
        
    }
    
    return NRE_GPU_ShaderBase::shader_prefix + output;
}
