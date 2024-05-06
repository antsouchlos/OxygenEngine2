#ifndef OE_API_H_INCLUDED
#define OE_API_H_INCLUDED

#include <OE/api_helpers_oe.h>
#include <OE/math_oe.h>
#include <OE/task_manager.h>


/*enum OE_OS : int{
    OE_UNDEFINED=0,
    OE_LINUX=1,
    OE_WINDOWS=2
};

enum OE_WINSYS : int{
    OE_NONE=0,
    OE_SDL=1,
    OE_SOMETHING_ELSE=2
};

enum oe::renderer::shading_mode::type : int{
    OE_RENDERER_NORMALS_SHADING=0,
    OE_RENDERER_NO_LIGHTS_SHADING=1,
    OE_RENDERER_DIR_LIGHTS_SHADING=2,
    OE_RENDERER_INDEXED_LIGHTS_SHADING=3,
    OE_RENDERER_REGULAR_SHADING=4
};*/


// This is going to be a very long header with ALL avaiable API functions declared
// TODO: Make this extern "C" so that we have a C compatible API (Long-term goal)

namespace oe {

    // GLOBAL VARIABLE
    extern task_manager_t* OE_Main;

    // class typedefs
    typedef OE_Vec3   vec3;
    typedef OE_Vec4   vec4;
    typedef OE_Mat4x4 mat4x4;
    typedef OE_Quat   quat;
    typedef OE_Mat3x3 mat3x3;

    //------------------------BLOCK-------------------------//
    // API functions to be executed before the engine runs
    namespace preinit {

        extern oe::renderer_init_info   renderer_parameters;
        extern oe::winsys_init_info     winsys_parameters;
        extern oe::physics_init_info    physics_parameters;
        extern oe::networking_init_info networking_parameters;

        void request_gles2();
        void request_gles31();
    }; // namespace preinit

    /** Basic API functions for starting the Oxygen Engine
     *  and assigning tasks
     */
    size_t init();
    size_t init(std::string, bool);
    size_t init(std::string, int, int, bool);
    void   step();
    void   start();
    void   destroy();
    bool   is_done();
    void   finish();
    // size_t initFromFile(std::string); //TODO

    //------------------------BLOCK-------------------------//
    // The API functions inside this block are the only ones that are
    // safe to be called on an unsynchronized thread together with all the get* functions

    // plain add task
    template <typename T, typename... Args>
    void add_task_func(std::string name, T func, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->add_task(name, func_copy);
    }

