#include <OE/api_oe.h>

using namespace std;

oe::task_action update_monkey_rot(const oe::task_info_t, string obj_name){

    size_t obj_id = oe::get_object_id(obj_name);
    oe::change_object_local_pos(obj_id, oe::vec3(0.0f, 0.0f, -0.1f));
    return oe::task_action::keep;
}

oe::task_action update_monkey_rot_neg(const oe::task_info_t){

    size_t camera_id = oe::get_object_id("Camera");
    oe::change_object_local_pos(camera_id, oe::vec3(0.0f, 0.0f, 0.1f));
    return oe::task_action::keep;
}

oe::task_action update_monkey_rot_x(const oe::task_info_t){

    size_t camera_id = oe::get_object_id("Camera");
    //size_t suzanne_id = oe::get_object_id("Suzanne");
    oe::change_object_local_pos(camera_id,  oe::vec3(0.0f, 0.1f, 0.0f));
    //oe::change_object_rot(suzanne_id, oe::vec4(0.1f, 1.0f, 0.0f, 0.0f));
    //oe::change_object_scale(suzanne_id, oe::vec3(0.0f, 0.1f, 0.0f));
    return oe::task_action::keep;
}

oe::task_action update_monkey_rot_neg_x(const oe::task_info_t){

    size_t camera_id = oe::get_object_id("Camera");
    //size_t suzanne_id = oe::get_object_id("Suzanne");
    oe::change_object_local_pos(camera_id, oe::vec3(0.0f, -0.1f, 0.0f));
    //oe::change_object_rot(suzanne_id, oe::vec4(-0.1f, 1.0f, 0.0f, 0.0f));
    //oe::change_object_scale(suzanne_id, oe::vec3(0.0f, -0.1f, 0.0f));
    return oe::task_action::keep;
}

oe::task_action update_monkey_rot_z(const oe::task_info_t){

    size_t camera_id = oe::get_object_id("Camera");
    oe::change_object_local_pos(camera_id, oe::vec3(-0.1f, 0.0f, 0.0f));
    return oe::task_action::keep;
}

oe::task_action update_monkey_rot_neg_z(const oe::task_info_t){

    size_t camera_id = oe::get_object_id("Camera");
    oe::change_object_local_pos(camera_id, oe::vec3(0.1f, 0.0f, 0.0f));
    return oe::task_action::keep;
}

oe::task_action toggle_mouse_locked_state(const oe::task_info_t){

    if (oe::is_mouse_locked()){
        oe::mouse_unlock();
    } else {
        oe::mouse_lock();
    }
    return oe::task_action::keep;
}

oe::task_action renderer_toggle_bounding_spheres(const oe::task_info_t){
    
    oe::toggle_bounding_spheres_rendering();
    return oe::task_action::keep;
}

oe::task_action set_renderer_mode_normals(const oe::task_info_t){
    
    if (oe::get_shading_mode() == oe::RENDERER_REGULAR_SHADING)
        oe::set_shading_mode(oe::RENDERER_NORMALS_SHADING);
    else
        oe::set_shading_mode(oe::RENDERER_REGULAR_SHADING);
    return oe::task_action::keep;
}

oe::task_action load_object_handler(const oe::task_info_t);

oe::task_action renderer_toggle_wireframe(const oe::task_info_t){
    
    oe::toggle_wireframe_rendering();
    //oe::toggle_render_HDR();
    return oe::task_action::keep;
}

oe::task_action renderer_toggle_bounding_boxes(const oe::task_info_t){
    
    oe::toggle_bounding_boxes_rendering();
    //throw 5;
    return oe::task_action::keep;
}

oe::task_action test_task1(const oe::task_info_t task, std::string obj_name){

    size_t obj_id = oe::get_object_id(obj_name);

    if (oe::is_mouse_moved() && oe::is_mouse_locked()){
        float x = (float)oe::get_delta_mouse_x()/160.0f;
        float y = (float)oe::get_delta_mouse_y()/160.0f;
       
        oe::change_object_global_rot(obj_id, oe::vec4(-x, 0.0f, 0.0f, 1.0f));
        oe::change_object_rot(obj_id, oe::vec4(-y, 1.0f, 0.0f, 0.0f));
    }
    
    //cout << " task1 " << endl;
    return oe::task_action::keep;
}

oe::task_action test_task0(const oe::task_info_t task){
    
    //cout << " task0";
    std::map<int, int> empty_test_map;
    
    if (oe::is_key_just_pressed("keyboard-p")){
        oe::create_event("keyboard-a");
    }
    
    
    return oe::task_action::keep;
}

oe::task_action test_task2(const oe::task_info_t task){
    
    //cout << " task2";
    
    auto a = oe::get_task_info("default", "paparounes");
    cout << a.get_counter() << endl;
    
    return oe::task_action::keep;
}

