#include <OE_API.h>

using namespace std;
using namespace OE;

int update_monkey_rot(void*, OE_Task*, string event_name){

    OE_ChangeObjectLocalPos("Camera", OE_Vec3(0.0f, 0.0f, -0.1f));
    return 0;
}

int update_monkey_rot_neg(void*, OE_Task*, string event_name){

    OE_ChangeObjectLocalPos("Camera", OE_Vec3(0.0f, 0.0f, 0.1f));
    return 0;
}

int update_monkey_rot_x(void*, OE_Task*, string event_name){

    OE_ChangeObjectLocalPos("Camera", OE_Vec3(0.0f, 0.1f, 0.0f));
    //OE_ChangeObjectRot("Suzanne", OE_Vec4(0.1f, 1.0f, 0.0f, 0.0f));
    return 0;
}

int update_monkey_rot_neg_x(void*, OE_Task*, string event_name){

    OE_ChangeObjectLocalPos("Camera", OE_Vec3(0.0f, -0.1f, 0.0f));
    //OE_ChangeObjectRot("Suzanne", OE_Vec4(-0.1f, 1.0f, 0.0f, 0.0f));
    return 0;
}

int update_monkey_rot_z(void*, OE_Task*, string event_name){

    OE_ChangeObjectLocalPos("Camera", OE_Vec3(-0.1f, 0.0f, 0.0f));
    return 0;
}

int update_monkey_rot_neg_z(void*, OE_Task*, string event_name){

    OE_ChangeObjectLocalPos("Camera", OE_Vec3(0.1f, 0.0f, 0.0f));
    return 0;
}

int toggle_mouse_locked_state(void*, OE_Task*, string event_name){
    
    if (OE_IsMouseLocked()){
        OE_MouseUnlock();
    } else {
        OE_MouseLock();
    }
    return 0;
}

int set_renderer_mode_normals(void*, OE_Task*, string event_name){
    
    OE_SetShadingMode(OE_RENDERER_NORMALS_SHADING);
    return 0;
}

int set_renderer_mode_regular(void*, OE_Task*, string event_name){
    
    OE_SetShadingMode(OE_RENDERER_REGULAR_SHADING);
    return 0;
}

int renderer_toggle_wireframe(void*, OE_Task*, string event_name){
    
    OE_ToggleWireframeRendering();
    return 0;
}

int renderer_toggle_bounding_boxes(void*, OE_Task*, string event_name){
    
    OE_ToggleBoundingBoxesRendering();
    return 0;
}

int test_task1(void*, OE_Task task){
    
    if (OE_IsMouseMoved() && OE_IsMouseLocked()){
        float x = (float)OE_GetDeltaMouseX()/160.0f;
        float y = (float)OE_GetDeltaMouseY()/160.0f;
       
        OE_ChangeObjectGlobalRot("Camera", OE_Vec4(-x, 0.0f, 0.0f, 1.0f));
        OE_ChangeObjectRot("Camera", OE_Vec4(-y, 1.0f, 0.0f, 0.0f));
    }
    return task.CONTINUE();
}

int OnLoadObject(void* data, OE_Task* event_task, string event_name){
    cout << "SUCCESSFULLY loaded '" << event_name << "'" << endl;
    OE_SetEventFunc("keyboard-w", &update_monkey_rot, nullptr);
    OE_SetEventFunc("keyboard-s", &update_monkey_rot_neg, nullptr);
    OE_SetEventFunc("keyboard-q", &update_monkey_rot_x, nullptr);
    OE_SetEventFunc("keyboard-e", &update_monkey_rot_neg_x, nullptr);
    OE_SetEventFunc("keyboard-a", &update_monkey_rot_z, nullptr);
    OE_SetEventFunc("keyboard-d", &update_monkey_rot_neg_z, nullptr);
    
    OE_SetEventFunc("keyboard-w+", &update_monkey_rot, nullptr);
    OE_SetEventFunc("keyboard-s+", &update_monkey_rot_neg, nullptr);
    OE_SetEventFunc("keyboard-q+", &update_monkey_rot_x, nullptr);
    OE_SetEventFunc("keyboard-e+", &update_monkey_rot_neg_x, nullptr);
    OE_SetEventFunc("keyboard-a+", &update_monkey_rot_z, nullptr);
    OE_SetEventFunc("keyboard-d+", &update_monkey_rot_neg_z, nullptr);
    
    OE_SetEventFunc("keyboard-space+", &toggle_mouse_locked_state, nullptr);
    
    // Useful for debugging
    OE_SetEventFunc("keyboard-f5+", &set_renderer_mode_normals, nullptr);
    OE_SetEventFunc("keyboard-f6+", &set_renderer_mode_regular, nullptr);
    OE_SetEventFunc("keyboard-f7+", &renderer_toggle_wireframe, nullptr);
    OE_SetEventFunc("keyboard-f8+", &renderer_toggle_bounding_boxes, nullptr);
    
    OE_AddTask("test_task1", test_task1, nullptr);
    
    return 0;
}

int main(){
        
    OE_Init("Oxygen Engine Demo", 1280, 720, false);
    OE_Pause(20);
    //OE_LoadWorld("OE_Mat_light_test.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("challenge_car.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("monkeys.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("csl_very_large_object_test.csl", &OnLoadObject, nullptr);
    OE_LoadWorld("OE_Demo.csl", &OnLoadObject, nullptr);
    //taskMgr.Start();
    OE_Start();
    
    return 0;
}