    template <typename T>
    void add_task_func(std::string name, T func) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1);
        OE_Main->add_task(name, func_copy);
    }

    template <typename T, typename A, typename... Args>
    void add_task_method(std::string name, T func, A instance, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->add_task(name, func_copy);
    }

    template <typename T, typename A>
    void add_task_method(std::string name, T func, A instance) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1);
        OE_Main->add_task(name, func_copy);
    }

    // with priority
    template <typename T, typename... Args>
    void add_task_func(std::string name, int priority, T func, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->add_task(name, func_copy, priority);
    }

    template <typename T>
    void add_task_func(std::string name, int priority, T func) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1);
        OE_Main->add_task(name, func_copy, priority);
    }

    template <typename T, typename A, typename... Args>
    void add_task_method(std::string name, int priority, T func, A instance, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->add_task(name, func_copy, priority);
    }

    template <typename T, typename A>
    void add_task_method(std::string name, int priority, T func, A instance) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1);
        OE_Main->add_task(name, func_copy, priority);
    }

    // with priority and threadname
    template <typename T, typename... Args>
    void add_task_func(std::string name, std::string threadname, int priority, T func, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->add_task(name, func_copy, priority, threadname);
    }

    template <typename T>
    void add_task_func(std::string name, std::string threadname, int priority, T func) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1);
        OE_Main->add_task(name, func_copy, priority, threadname);
    }

    template <typename T, typename A, typename... Args>
    void add_task_method(std::string name, std::string threadname, int priority, T func, A instance, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->add_task(name, func_copy, priority, threadname);
    }

    template <typename T, typename A>
    void add_task_method(std::string name, std::string threadname, int priority, T func, A instance) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1);
        OE_Main->add_task(name, func_copy, priority, threadname);
    }

    // with threadname
    template <typename T, typename... Args>
    void add_task_func(std::string name, std::string threadname, T func, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->add_task(name, func_copy, threadname);
    }

    template <typename T>
    void add_task_func(std::string name, std::string threadname, T func) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1);
        OE_Main->add_task(name, func_copy, threadname);
    }

    template <typename T, typename A, typename... Args>
    void add_task_method(std::string name, std::string threadname, T func, A instance, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->add_task(name, func_copy, threadname);
    }

    template <typename T, typename A>
    void add_task_method(std::string name, std::string threadname, T func, A instance) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1);
        OE_Main->add_task(name, func_copy, threadname);
    }

    void remove_task(std::string);
    void remove_task(std::string, std::string);

    void        broadcast_event(std::string);
    void        broadcast_event(std::size_t);
    std::size_t create_event(std::string);
    std::size_t create_internal_physics_event(std::string);
    std::size_t create_internal_renderer_event(std::string);
    std::size_t create_internal_network_event(std::string);
    std::size_t get_event_id(std::string);
    std::string get_event_name(std::size_t);

    template <typename T, typename... Args>
    void set_event_func(std::string name, T func, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->window_->event_handler_.set_event_func(name, func_copy);
    }

    template <typename T>
    void set_event_func(std::string name, T func) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1);
        OE_Main->window_->event_handler_.set_event_func(name, func_copy);
    }

    template <typename T, typename A, typename... Args>
    void set_event_method(std::string name, T func, A instance, Args... arguments) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->window_->event_handler_.set_event_func(name, func_copy);
    }

    template <typename T, typename A>
    void set_event_method(std::string name, T func, A instance) {

        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1);
        OE_Main->window_->event_handler_.set_event_func(name, func_copy);
    }

    void destroy_event(std::string);

    void pause(int);

    // for events in general
    size_t get_event_activations(std::string);
    size_t get_event_counter(std::string);

    // for keyboard and mouse events
    bool is_key_just_pressed(std::string);
    bool is_key_just_released(std::string);

    bool is_key_held(std::string);
    bool is_key_pressed(std::string);

    bool is_mouse_moved();

    int get_delta_mouse_x();
    int get_delta_mouse_y();

    int get_mouse_x();
    int get_mouse_y();

    bool is_mouse_locked();
    void mouse_lock();
    void mouse_unlock();

    //------------------------BLOCK-------------------------//

    task_info_t get_task_info(std::string, std::string);

    void create_new_thread(std::string);

    template <typename T, typename... Args>
    void create_unsync_thread_func(std::string name, T func, Args... arguments) {
        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->create_unsync_thread(name, func_copy);
    }

    template <typename T>
    void create_unsync_thread_func(std::string name, T func) {
        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1);
        OE_Main->create_unsync_thread(name, func_copy);
    }

    template <typename T, typename A, typename... Args>
    void create_unsync_thread_method(std::string name, T func, A instance, Args... arguments) {
        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->create_unsync_thread(name, func_copy);
    }

    template <typename T, typename A>
    void create_unsync_thread_method(std::string name, T func, A instance) {
        assert(OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1);
        OE_Main->create_unsync_thread(name, func_copy);
    }

    /** API functions for loading worlds/scenes/objects/etc.
     */

    template <typename T, typename... Args>
    void load_world_func(std::string filename, T func, Args... arguments) {

        oe::create_event("loaded-" + filename);
        oe::set_event_func("loaded-" + filename, func, std::forward<Args...>(arguments...));
        oe::create_unsync_thread_func("loading-" + filename, &OE_API_Helpers::load_world, filename);
    }

    template <typename T>
    void load_world_func(std::string filename, T func) {

        oe::create_event("loaded-" + filename);
        oe::set_event_func("loaded-" + filename, func);
        oe::create_unsync_thread_func("loading-" + filename, &OE_API_Helpers::load_world, filename);
    }

    template <typename T, typename A, typename... Args>
    void load_world_method(std::string filename, T func, A instance, Args... arguments) {

        oe::create_event("loaded-" + filename);
        oe::set_event_method("loaded-" + filename, func, instance, std::forward<Args...>(arguments...));
        oe::create_unsync_thread_func("loading-" + filename, &OE_API_Helpers::load_world, filename);
    }

    template <typename T, typename A>
    void load_world_method(std::string filename, T func, A instance) {

        oe::create_event("loaded-" + filename);
        oe::set_event_method("loaded-" + filename, func, instance);
        oe::create_unsync_thread_func("loading-" + filename, &OE_API_Helpers::load_world, filename);
    }

    /** API functions for manipulating objects and basic types
     * to be vastly extended when the physics engine comes
     */

    std::size_t get_object_id(std::string);
    std::string get_object_name(std::size_t);

    std::set<std::size_t> get_scene_objects(std::size_t);
    OE_Vec3               get_object_pos(std::size_t);
    OE_Vec3               get_object_model_matrix(std::size_t);
    OE_Quat               get_object_rot(std::size_t);

    void set_object_pos(std::size_t, OE_Vec3);
    void set_object_rot(std::size_t, OE_Quat);
    void set_object_rot(std::size_t, OE_Vec4);

    void change_object_pos(std::size_t, OE_Vec3);
    void change_object_rot(std::size_t, OE_Quat);
    void change_object_rot(std::size_t, OE_Vec4);

    void change_object_global_rot(std::size_t, OE_Quat);
    void change_object_global_rot(std::size_t, OE_Vec4);

    void change_object_local_pos(std::size_t, OE_Vec3);

    void set_object_scale(std::size_t, OE_Vec3);

    void change_object_scale(std::size_t, OE_Vec3);

    /** API functions to control the renderer
     *  These should work for ANY renderer
     */

    void                      restart_renderer();
    void                      set_shading_mode(oe::RENDERER_SHADING_MODE);
    oe::RENDERER_SHADING_MODE get_shading_mode();

    void render_wireframe(bool);
    void toggle_wireframe_rendering();

    void render_bounding_boxes(bool);
    void toggle_bounding_boxes_rendering();

    void render_bounding_spheres(bool);
    void toggle_bounding_spheres_rendering();

    void render_HDR(bool);
    void toggle_render_HDR();

    void render_software_vertex_shaders(bool);
    void toggle_software_vertex_shaders();

    void render_z_prepass(bool);
    void toggle_render_z_prepass();

    // Debug options
    void set_renderer_sanity_checks(bool);
    void toggle_renderer_sanity_checks();

    /** API functions to control the window system
     *  These should work for ANY window system
     */
    void        set_window_title(std::string title);
    std::string get_window_title();

    // not functional yet
    void toggle_window_fullscreen();
    void set_window_fullscreen(bool);
}; // namespace oe



#endif
