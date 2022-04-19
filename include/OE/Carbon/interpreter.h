#ifndef CSL_INTERPRETER_H_
#define CSL_INTERPRETER_H_


#include <OE/task_manager.h>
#include <OE/types/world.h>

#include <OE/Carbon/exceptions_csl.h>
#include <OE/Carbon/parser.h>

#include <bitset>
#include <charconv>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>


class OE_TaskManager;


// TODO: Make use of RAII in every type class
// TODO: Everywhere, where num_of* is used, instead just call *.elements.at("*").size(). No extra
// 		 variable is necessary
// TODO: Add the option to make members optional


// Philipp might kill me for this, but I really think it looks better,
// so it's a chance I'm willing to take
// Update: TODO: Don't add this to the oe namespace
namespace oe {
    using world       = OE_World;
    using scene       = OE_Scene;
    using camera      = OE_Camera;
    using light       = OE_Light;
    using mesh        = OE_Mesh32;
    using uvmap_data  = OE_UVMapData;
    using triangle    = OE_Triangle32;
    using vgroup      = OE_VertexGroup;
    using texture     = OE_Texture;
    using tcm_texture = OE_TCM_Texture;
    using material    = OE_Material;
    using tcm         = OE_TCM;
    using vpconfig    = OE_ViewportConfig;
} // namespace oe


namespace csl {
    using world_ptr    = std::shared_ptr<oe::world>;
    using scene_ptr    = std::shared_ptr<oe::scene>;
    using camera_ptr   = std::shared_ptr<oe::camera>;
    using light_ptr    = std::shared_ptr<oe::light>;
    using mesh_ptr     = std::shared_ptr<oe::mesh>;
    using vgroup_ptr   = std::shared_ptr<oe::vgroup>;
    using texture_ptr  = std::shared_ptr<oe::texture>;
    using material_ptr = std::shared_ptr<oe::material>;
    using tcm_ptr      = std::shared_ptr<oe::tcm>;
    using vpconfig_ptr = std::shared_ptr<oe::vpconfig>;


    class Interpreter {
    public:
        static int   sv_to_int(std::string_view sv);
        static float sv_to_float(std::string_view sv);

        void interpret(std::string& input);

    private:
        OE_SharedIndexMap<OE_Scene>          scene_list_;
        OE_SharedIndexMap<OE_Object>         object_list_;
        OE_SharedIndexMap<OE_Material>       material_list_;
        OE_SharedIndexMap<OE_Texture>        texture_list_;
        OE_SharedIndexMap<OE_TCM>            tcm_list_;
        OE_SharedIndexMap<OE_ViewportConfig> viewport_list_;


        world_ptr    process_world(const element&);
        scene_ptr    process_scene(const element&);
        camera_ptr   process_camera(const element&);
        light_ptr    process_light(const element&);
        mesh_ptr     process_mesh(const element&);
        vgroup_ptr   process_vgroup(const element&);
        texture_ptr  process_texture(const element&);
        material_ptr process_material(const element&);
        tcm_ptr      process_tcm(const element&);
        vpconfig_ptr process_vpconfig(const element&);

        oe::tcm_texture process_tcm_texture(const element&);
        oe::uvmap_data  process_uvmap_data(const element&, std::size_t);

        oe::triangle process_triangle(mesh_ptr, const element&, std::size_t);
    };

    inline void interpret(std::string& input) {
        csl::Interpreter interpreter;

        interpreter.interpret(input);
    }

    inline void interpret_file(std::string& path_to_file) {
        // TODO: Read from disk more efficiently

        auto t = clock();

        std::string   input = "";
        std::ifstream f(path_to_file);

        std::string line;
        while (getline(f, line))
            input += line;

        f.close();

        std::cout << "[Performance] Time reading from file: " << (float)(clock() - t) / CLOCKS_PER_SEC << std::endl;

        interpret(input);
    }
} // namespace csl


#endif
