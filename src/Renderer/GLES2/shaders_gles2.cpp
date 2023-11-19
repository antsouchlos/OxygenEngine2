#include <OE/Renderer/GLES2/shaders_gles2.h>
#include <OE/Renderer/api_gpu.h>
#include <iostream>

using namespace std;

std::string nre::gles2::gen_vertex_shader(nre::gpu::vertex_shader_t vs) {

    std::string output = "\n";

    output.append("precision highp float; \n");

    output.append("invariant gl_Position;\n");

    if (vs.fullscreenQuad) {
        output.append(NRE_Shader(attribute vec2 oe_position;

                                 varying vec2 position;

                                 void main() {
                                     position    = oe_position;
                                     gl_Position = vec4(oe_position, 0.0, 1.0);
                                 }));
    }
    else {

        if (vs.type == nre::gpu::VS_Z_PREPASS) {
            output.append(NRE_Shader(attribute vec3 oe_position;

                                     uniform mat4 MVP_Matrix;

                                     void main() {
                                         // float Fcoef = 2.0 / log2(150.0 + 1.0);

                                         // mat4 final_mat = PV_Matrix*Model_Matrix;
                                         gl_Position.w = 1.0;
                                         gl_Position   = MVP_Matrix * vec4(oe_position, 1.0);

                                         // gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
                                         // gl_Position.z *= gl_Position.w;
                                     }));
        }
        else if ((vs.type == nre::gpu::VS_REGULAR) || (vs.type == nre::gpu::VS_BOUNDING_BOX) ||
                 (vs.type == nre::gpu::VS_BOUNDING_SPHERE) or (vs.type == nre::gpu::VS_REGULAR_SOFTWARE)) {

            bool use_software_dummy = vs.type == nre::gpu::VS_REGULAR_SOFTWARE;

            // setup inputs
            output.append(NRE_Shader(attribute vec3 oe_position; attribute vec3 oe_normals;));

            if (use_software_dummy) {
                output.append(NRE_Shader(attribute vec4 oe_position_clip;));
                output.append("\n");
            }
            for (size_t i = 0; i < vs.num_of_uvs; i++) {
                output.append("attribute vec2 oe_uvs" + to_string(i) + ";");
                output.append("\n");
            }


            // setup outputs
            output.append(NRE_Shader(varying vec3 position; varying vec3 normals;));
            output.append("\n");
            for (size_t i = 0; i < vs.num_of_uvs; i++) {
                output.append("varying vec2 uvs" + to_string(i) + ";");
                output.append("\n");
            }

            // setup uniforms
            if (!use_software_dummy) output.append(NRE_Shader(uniform mat4 Model_Matrix;));

            // setup final logic
            if (use_software_dummy) {
                output.append(NRE_Shader(void main() {
                    normals     = oe_normals;
                    position    = oe_position;
                    gl_Position = oe_position_clip;
                }));
            }
            else if (vs.type == nre::gpu::VS_REGULAR) {



                // handle regular case
                output.append(NRE_Shader(

                    uniform mat4 MVP_Matrix;

                    void main() {
                        // float Fcoef = 2.0 / log2(150.0 + 1.0);

                        normals = mat3(Model_Matrix) * oe_normals;
                        // mat4 final_mat = PV_Matrix*Model_Matrix;

                        vec4 temp_position = Model_Matrix * vec4(oe_position, 1.0);
                        position           = temp_position.xyz;
                        gl_Position.w      = 1.0;
                        gl_Position        = MVP_Matrix * vec4(oe_position, 1.0);

                        // gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * Fcoef - 1.0;
                        // gl_Position.z *= gl_Position.w;
                    }));
            }
            else if (vs.type == nre::gpu::VS_BOUNDING_BOX) {

                // handle bounding box case
                // here one does not need to consider rotation, since the (non-naive) bounding box
                // already accounts for rotation, but needs to handle scale
                output.append(NRE_Shader(

                    uniform mat4 PV_Matrix; uniform vec4 scaling_min_data; uniform vec4 scaling_max_data;

                    void main() {
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


                output.append(NRE_Shader(

                    uniform mat4 PV_Matrix; uniform vec4 scaling_min_data; uniform vec4 scaling_max_data;

                    void main() {
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

std::string nre::gles2::gen_pixel_shader(nre::gpu::pixel_shader_t fs) {

    std::string output = "\n";

    output.append("precision mediump float; \n");

    if ((fs.type == nre::gpu::FS_UNDEFINED) or (fs.type == nre::gpu::FS_SIMPLE)) {
        output.append(NRE_Shader(varying vec2 position;

                                 void main() { gl_FragColor = vec4(vec3(0.5), 1.0); }));
        return nre::gpu::gen_shader_prefix() + output;
    }
    else if (fs.type == nre::gpu::FS_GAMMA) {
        output.append(NRE_Shader(varying vec2 position;

                                 uniform sampler2D tex_output; vec4 sampled_data;

                                 void main() {
                                     sampled_data = texture2D(tex_output, position / 2.0 + 0.5);
                                     // for (int i=0; i <4; i++){
                                     //     if (sampled_data[i] <= 0.00314){
                                     //         sampled_data[i] = sampled_data[i]*12.92;
                                     //     }
                                     //     else{
                                     //         sampled_data[i] = 1.055*pow(sampled_data[i], 1.0/2.4) - 0.055;
                                     //     }
                                     // }
                                     gl_FragColor = vec4(pow(sampled_data.xyz, vec3(1.0 / 2.2)), sampled_data.w);
                                 }));
        return nre::gpu::gen_shader_prefix() + output;
    }
    else {
    }

    output.append(NRE_Shader(varying vec3 position; varying vec3 normals;));
    output.append("\n");

    // setup inputs
    for (size_t i = 0; i < fs.num_of_uvs; i++) {
        output.append("varying vec2 uvs" + to_string(i) + ";");
        output.append("\n");
    }

    // setup uniforms

    output.append(NRE_Shader(uniform vec4 mat_diffuse; uniform float mat_specular_hardness;));
    output.append("\n");

    if (fs.type == nre::gpu::FS_MATERIAL) {

        // setup uniforms
        output.append(NRE_Shader(uniform vec4 camera_pos;

                                 ));
        output.append("\n");



        output.append(NRE_Shader(vec3 light_pos; vec3 s; vec3 v; vec3 specular; float sDotN; vec3 dif_output;

                                 void main() {
                                     light_pos = vec3(camera_pos);

                                     s = normalize(light_pos - position);
                                     v = normalize(-position);

                                     specular = vec3(0.0);
                                     sDotN    = abs(dot(s, normals));

                                     if (sDotN > 0.0) {
                                         // specular = vec3(0.5);
                                         specular = vec3(pow(sDotN, 2.0 / mat_specular_hardness));
                                     }

                                     dif_output = clamp(mat_diffuse.rgb * max(sDotN, 0.2), 0.0, 1.0);

                                     // gl_FragColor = vec4(abs(normals), 1.0);
                                     gl_FragColor = vec4(dif_output, 1.0);
                                 }));
    }
    else if (fs.type == nre::gpu::FS_NORMALS) {

        output.append(NRE_Shader(


            // const vec3 light_pos = vec3( 13.37035561, -12.76134396, 10.10574818);

            void main() { gl_FragColor = vec4(abs(normals), 1.0); }));
    }

    return nre::gpu::gen_shader_prefix() + output;
}
