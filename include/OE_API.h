#ifndef OE_API_H_INCLUDED
#define OE_API_H_INCLUDED

#include <OE_Math.h>
#include <OE_TaskManager.h>
#include <OE_API_Helpers.h>


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

namespace oe{
    
    // GLOBAL VARIABLE
    extern OE_TaskManager* OE_Main;
    
    // class typedefs
    
    typedef OE_Task     task;
    typedef OE_Vec3     vec3;
    typedef OE_Vec4     vec4;
    typedef OE_Mat4x4   mat4x4;
    typedef OE_Quat     quat;
    typedef OE_Mat3x3   mat3x3;
    
    // enum typedefs
    
    typedef OE_METHOD       method_type;
    typedef OE_EVENTFUNC    event_func_type;
    
    namespace os{
        typedef OE_OS type;
        const type undefined = (type)0;
        const type linux = (type)1;
        const type windows = (type)2;
    };
    
    namespace winsys{
        typedef OE_WINSYS type;
        const type none=(type)0;
        const type sdl = (type)1;
        const type something_else = (type)2;
    };
    
    namespace renderer{
        
        namespace shading_mode{
            
            typedef OE_RENDERER_SHADING_MODE type;
            const type normals = (type)0;
            const type no_light = (type)1;
            const type dir_lights = (type)2;
            const type indexed_lights = (type)3;
            const type regular = (type)4;
        };
    };
    
    /** Basic API functions for starting the Oxygen Engine
     *  and assigning tasks
     */
    size_t init();
    size_t init(std::string, bool);
    size_t init(std::string, int, int, bool);
    void step();
    void start();
    void destroy();
    bool is_done();
    void finish();
    //size_t initFromFile(std::string); //TODO
    
    //------------------------BLOCK-------------------------//
    // The API functions inside this block are the only ones that are 
    // safe to be called on an unsynchronized thread together with all the get* functions
    
    // plain add task
    template<typename T, typename... Args>
    void add_task_func(std::string name, T func, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->AddTask(name, func_copy);
    }
    
