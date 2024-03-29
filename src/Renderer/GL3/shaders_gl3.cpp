#include <OE/Renderer/GL3/shaders_gl3.h>
#include <OE/Renderer/api_gpu.h>
#include <iostream>

using namespace std;

std::string nre::gl3::gen_vertex_shader(nre::gpu::vertex_shader_t vs) {

    std::string output = "\n";

    if (nre::gpu::get_api() == nre::gpu::GLES) {
        output.append("precision highp float; \n");
    }

    output.append("invariant gl_Position;\n");

    if (vs.fullscreenQuad) {
        output.append(NRE_Shader(layout(location = 0) in vec2 oe_position;

                                 out vec2 position;

                                 void main() {
                                     position    = oe_position;
                                     gl_Position = vec4(oe_position, 0.0, 1.0);
                                 }));
    }
    else {

        if (vs.type == nre::gpu::VS_Z_PREPASS) {
            output.append(NRE_Shader(
                layout(location = 0) in vec3 oe_position;

                layout(std140) uniform OE_Camera {
                    mat4 PV_Matrix;
                    vec4 camera_pos;
                };

                layout(std140) uniform OE_Mesh32 {
                    mat4 Model_Matrix;
                    vec4 scaling_max_data;
                    vec4 scaling_min_data;
                };

                void main() {
                    // float Fcoef = 2.0 / log2(150.0 + 1.0);

                    // mat4 final_mat = PV_Matrix*Model_Matrix;
                    gl_Position.w = 1.0;
                    gl_Position   = PV_Matrix * (Model_Matrix * vec4(oe_position, 1.0));

                    // gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
                    // gl_Position.z *= gl_Position.w;
                }));
        }
        else if (vs.type == nre::gpu::VS_LIGHT) {
            output.append(NRE_Shader(
                layout(location = 0) in vec3 oe_position;
                // layout (location = 1) in vec3 oe_normals;

                // out vec3 position;
                // out vec3 normals;
                flat out float instance_num;

                layout(std140) uniform OE_Camera {
                    mat4 PV_Matrix;
                    vec4 camera_pos;
                };

                layout(std140) uniform OE_Lights { mat4 Model_Matrix[255]; };

                void main() {
                    // float Fcoef = 2.0 / log2(150.0 + 1.0);



                    mat4  model_copy = Model_Matrix[gl_InstanceID];
                    float scale      = model_copy[3][3];
                    mat4  final_mat  = PV_Matrix;

                    vec3 delta_pos     = vec3(model_copy[3][0], model_copy[3][1], model_copy[3][2]);
                    vec4 temp_position = vec4(oe_position * scale + delta_pos, 1.0);

                    // position = temp_position.xyz;
                    float instance_float = float(gl_InstanceID + 1);
                    instance_num         = instance_float / 256.0 + 1.0 / 256.0;
                    gl_Position.w        = 1.0;
                    gl_Position          = final_mat * temp_position;

                    // gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
                    // gl_Position.z *= gl_Position.w;
                }));
        }
        else if ((vs.type == nre::gpu::VS_REGULAR) || (vs.type == nre::gpu::VS_BOUNDING_BOX) ||
                 (vs.type == nre::gpu::VS_BOUNDING_SPHERE)) {

            // setup inputs
            output.append(NRE_Shader(layout(location = 0) in vec3 oe_position; layout(location = 1) in vec3 oe_normals;));
            output.append("\n");
            for (size_t i = 0; i < vs.num_of_uvs; i++) {
                output.append("layout (location=" + to_string(2 + i) + ") in vec2 oe_uvs" + to_string(i) + ";");
                output.append("\n");
            }

            // setup outputs
            output.append(NRE_Shader(out vec3 position; out vec3 normals;));
            output.append("\n");
            for (size_t i = 0; i < vs.num_of_uvs; i++) {
                output.append("out vec2 uvs" + to_string(i) + ";");
                output.append("\n");
            }

            // setup uniforms
            output.append(NRE_Shader(
                layout(std140) uniform OE_Camera {
                    mat4 PV_Matrix;
                    vec4 camera_pos;
                };

                layout(std140) uniform OE_Mesh32 {
                    mat4 Model_Matrix;
                    vec4 scaling_max_data;
                    vec4 scaling_min_data;
                };));

            // setup final logic
            if (vs.type == nre::gpu::VS_REGULAR) {

                // handle regular case
                output.append(NRE_Shader(void main() {
                    // float Fcoef = 2.0 / log2(150.0 + 1.0);

                    normals = mat3(Model_Matrix) * oe_normals;
                    // mat4 final_mat = PV_Matrix*Model_Matrix;

                    vec4 temp_position = Model_Matrix * vec4(oe_position, 1.0);
                    position           = temp_position.xyz;
                    gl_Position.w      = 1.0;
                    gl_Position        = PV_Matrix * (Model_Matrix * vec4(oe_position, 1.0));

                    // gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
                    // gl_Position.z *= gl_Position.w;
                }));
            }
            else if (vs.type == nre::gpu::VS_BOUNDING_BOX) {

                // handle bounding box case
                // here one does not need to consider rotation, since the (non-naive) bounding box
                // already accounts for rotation, but needs to handle scale
                output.append(NRE_Shader(void main() {
                    // float Fcoef = 2.0 / log2(150.0 + 1.0);

                    normals        = oe_normals;
                    mat4 final_mat = PV_Matrix;

                    vec3 delta_pos     = vec3(Model_Matrix[3][0], Model_Matrix[3][1], Model_Matrix[3][2]);
                    vec4 temp_position = vec4(oe_position * scaling_max_data.xyz + delta_pos, 1.0);
                    if (oe_position.x * abs(oe_position.x) < 0.0) {
                        temp_position.x = abs(oe_position.x) * scaling_min_data.x + delta_pos.x;
                    }
                    if (oe_position.y * abs(oe_position.y) < 0.0) {
                        temp_position.y = abs(oe_position.y) * scaling_min_data.y + delta_pos.y;
                    }
                    if (oe_position.z * abs(oe_position.z) < 0.0) {
                        temp_position.z = abs(oe_position.z) * scaling_min_data.z + delta_pos.z;
                    }

                    position      = temp_position.xyz;
                    gl_Position.w = 1.0;
                    gl_Position   = final_mat * temp_position;

                    // gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
                    // gl_Position.z *= gl_Position.w;
                }));
            }
            else if (vs.type == nre::gpu::VS_BOUNDING_SPHERE) {

                // handle bounding sphere case
                // here one does not need to consider rotation, since the sphere
                // already accounts for rotation, but needs appropriate scaling

                output.append(NRE_Shader(void main() {
                    // float Fcoef = 2.0 / log2(150.0 + 1.0);

                    normals        = oe_normals;
                    mat4 final_mat = PV_Matrix;

                    vec3 delta_pos = vec3(Model_Matrix[3][0], Model_Matrix[3][1], Model_Matrix[3][2]);
                    vec4 temp_position =
                        vec4(oe_position * max(abs(scaling_max_data.w), abs(scaling_min_data.w)) + delta_pos, 1.0);

                    position      = temp_position.xyz;
                    gl_Position.w = 1.0;
                    gl_Position   = final_mat * temp_position;

                    // gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
                    // gl_Position.z *= gl_Position.w;
                }));
            }
            else {
            }
        }
    }



    return nre::gpu::gen_shader_prefix() + output;
}

