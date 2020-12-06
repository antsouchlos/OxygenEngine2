#include <Renderer/NRE_GL3_Shaders.h>

using namespace std;

std::string NRE_GenGL3VertexShader(NRE_GPU_VertexShader vs){
    
    std::string output = "\n";
    
    if (vs.fullscreenQuad){
        output = NRE_Shader(
            layout (location=0) in vec3 oe_position;
            
            out vec4 position;
            
            void main(){
                position = vec4(oe_position, 1.0, 1.0);
            }
        );
    }
    else {
        
        if (vs.type == NRE_GPU_VS_Z_PREPASS){
            output = NRE_Shader(
                layout (location = 0) in vec3 oe_position;
                
                layout(std140) uniform OE_Camera{
                    mat4 PV_Matrix;
                };
            
                layout(std140) uniform OE_Mesh32{
                    mat4 Model_Matrix;
                };
                
                void main(){
                    mat4 final_mat = PV_Matrix*Model_Matrix;
                    gl_Position = final_mat*vec4(oe_position, 1.0);
                }
            );
        } 
        else if (vs.type == NRE_GPU_VS_REGULAR){
            
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
                };
            
                layout(std140) uniform OE_Mesh32{
                    mat4 Model_Matrix;
                };
            ));
            
            // setup final logic
            
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
    }

    
    
    return NRE_GPU_ShaderBase::shader_prefix + output;
}

std::string NRE_GenGL3PixelShader(NRE_GPU_PixelShader fs){
    
    std::string output = "\n";
    
    if (fs.type == NRE_GPU_FS_UNDEFINED){
        output.append(NRE_Shader(
            out vec4 fragColor;
            
            void main(){
                fragColor = vec4(1.0);
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
        if (NRE_GPU_ShaderBase::backend == NRE_GPU_GLES){
            output.append(NRE_Shader(
            
            out vec4 fragColor;
            
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
                
                vec3 dif_output = clamp(mat_diffuse.rgb*max(sDotN, 0.2), 0.0, 1.0);
                
                //fragColor = vec4(abs(normals), 1.0);
                fragColor = vec4(pow(dif_output, vec3(1.0/2.2)), 1.0);
            }
            ));
        } else {
            output.append(NRE_Shader(
            
            out vec4 fragColor;
            
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
                
                vec3 dif_output = clamp(mat_diffuse.rgb*max(sDotN, 0.2), 0.0, 1.0);
                
                //fragColor = vec4(abs(normals), 1.0);
                fragColor = vec4(dif_output, 1.0);
            }
            ));
        }
    }
    else if (fs.type == NRE_GPU_FS_NORMALS){
        if (NRE_GPU_ShaderBase::backend == NRE_GPU_GLES){
            output.append(NRE_Shader(
            
            out vec4 fragColor;
            
            //const vec3 light_pos = vec3( 13.37035561, -12.76134396, 10.10574818);
            
            void main(){
                
                fragColor = vec4(pow(abs(normals), vec3(1.0/2.2)), 1.0);
            }
            ));
        } else {
            output.append(NRE_Shader(
            
            out vec4 fragColor;
            
            //const vec3 light_pos = vec3( 13.37035561, -12.76134396, 10.10574818);
            
            void main(){
                
                fragColor = vec4(abs(normals), 1.0);
            }
            ));
        }
    }
    
    return NRE_GPU_ShaderBase::shader_prefix + output;
}