    template<typename T>
    void add_task_func(std::string name, T func){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  std::placeholders::_1);
        OE_Main->AddTask(name, func_copy);
    }
    
    template<typename T, typename A, typename... Args>
    void add_task_method(std::string name, T func, A instance, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->AddTask(name, func_copy);
    }
    
    template<typename T,typename A>
    void add_task_method(std::string name, T func, A instance){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1);
        OE_Main->AddTask(name, func_copy);
    }
    
    // with priority
    template<typename T, typename... Args>
    void add_task_func(std::string name, int priority, T func, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->AddTask(name, func_copy, priority);
    }
    
    template<typename T>
    void add_task_func(std::string name, int priority, T func){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  std::placeholders::_1);
        OE_Main->AddTask(name, func_copy, priority);
    }
    
    template<typename T, typename A, typename... Args>
    void add_task_method(std::string name, int priority, T func, A instance, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->AddTask(name, func_copy, priority);
    }
    
    template<typename T,typename A>
    void add_task_method(std::string name, int priority, T func, A instance){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1);
        OE_Main->AddTask(name, func_copy, priority);
    }
    
    // with priority and threadname
    template<typename T, typename... Args>
    void add_task_func(std::string name, std::string threadname, int priority, T func, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->AddTask(name, func_copy, priority, threadname);
    }
    
    template<typename T>
    void add_task_func(std::string name, std::string threadname, int priority, T func){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  std::placeholders::_1);
        OE_Main->AddTask(name, func_copy, priority, threadname);
    }
    
    template<typename T, typename A, typename... Args>
    void add_task_method(std::string name, std::string threadname, int priority, T func, A instance, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->AddTask(name, func_copy, priority, threadname);
    }
    
    template<typename T,typename A>
    void add_task_method(std::string name, std::string threadname,  int priority, T func, A instance){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1);
        OE_Main->AddTask(name, func_copy, priority, threadname);
    }
    
    // with threadname
    template<typename T, typename... Args>
    void add_task_func(std::string name, std::string threadname, T func, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->AddTask(name, func_copy, threadname);
    }
    
    template<typename T>
    void add_task_func(std::string name, std::string threadname, T func){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  std::placeholders::_1);
        OE_Main->AddTask(name, func_copy, threadname);
    }
    
    template<typename T, typename A, typename... Args>
    void add_task_method(std::string name, std::string threadname, T func, A instance, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->AddTask(name, func_copy, threadname);
    }
    
    template<typename T,typename A>
    void add_task_method(std::string name, std::string threadname, T func, A instance){
        
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1);
        OE_Main->AddTask(name, func_copy, threadname);
    }
    
    void remove_task(std::string);
    void remove_task(std::string, std::string);
    
    void broadcast_event(std::string);
    void create_event(std::string);
    
    template<typename T, typename... Args>
    void set_event_func(std::string name, T func, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::event_func_type func_copy = std::bind(func,  std::placeholders::_1, std::placeholders::_2, std::forward<Args...>(arguments...));
        OE_Main->window->event_handler.setIEventFunc(name, func_copy);
    }
    
    template<typename T>
    void set_event_func(std::string name, T func){
        
        assert (OE_Main != nullptr);
        oe::event_func_type func_copy = std::bind(func,  std::placeholders::_1, std::placeholders::_2);
        OE_Main->window->event_handler.setIEventFunc(name, func_copy);
    }
    
    template<typename T, typename A, typename... Args>
    void set_event_method(std::string name, T func, A instance, Args... arguments){
        
        assert (OE_Main != nullptr);
        oe::event_func_type func_copy = std::bind(func,  instance, std::placeholders::_1, std::placeholders::_2, std::forward<Args...>(arguments...));
        OE_Main->window->event_handler.setIEventFunc(name, func_copy);
    }
    
    template<typename T, typename A>
    void set_event_method(std::string name, T func, A instance){
        
        assert (OE_Main != nullptr);
        oe::event_func_type func_copy = std::bind(func,  instance, std::placeholders::_1, std::placeholders::_2);
        OE_Main->window->event_handler.setIEventFunc(name, func_copy);
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
    
    OE_Task get_task_info(std::string, std::string);
    
    void create_new_thread(std::string);
    
    template<typename T, typename... Args>
    void create_unsync_thread_func(std::string name, T func, Args... arguments){
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->CreateUnsyncThread(name, func_copy);
    }
    
    template<typename T>
    void create_unsync_thread_func(std::string name, T func){
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, std::placeholders::_1);
        OE_Main->CreateUnsyncThread(name, func_copy);
    }
    
    template<typename T, typename A, typename... Args>
    void create_unsync_thread_method(std::string name, T func, A instance, Args... arguments){
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func, instance, std::placeholders::_1, std::forward<Args...>(arguments...));
        OE_Main->CreateUnsyncThread(name, func_copy);
    }
    
    template<typename T, typename A>
    void create_unsync_thread_method(std::string name, T func, A instance){
        assert (OE_Main != nullptr);
        oe::method_type func_copy = std::bind(func,  instance, std::placeholders::_1);
        OE_Main->CreateUnsyncThread(name, func_copy);
    }
    
    /** API functions for loading worlds/scenes/objects/etc.
     */
    
    template<typename T, typename... Args>
    void load_world_func(std::string filename, T func, Args... arguments){
        
        oe::create_event("loaded-" + filename);
        oe::set_event_func("loaded-" + filename, func, std::forward<Args...>(arguments...));
        oe::create_unsync_thread_func("loading-" + filename, &OE_API_Helpers::load_world, filename);
    }
    
    template<typename T>
    void load_world_func(std::string filename, T func){
        
        oe::create_event("loaded-" + filename);
        oe::set_event_func("loaded-" + filename, func);
        oe::create_unsync_thread_func("loading-" + filename, &OE_API_Helpers::load_world, filename);
    }
    
    template<typename T, typename A, typename... Args>
    void load_world_method(std::string filename, T func, A instance, Args... arguments){
        
        oe::create_event("loaded-" + filename);
        oe::set_event_method("loaded-" + filename, func, instance, std::forward<Args...>(arguments...));
        oe::create_unsync_thread_func("loading-" + filename, &OE_API_Helpers::load_world, filename);
    }
    
    template<typename T, typename A>
    void load_world_method(std::string filename, T func, A instance){
        
        oe::create_event("loaded-" + filename);
        oe::set_event_method("loaded-" + filename, func, instance);
        oe::create_unsync_thread_func("loading-" + filename, &OE_API_Helpers::load_world, filename);
    }
    
    /** API functions for manipulating objects and basic types
     * to be vastly extended when the physics engine comes
     */
    
    std::size_t             get_object_id(std::string);
    std::string             get_object_name(std::size_t);
    
    std::set<std::size_t>   get_scene_objects(std::size_t);
    OE_Vec3                 get_object_pos(std::size_t);
    OE_Vec3                 get_object_model_matrix(std::size_t);
    OE_Quat                 get_object_rot(std::size_t);
    
    std::set<std::size_t>   get_scene_objects(std::string);
    OE_Vec3                 get_object_pos(std::string);
    OE_Vec3                 get_object_model_matrix(std::string);
    OE_Quat                 get_object_rot(std::string);
    
    void set_object_pos(std::size_t, OE_Vec3);
    void set_object_rot(std::size_t, OE_Quat);
    void set_object_rot(std::size_t, OE_Vec4);
    
    void change_object_pos(std::size_t, OE_Vec3);
    void change_object_rot(std::size_t, OE_Quat);
    void change_object_rot(std::size_t, OE_Vec4);
    
    void set_object_pos(std::string, OE_Vec3);
    void set_object_rot(std::string, OE_Quat);
    void set_object_rot(std::string, OE_Vec4);
    
    void change_object_pos(std::string, OE_Vec3);
    void change_object_rot(std::string, OE_Quat);
    void change_object_rot(std::string, OE_Vec4);
    
    void change_object_global_rot(std::size_t, OE_Quat);
    void change_object_global_rot(std::size_t, OE_Vec4);
    void change_object_global_rot(std::string, OE_Quat);
    void change_object_global_rot(std::string, OE_Vec4);
    
    void change_object_local_pos(std::size_t, OE_Vec3);
    void change_object_local_pos(std::string, OE_Vec3);
    
    void set_object_scale(std::size_t, OE_Vec3);
    void set_object_scale(std::string, OE_Vec3);
    
    void change_object_scale(std::size_t, OE_Vec3);
    void change_object_scale(std::string, OE_Vec3);
    
    /** API functions to control the renderer
     *  These should work for ANY renderer
     * All those parameters require a call to OE_RestartRenderer to take effect
     */
    
    void restart_renderer();
    void set_shading_mode(oe::renderer::shading_mode::type);
    oe::renderer::shading_mode::type get_shading_mode();
    
    void render_wireframe(bool);
    void toggle_wireframe_rendering();
    
    void render_bounding_boxes(bool);
    void toggle_bounding_boxes_rendering();
    
    void render_bounding_spheres(bool);
    void toggle_bounding_spheres_rendering();
    
    // These two do not require a call to OE_RestartRenderer
    void render_HDR(bool);
    void toggle_render_HDR();
};















#endif