std::string nre::gl3::gen_pixel_shader(nre::gpu::pixel_shader_t fs) {

    std::string output = "\n";

    if (nre::gpu::get_api() == nre::gpu::GLES) {
        output.append("precision highp float; \n");
    }

    if ((fs.type == nre::gpu::FS_UNDEFINED) or (fs.type == nre::gpu::FS_SIMPLE)) {
        output.append(NRE_Shader(in vec2 position; out vec4 fragColor;

                                 void main() { fragColor = vec4(vec3(0.5), 1.0); }));
        return nre::gpu::gen_shader_prefix() + output;
    }
    else if (fs.type == nre::gpu::FS_GAMMA) {
        output.append(NRE_Shader(in vec2 position; out vec4 fragColor;

                                 uniform sampler2D tex_output;

                                 void main() {
                                     vec4 sampled_data = texture(tex_output, position / 2.0 + 0.5);
                                     // for (int i=0; i <4; i++){
                                     //     if (sampled_data[i] <= 0.00314){
                                     //         sampled_data[i] = sampled_data[i]*12.92;
                                     //     }
                                     //     else{
                                     //         sampled_data[i] = 1.055*pow(sampled_data[i], 1.0/2.4) - 0.055;
                                     //     }
                                     // }
                                     fragColor = vec4(pow(sampled_data.xyz, vec3(1.0 / 2.2)), sampled_data.w);
                                 }));
        return nre::gpu::gen_shader_prefix() + output;
    }
    else if (fs.type == nre::gpu::FS_LIGHT_INDEX) {
        output.append(NRE_Shader(flat in float instance_num;

                                 out vec4 fragColor;

                                 void main() {
                                     // fragColor = uvec4((instance_num & 3) << 6, (instance_num & 12) << 4, (instance_num & 48)
                                     // << 2, (instance_num & 192));
                                     fragColor = vec4(instance_num, 1.0 - instance_num, instance_num, 1.0 - instance_num);
                                 }

                                 ));
        return nre::gpu::gen_shader_prefix() + output;
    }

    output.append(NRE_Shader(in vec3 position; in vec3 normals;));
    output.append("\n");

    // setup inputs
    for (size_t i = 0; i < fs.num_of_uvs; i++) {
        output.append("in vec2 uvs" + to_string(i) + ";");
        output.append("\n");
    }

    // setup uniforms

    output.append(NRE_Shader(layout(std140) uniform OE_Material {
        vec4  mat_diffuse;
        vec4  mat_specular;
        float mat_specular_intensity;
        float mat_specular_hardness;
        float mat_translucency;
        float mat_illuminosity;
    };));
    output.append("\n");

    if (fs.type == nre::gpu::FS_MATERIAL) {

        // setup uniforms
        output.append(NRE_Shader(
            layout(std140) uniform OE_Camera {
                mat4 PV_Matrix;
                vec4 camera_pos;
            };

            layout(std140) uniform OE_Mesh32 {
                mat4 Model_Matrix;
                vec4 scaling_max_data;
                vec4 scaling_min_data;
            };

            out vec4 fragColor;));
        output.append("\n");



        output.append(NRE_Shader(

            void main() {
                vec3 light_pos = vec3(camera_pos);

                vec3 s = normalize(light_pos - position);
                vec3 v = normalize(-position);

                vec3 specular = vec3(0.0);

                float sDotN = abs(dot(s, normals));

                if (sDotN > 0.0) {
                    // specular = vec3(0.5);
                    specular = vec3(pow(sDotN, 2.0 / mat_specular_hardness));
                }

                vec3 dif_output = clamp(mat_diffuse.rgb * max(sDotN, 0.2), 0.0, 1.0);

                // fragColor = vec4(abs(normals), 1.0);
                fragColor = vec4(dif_output, 1.0);
            }));
    }
    else if (fs.type == nre::gpu::FS_NORMALS) {

        output.append(NRE_Shader(

            out vec4 fragColor;

            // const vec3 light_pos = vec3( 13.37035561, -12.76134396, 10.10574818);

            void main() { fragColor = vec4(abs(normals), 1.0); }));
    }

    return nre::gpu::gen_shader_prefix() + output;
}
