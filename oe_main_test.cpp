#include <OE_API.h>

using namespace std;
using namespace OE;

int update_camera_pos(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                object.second->current_state.pos_z = object.second->current_state.pos_z - 0.1f;
            }
        }
    }
    return 0;
}

int update_camera_pos_neg(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                object.second->current_state.pos_z = object.second->current_state.pos_z + 0.1f;
            }
        }
    }
    return 0;
}

int update_camera_pos_y(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                object.second->current_state.pos_x = object.second->current_state.pos_x - 0.1f;
            }
        }
    }
    return 0;
}

int update_camera_pos_neg_y(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                object.second->current_state.pos_x = object.second->current_state.pos_x + 0.1f;
            }
        }
    }
    return 0;
}

int OnLoadObject(void*, OE_Task* event_task, string event_name){
    cout << "SUCCESSFULLY loaded '" << event_name << "'" << endl;
    OE_SetEventFunc("keyboard-w", &update_camera_pos, nullptr);
    OE_SetEventFunc("keyboard-s", &update_camera_pos_neg, nullptr);
    OE_SetEventFunc("keyboard-a", &update_camera_pos_y, nullptr);
    OE_SetEventFunc("keyboard-d", &update_camera_pos_neg_y, nullptr);
    return 0;
}

int main(){
        
    //OE_TaskManager taskMgr = OE_TaskManager();
    
    //taskMgr.Init();
    OE_Init("Oxygen Engine Demo", 1280, 720, false);
    OE_Pause(20);
    OE_LoadWorld("monkeys.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("fe_export_test.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("csl_very_large_object_test.csl", &OnLoadObject, nullptr);
    //taskMgr.Start();
    OE_Start();
    
    return 0;
}

#ifdef OE_PLATFORM_WINDOWS
int WInMain(){return 0;}
#endif
