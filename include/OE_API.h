#ifndef OE_API_H_INCLUDED
#define OE_API_H_INCLUDED

#include <OE_TaskManager.h>
#include <OE_API_Helpers.h>

// This is going to be a very long header with ALL avaiable API functions declared
// TODO: Make this extern "C" so that we have a C compatible API (Long-term goal)

namespace oe{
    
    // GLOBAL VARIABLE
    extern OE_TaskManager* OE_Main;
    
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
    
    void add_task(std::string, const OE_METHOD, void*);
    void add_task(std::string, const OE_METHOD, int, void*);
    void add_task(std::string, const OE_METHOD, int, std::string, void*);
    void add_task(std::string, const OE_METHOD, std::string, void*);
    
    void remove_task(std::string);
    void remove_task(std::string, std::string);
    
    void broadcast_event(std::string, void*);
    void create_event(std::string);
    void set_event_func(std::string, const OE_EVENTFUNC, void*);
    
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
    void create_unsync_thread(std::string, const OE_METHOD, void*);
    
    /** API functions for loading worlds/scenes/objects/etc.
     */
    
    void load_world(std::string, const OE_EVENTFUNC, void*);
    
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
    void set_shading_mode(OE_RENDERER_SHADING_MODE);
    OE_RENDERER_SHADING_MODE get_shading_mode();
    
    void render_wireframe(bool);
    void toggle_wireframe_rendering();
    
    void render_bounding_boxes(bool);
    void toggle_bounding_boxes_rendering();
};















#endif
