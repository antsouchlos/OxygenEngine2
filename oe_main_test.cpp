#include <OE_API.h>

using namespace std;
using namespace OE;

int update_monkey_rot(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                //object.second->current_state.pos_z = object.second->current_state.pos_z - 0.1f;
                object.second->SetRot(object.second->GetRot()* OE_QuatFromAxisAngle(0.05f, 1.0f, 0.0f, 0.0f));
            }
        }
    }
    return 0;
}

int update_monkey_rot_neg(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (object.second->getType() == "CAMERA"){
                //object.second->current_state.pos_z = object.second->current_state.pos_z + 0.1f;
                object.second->SetRot(object.second->GetRot()* OE_QuatFromAxisAngle(-0.05f, 1.0f, 0.0f, 0.0f));
            }
        }
    }
    return 0;
}

int update_monkey_rot_y(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
    for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (OE_Object::id2name[object.first] == "Suzanne.001"){
                object.second->SetRot(object.second->GetRot()* OE_QuatFromAxisAngle(0.03f, 1.0f, 0.0f, 0.0f));
            }
        }
    }
    return 0;
}

int update_monkey_rot_neg_y(void*, OE_Task*, string event_name){
    //cout << "PRESSED W" << endl;
    // HACK
     for (auto scene : OE_Main->world->scenes){
        for (auto object : scene.second->objects){
            if (OE_Object::id2name[object.first] == "Suzanne.001"){
                object.second->SetRot(object.second->GetRot()* OE_QuatFromAxisAngle(-0.03f, 1.0f, 0.0f, 0.0f));
            }
        }
    }
    return 0;
}

int OnLoadObject(void*, OE_Task* event_task, string event_name){
    cout << "SUCCESSFULLY loaded '" << event_name << "'" << endl;
    OE_SetEventFunc("keyboard-w", &update_monkey_rot, nullptr);
    OE_SetEventFunc("keyboard-s", &update_monkey_rot_neg, nullptr);
    OE_SetEventFunc("keyboard-a", &update_monkey_rot_y, nullptr);
    OE_SetEventFunc("keyboard-d", &update_monkey_rot_neg_y, nullptr);
    return 0;
}

int main(){
        
    //OE_TaskManager taskMgr = OE_TaskManager();
    
    //taskMgr.Init();
    OE_Init("Oxygen Engine Demo", 1280, 720, false);
    OE_Pause(20);
    OE_LoadWorld("csl_2UVMAP_TEST.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("monkeys.csl", &OnLoadObject, nullptr);
    //OE_LoadWorld("csl_very_large_object_test.csl", &OnLoadObject, nullptr);
    //taskMgr.Start();
    OE_Start();
    
    return 0;
}
