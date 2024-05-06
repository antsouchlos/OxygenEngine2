#ifndef NRE_RENDERDATA_H
#define NRE_RENDERDATA_H

#include <OE/Renderer/api_gpu.h>
#include <OE/Renderer/drawcall_container.h>
#include <OE/types/world.h>
#include <memory>


class OE_Mat4x4;
namespace nre {
    struct base_object_t {

        std::size_t        id{0};
        bool               changed{false};
        bool               has_init{false};
        std::vector<float> data;
    };

    struct camera_render_data_t : public nre::base_object_t {
        OE_Mat4x4 perspective_view_mat;
        OE_Mat4x4 perspective_mat;
        OE_Mat4x4 view_mat;
        OE_Mat4x4 model_mat;

        OE_Vec4 get_position();

        float near{0.0f};
        float far{0.0f};

        std::size_t  scene_id{0};
        std::size_t  ubo{0};
        unsigned int offset{0};
        unsigned int size{0};
    };

    struct material_render_data_t : public nre::base_object_t {

        std::size_t scene_id{0};

        OE_Vec4 get_mat_diffuse();
        float   get_mat_specular_hardness();


        std::size_t  ubo{0};
        unsigned int offset{0};
        unsigned int size{0};
    };

    struct vgroup_render_data_t {

        std::size_t id{0};
        OE_Mat4x4   bone_mat;

        std::size_t ibo{0};
        std::size_t material_id{0};
        std::size_t mesh_id{0};

        unsigned int offset{0};
        unsigned int size{0};
    };

    struct mesh_render_data_t : public nre::base_object_t {

        std::size_t scene_id{0};

        OE_Mat4x4 model_mat;

        unsigned int uvmaps{0};
        unsigned int bones{0};

        std::size_t vbo{0};
        std::size_t vbo_size{0};

        std::size_t  vao{0};
        std::size_t  ubo{0};
        unsigned int offset{0};
        unsigned int size{0};

        bool vao_initialized{false};

        std::vector<nre::gpu::vertex_layout_input> vao_input;

        float max_x{0.0f}, min_x{0.0f}, max_y{0.0f}, min_y{0.0f}, max_z{0.0f}, min_z{0.0f}, max_radius{0.0f}, min_radius{0.0f};

        std::vector<float> get_scaling_min_data();
        std::vector<float> get_scaling_max_data();

        std::vector<float>                                vbo_data;
        std::unordered_map<size_t, std::vector<uint32_t>> ibos_data;

        std::set<size_t> vgroups;
    };

    struct point_light_render_data_t : public nre::base_object_t {
        OE_Mat4x4 model_mat;

        OE_RGBColor color;
        float       intensity{0.0f};
        float       range{0.0f};

        std::size_t  ubo{0};
        unsigned int offset{0};
        unsigned int size{0};
    };

    struct directional_light_render_data_t : public nre::base_object_t {
        OE_Mat4x4   model_mat;
        OE_RGBColor color;
        float       intensity;


        std::size_t  ubo{0};
        unsigned int offset{0};
        unsigned int size{0};
    };

    struct scene_render_data_t : public nre::base_object_t {

        std::set<std::size_t> cameras;
        std::set<std::size_t> meshes;
        std::set<std::size_t> dir_lights;
        std::set<std::size_t> pt_lights;
        std::set<std::size_t> materials;
    };

    struct viewport_render_data_t : public nre::base_object_t {

        std::vector<std::size_t> layers;
        std::vector<std::size_t> cameras;

        std::vector<int> camera_modes;

        std::vector<int> layer_combine_modes;
        // two floats for each layer, but only useful when there is a split screen in eye coordinates
        std::vector<float> split_screen_positions;
    };
}; // namespace nre

#endif // FE_MESHRENDERDATA_H
