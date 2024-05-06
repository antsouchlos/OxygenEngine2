#ifndef OE_DUMMYCLASSES_H
#define OE_DUMMYCLASSES_H

#include <OE/Events/event_handler.h>
#include <OE/Renderer/api_gpu.h>
#include <OE/task.h>
#include <OE/types/base_types.h>

namespace oe {

    enum OS : int { OS_UNDEFINED = 0, OS_LINUX = 1, OS_WINDOWS = 2, OS_ANDROID = 3, OS_WEB = 4 };

    enum WINSYS : int { WINSYS_NONE = 0, WINSYS_SDL = 1, WINSYS_SOMETHING_ELSE = 2 };

    enum RENDERER_SHADING_MODE : int {
        RENDERER_NORMALS_SHADING        = 0,
        RENDERER_NO_LIGHTS_SHADING      = 1,
        RENDERER_DIR_LIGHTS_SHADING     = 2,
        RENDERER_INDEXED_LIGHTS_SHADING = 3,
        RENDERER_REGULAR_SHADING        = 4
    };

    /** This a dummy class aimed to be used as a base class for windowing systems
     * Plus relevant structs for getting info in and out
     */

    // initial parameters
    struct winsys_init_info {
        nre::gpu::SHADER_BACKEND requested_backend{nre::gpu::GL};
    };

    // dynamic parameters
    struct winsys_update_info {
        std::string title;
        int         res_x{640};
        int         res_y{480};
        bool        changed_resolution{false};

        bool use_fullscreen{false};
        bool changed_fullscreen{false};

        bool vsync{true};
        bool mouse_locked{true};
    };

    // output to the renderer
    struct winsys_output {
        int                      res_x{640};
        int                      res_y{480};
        nre::gpu::SHADER_BACKEND backend{nre::gpu::GLES2};
        int                      major{2};
        int                      minor{0};
        bool                     mouse_moved{false};
        bool                     restart_renderer{false};
        bool                     done{false};
        int                      dpi{96};
    };

    class winsys_base_t : public OE_THREAD_SAFETY_OBJECT {
        friend class task_manager;

    public:
        winsys_base_t();
        virtual ~winsys_base_t();


        virtual winsys_output init(winsys_init_info, winsys_update_info);
        virtual winsys_output update(winsys_update_info);

        virtual bool is_mouse_locked();
        virtual void lock_mouse();
        virtual void unlock_mouse();

        virtual void destroy();


        // The global event handler is here and must be initialized in all sub classes
        // YES this MUST be a public member, but i made sure one cannot fuck it up easily
        oe::event_handler_t event_handler_;

    protected:
        virtual bool update_events();
        OS           os_{OS_UNDEFINED};
        WINSYS       winsys_{WINSYS_NONE};
    };

    /** This is a dummy class aimed to be a base class for
     *  a renderer
     * plus relevant structs for getting info in and out
     */
    // initial parameters
    struct renderer_init_info {
        bool some_var{false};
    };

    // dynamic parameters
    struct renderer_update_info {
        bool                  use_hdr{false};
        bool                  use_wireframe{false};
        bool                  use_light_indexed_rendering{false};
        bool                  render_bounding_boxes{false};
        bool                  render_bounding_spheres{false};
        bool                  use_z_prepass{true};
        bool                  use_software_vertex_shaders{false};
        RENDERER_SHADING_MODE shading_mode{RENDERER_REGULAR_SHADING};

        /// Without sanity checks the renderer may segfault or silently fail without warning or error messages
        /// Tradeoff: Higher performance
        bool sanity_checks{true};
    };

    class renderer_base_t : public OE_THREAD_SAFETY_OBJECT {
    public:
        renderer_base_t() = delete;
        renderer_base_t(bool, std::string);
        virtual ~renderer_base_t();

        virtual bool init(renderer_init_info, renderer_update_info, winsys_output);
        virtual bool update_single_thread(renderer_update_info, winsys_output);
        // last bool is true if the renderer has been restarted. This is useful so as to fetch all the data again
        virtual bool update_data(renderer_update_info, winsys_output, bool);

        virtual bool update_multi_thread(const task_info_t, int); // stub for now
        virtual void destroy();

        bool              is_multi_threaded();
        const std::string get_name();

    protected:
        const bool        use_multithreading_{false};
        const std::string name_;
    };

    /** This is a dummy class aimed to be a base class for
     *  a physics engine
     * plus relevant structs for getting info in and out
     */
    // initial parameters
    struct physics_init_info {
        bool some_var{false};
    };

    // dynamic parameters
    struct physics_update_info {
        float gravity{-9.81f};
    };

    class physics_base_t : public OE_THREAD_SAFETY_OBJECT {
    public:
        physics_base_t() = delete;
        physics_base_t(std::string);
        virtual ~physics_base_t();

        virtual bool init(physics_init_info);

        virtual void update_info(physics_update_info) noexcept;
        virtual bool update_multi_thread(const task_info_t, int);
        virtual void destroy();

        const std::string get_name();

    protected:
        const std::string name_;
    };

    /** This is a dummy class aimed to be a base class for
     *  a netwotking manager
     * plus relevant structs for getting info in and out
     */
    struct networking_init_info {
        bool some_var{false};
    };

    class networking_base_t : public OE_THREAD_SAFETY_OBJECT {
    public:
        networking_base_t() = delete;
        networking_base_t(std::string);
        virtual ~networking_base_t();

        virtual void        init(networking_init_info);
        virtual task_action execute(const task_info_t);
        virtual void        destroy();

        const std::string get_name();

    protected:
        std::atomic<bool> done_{false};
        const std::string name_;
    };
}; // namespace oe
#endif
