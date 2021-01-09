#include <OE_API.h>

using namespace std;

int update_monkey_rot( oe::task, string event_name, string obj_name){

    oe::change_object_local_pos(obj_name, oe::vec3(0.0f, 0.0f, -0.1f));
    return 0;
}

int update_monkey_rot_neg( oe::task, string event_name){

    oe::change_object_local_pos("\"Camera\"", oe::vec3(0.0f, 0.0f, 0.1f));
    return 0;
}

int update_monkey_rot_x( oe::task, string event_name){

    oe::change_object_local_pos("\"Camera\"", oe::vec3(0.0f, 0.1f, 0.0f));
    //oe::ChangeObjectRot("Suzanne", oe::vec4(0.1f, 1.0f, 0.0f, 0.0f));
    //oe::ChangeObjectScale("Suzanne", oe::vec3(0.0f, 0.1f, 0.0f));
    return 0;
}

int update_monkey_rot_neg_x( oe::task, string event_name){

    oe::change_object_local_pos("\"Camera\"", oe::vec3(0.0f, -0.1f, 0.0f));
    //oe::ChangeObjectRot("Suzanne", oe::vec4(-0.1f, 1.0f, 0.0f, 0.0f));
    //oe::ChangeObjectScale("Suzanne", oe::vec3(0.0f, -0.1f, 0.0f));
    return 0;
}

int update_monkey_rot_z( oe::task, string event_name){

    oe::change_object_local_pos("\"Camera\"", oe::vec3(-0.1f, 0.0f, 0.0f));
    return 0;
}

int update_monkey_rot_neg_z( oe::task, string event_name){

    oe::change_object_local_pos("\"Camera\"", oe::vec3(0.1f, 0.0f, 0.0f));
    return 0;
}

int toggle_mouse_locked_state( oe::task, string event_name){
    
    if (oe::is_mouse_locked()){
        oe::mouse_unlock();
    } else {
        oe::mouse_lock();
    }
    return 0;
}

int set_renderer_mode_normals( oe::task, string event_name){
    
    oe::set_shading_mode(oe::renderer::shading_mode::normals);
    return 0;
}

int set_renderer_mode_regular( oe::task, string event_name){
    
    oe::set_shading_mode(oe::renderer::shading_mode::regular);
    return 0;
}

int renderer_toggle_wireframe( oe::task, string event_name){
    
    oe::toggle_wireframe_rendering();
    return 0;
}

int renderer_toggle_bounding_boxes( oe::task, string event_name){
    
    oe::toggle_bounding_boxes_rendering();
    //throw 5;
    return 0;
}

int test_task1(oe::task task, std::string obj_name){
    
    if (oe::is_mouse_moved() && oe::is_mouse_locked()){
        float x = (float)oe::get_delta_mouse_x()/160.0f;
        float y = (float)oe::get_delta_mouse_y()/160.0f;
       
        oe::change_object_global_rot(obj_name, oe::vec4(-x, 0.0f, 0.0f, 1.0f));
        oe::change_object_rot(obj_name, oe::vec4(-y, 1.0f, 0.0f, 0.0f));
    }
    
    //cout << " task1 " << endl;
    return task.CONTINUE();
}

int test_task0(oe::task task){
    
    //cout << " task0";
    std::map<int, int> empty_test_map;
    
    if (oe::is_key_just_pressed("keyboard-p")){
        int a = empty_test_map.at(1);
        cout << a << endl;
    }
    
    
    return task.CONTINUE();
}

int test_task2(oe::task task){
    
    //cout << " task2";
    
    auto a = oe::get_task_info("default", "paparounes");
    cout << a.GetCounter() << endl;
    
    return task.CONTINUE();
}

int test_task3(oe::task task){
    
    //cout << " task3";
    if (oe::is_key_just_pressed("keyboard-va")){
        //int a = empty_test_map.at(1);
        cout << 2 << endl;
    }
    
    
    return task.CONTINUE();
}


int OnLoadObject(oe::task load_event_task, string event_name){
    cout << "SUCCESSFULLY loaded '" << event_name << "'" << endl;
    
    oe::set_event_func("keyboard-w", &update_monkey_rot, "\"Camera\"");
    oe::set_event_func("keyboard-s", &update_monkey_rot_neg);
    oe::set_event_func("keyboard-q", &update_monkey_rot_x);
    oe::set_event_func("keyboard-e", &update_monkey_rot_neg_x);
    oe::set_event_func("keyboard-a", &update_monkey_rot_z);
    oe::set_event_func("keyboard-d", &update_monkey_rot_neg_z);
    
    oe::set_event_func("keyboard-w+", &update_monkey_rot,  "\"Camera\"");
    oe::set_event_func("keyboard-s+", &update_monkey_rot_neg);
    oe::set_event_func("keyboard-q+", &update_monkey_rot_x);
    oe::set_event_func("keyboard-e+", &update_monkey_rot_neg_x);
    oe::set_event_func("keyboard-a+", &update_monkey_rot_z);
    oe::set_event_func("keyboard-d+", &update_monkey_rot_neg_z);
    
    oe::set_event_func("keyboard-space+", &toggle_mouse_locked_state);
    
    // Useful for debugging
    oe::set_event_func("keyboard-f5+", &set_renderer_mode_normals);
    oe::set_event_func("keyboard-f6+", &set_renderer_mode_regular);
    oe::set_event_func("keyboard-f7+", &renderer_toggle_wireframe);
    oe::set_event_func("keyboard-f8+", &renderer_toggle_bounding_boxes);
    
    oe::add_task_func("test_task1", &test_task1, "\"Camera\"");
    oe::add_task_func("test_task0", 2, &test_task0);
    oe::add_task_func("test_task2", 3, &test_task2);
    oe::add_task_func("test_task3", 4, &test_task3);
    
    return 0;
}

int main(){
    
    oe::init("Oxygen Engine Demo", 1280, 720, false);
    //oe::pause(20);

	//oe::load_world_func("OE_Mat_light_test.csl", &OnLoadObject);
	//oe::load_world_func("challenge_car.csl", &OnLoadObject);
	//oe::load_world_func("monkeys.csl", &OnLoadObject);
	//oe::load_world_func("csl_very_large_object_test.csl", &OnLoadObject);
	//oe::load_world_func("OE_VerySimple.csl", &OnLoadObject);
	oe::load_world_func("OE_Demo.csl", &OnLoadObject);
	
	//taskMgr.Start();
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
