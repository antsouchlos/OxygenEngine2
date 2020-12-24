#include <OE_API.h>

using namespace std;
//using namespace OE;

int update_monkey_rot(void*, OE_Task*, string event_name){

    oe::change_object_local_pos("Camera", OE_Vec3(0.0f, 0.0f, -0.1f));
    return 0;
}

int update_monkey_rot_neg(void*, OE_Task*, string event_name){

    oe::change_object_local_pos("Camera", OE_Vec3(0.0f, 0.0f, 0.1f));
    return 0;
}

int update_monkey_rot_x(void*, OE_Task*, string event_name){

    oe::change_object_local_pos("Camera", OE_Vec3(0.0f, 0.1f, 0.0f));
    //oe::ChangeObjectRot("Suzanne", OE_Vec4(0.1f, 1.0f, 0.0f, 0.0f));
    //oe::ChangeObjectScale("Suzanne", OE_Vec3(0.0f, 0.1f, 0.0f));
    return 0;
}

int update_monkey_rot_neg_x(void*, OE_Task*, string event_name){

    oe::change_object_local_pos("Camera", OE_Vec3(0.0f, -0.1f, 0.0f));
    //oe::ChangeObjectRot("Suzanne", OE_Vec4(-0.1f, 1.0f, 0.0f, 0.0f));
    //oe::ChangeObjectScale("Suzanne", OE_Vec3(0.0f, -0.1f, 0.0f));
    return 0;
}

int update_monkey_rot_z(void*, OE_Task*, string event_name){

    oe::change_object_local_pos("Camera", OE_Vec3(-0.1f, 0.0f, 0.0f));
    return 0;
}

int update_monkey_rot_neg_z(void*, OE_Task*, string event_name){

    oe::change_object_local_pos("Camera", OE_Vec3(0.1f, 0.0f, 0.0f));
    return 0;
}

int toggle_mouse_locked_state(void*, OE_Task*, string event_name){
    
    if (oe::is_mouse_locked()){
        oe::mouse_unlock();
    } else {
        oe::mouse_lock();
    }
    return 0;
}

int set_renderer_mode_normals(void*, OE_Task*, string event_name){
    
    oe::set_shading_mode(OE_RENDERER_NORMALS_SHADING);
    return 0;
}

int set_renderer_mode_regular(void*, OE_Task*, string event_name){
    
    oe::set_shading_mode(OE_RENDERER_REGULAR_SHADING);
    return 0;
}

int renderer_toggle_wireframe(void*, OE_Task*, string event_name){
    
    oe::toggle_wireframe_rendering();
    return 0;
}

int renderer_toggle_bounding_boxes(void*, OE_Task*, string event_name){
    
    oe::toggle_bounding_boxes_rendering();
    return 0;
}

int test_task1(void*, OE_Task task){
    
    if (oe::is_mouse_moved() && oe::is_mouse_locked()){
        float x = (float)oe::get_delta_mouse_x()/160.0f;
        float y = (float)oe::get_delta_mouse_y()/160.0f;
       
        oe::change_object_global_rot("Camera", OE_Vec4(-x, 0.0f, 0.0f, 1.0f));
        oe::change_object_rot("Camera", OE_Vec4(-y, 1.0f, 0.0f, 0.0f));
    }
    return task.CONTINUE();
}

int OnLoadObject(void* data, OE_Task* event_task, string event_name){
    cout << "SUCCESSFULLY loaded '" << event_name << "'" << endl;
    
    oe::set_event_func("keyboard-w", &update_monkey_rot, nullptr);
    oe::set_event_func("keyboard-s", &update_monkey_rot_neg, nullptr);
    oe::set_event_func("keyboard-q", &update_monkey_rot_x, nullptr);
    oe::set_event_func("keyboard-e", &update_monkey_rot_neg_x, nullptr);
    oe::set_event_func("keyboard-a", &update_monkey_rot_z, nullptr);
    oe::set_event_func("keyboard-d", &update_monkey_rot_neg_z, nullptr);
    
    oe::set_event_func("keyboard-w+", &update_monkey_rot, nullptr);
    oe::set_event_func("keyboard-s+", &update_monkey_rot_neg, nullptr);
    oe::set_event_func("keyboard-q+", &update_monkey_rot_x, nullptr);
    oe::set_event_func("keyboard-e+", &update_monkey_rot_neg_x, nullptr);
    oe::set_event_func("keyboard-a+", &update_monkey_rot_z, nullptr);
    oe::set_event_func("keyboard-d+", &update_monkey_rot_neg_z, nullptr);
    
    oe::set_event_func("keyboard-space+", &toggle_mouse_locked_state, nullptr);
    
    // Useful for debugging
    oe::set_event_func("keyboard-f5+", &set_renderer_mode_normals, nullptr);
    oe::set_event_func("keyboard-f6+", &set_renderer_mode_regular, nullptr);
    oe::set_event_func("keyboard-f7+", &renderer_toggle_wireframe, nullptr);
    oe::set_event_func("keyboard-f8+", &renderer_toggle_bounding_boxes, nullptr);
    
    oe::add_task("test_task1", test_task1, nullptr);
    
    return 0;
}

int main(){
        
    oe::init("Oxygen Engine Demo", 1280, 720, false);
    oe::pause(20);
    //oe::load_world("OE_Mat_light_test.csl", &OnLoadObject, nullptr);
    //oe::load_world("challenge_car.csl", &OnLoadObject, nullptr);
    //oe::load_world("monkeys.csl", &OnLoadObject, nullptr);
    //oe::load_world("csl_very_large_object_test.csl", &OnLoadObject, nullptr);
    //oe::load_world("OE_VerySImple.csl", &OnLoadObject, nullptr);
    oe::load_world("OE_Demo.csl", &OnLoadObject, nullptr);
    //taskMgr.Start();
    oe::start();
    
    return 0;
}