oe::task_action test_task3(const oe::task_info_t task){

    //cout << " task3";
    if (oe::is_key_just_pressed("keyboard-1")){
        oe::load_world_func("assets/OE_Demo_50MB.csl", &load_object_handler);
    }
    else if (oe::is_key_just_pressed("keyboard-2")){
        oe::load_world_func("assets/OE_Demo2.csl", &load_object_handler);
    }
    else if (oe::is_key_just_pressed("keyboard-3")){
        oe::load_world_func("assets/monkeys.csl", &load_object_handler);
    }
    else if (oe::is_key_just_pressed("keyboard-4")){
        oe::load_world_func("assets/car_gma950.csl", &load_object_handler);
    }
    else if (oe::is_key_just_pressed("keyboard-5")){
        oe::load_world_func("assets/OE_VerySimple.csl", &load_object_handler);
    }
    else {

    }

    return oe::task_action::keep;
}

oe::task_action toggle_debug_mode(const oe::task_info_t task){
    //oe::toggle_renderer_sanity_checks();
    oe::toggle_software_vertex_shaders();
    return oe::task_action::keep;
}

oe::task_action load_object_handler(const oe::task_info_t load_event_task){
    cout << "SUCCESSFULLY loaded '" << oe::get_event_name(load_event_task.get_id()) << "'" << endl;
    
    oe::set_event_func("keyboard-w", &update_monkey_rot, "Camera");
    oe::set_event_func("keyboard-s", &update_monkey_rot_neg);
    oe::set_event_func("keyboard-q", &update_monkey_rot_x);
    oe::set_event_func("keyboard-e", &update_monkey_rot_neg_x);
    oe::set_event_func("keyboard-a", &update_monkey_rot_z);
    oe::set_event_func("keyboard-d", &update_monkey_rot_neg_z);
    
    oe::set_event_func("keyboard-w+", &update_monkey_rot,  "Camera");
    oe::set_event_func("keyboard-s+", &update_monkey_rot_neg);
    oe::set_event_func("keyboard-q+", &update_monkey_rot_x);
    oe::set_event_func("keyboard-e+", &update_monkey_rot_neg_x);
    oe::set_event_func("keyboard-a+", &update_monkey_rot_z);
    oe::set_event_func("keyboard-d+", &update_monkey_rot_neg_z);
    
    oe::set_event_func("keyboard-space+", &toggle_mouse_locked_state);
    // Useful for debugging
    oe::set_event_func("keyboard-f5+", &set_renderer_mode_normals);
    oe::set_event_func("keyboard-f6+", &renderer_toggle_bounding_spheres);
    oe::set_event_func("keyboard-f7+", &renderer_toggle_wireframe);
    oe::set_event_func("keyboard-f8+", &renderer_toggle_bounding_boxes);
    oe::set_event_func("keyboard-f9+", &toggle_debug_mode);
    
    oe::add_task_func("test_task1", &test_task1, "Camera");
    oe::add_task_func("test_task0", 2, &test_task0);
    oe::add_task_func("test_task2", 3, &test_task2);
    oe::add_task_func("test_task3", 4, &test_task3);
    oe::mouse_lock();

    oe::set_window_title("Oxygen Engine - " + oe::get_event_name(load_event_task.get_id()));
   return oe::task_action::keep;
}

int main(){
    
    // everything in namespace oe::preinit needs to be run before oe::init() to take effect
    oe::preinit::request_gles2();

    oe::init("Oxygen Engine Demo", 852, 480, false);
    //oe::pause(20);
	//oe::load_world_func("assets/OE_Mat_light_test.csl", &load_object_handler);
	//oe::load_world_func("assets/challenge_car.csl", &load_object_handler);
	//oe::load_world_func("assets/monkeys.csl", &load_object_handler);
	//oe::load_world_func("assets/csl_very_large_object_test.csl", &load_object_handler);
	//oe::load_world_func("assets/OE_VerySimple.csl", &load_object_handler);
	//oe::load_world_func("assets/some_car.csl", &load_object_handler);
    oe::load_world_func("assets/OE_Demo.csl", &load_object_handler);
    //oe::load_world_func("assets/OE_Demo2.csl", &load_object_handler);
	//oe::load_world_func("assets/lights_simple.csl", &load_object_handler);
	//taskMgr.Start();
    //cout << "this has started " << oe::OE_Main << endl;
    oe::start();
    
    
    if (__cplusplus > 201703L){
        cout << "C++20" << endl;
    }
    else if (__cplusplus == 201703L){
        cout << "C++17" << endl;
    }
    else {
        cout << __cplusplus << endl;
    }
    return 0;
}
