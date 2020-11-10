#include <OE_API.h>

using namespace OE;
using namespace std;

OE_TaskManager* OE::OE_Main = nullptr;

size_t OE::OE_Init(){    
    OE_Main = new OE_TaskManager();
    return OE_Main->Init("Oxygen Engine Test", 800, 600, false);
}
size_t OE::OE_Init(std::string title, bool fullscreen){
    OE_Main = new OE_TaskManager();
    return OE_Main->Init(title, 800, 600, fullscreen);
}
size_t OE::OE_Init(std::string title, int x, int y, bool fullscreen){
    OE_Main = new OE_TaskManager();
    return OE_Main->Init(title, x, y, fullscreen);
}
void OE::OE_Step(){
    assert (OE_Main != nullptr);
    OE_Main->Step();
}
void OE::OE_Start(){
    assert (OE_Main != nullptr);
    OE_Main->Start();
}
void OE::OE_Destroy(){
    assert (OE_Main != nullptr);
    OE_Main->Destroy();
    delete OE_Main;
    OE_Main = nullptr;
}
bool OE::OE_IsDone(){
    assert (OE_Main != nullptr);
    return OE_Main->done;
}
// ?? Where do i even need this ??? UPDATE: Now I remember
void OE::OE_Finish(){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.done = true;
} 

//size_t OE_InitFromFile(std::string); //TODO
    
//------------------------BLOCK-------------------------//
// The API functions inside this block are the only ones that are 
// safe to be called on an unsynchronized thread
    
void OE::OE_AddTask(std::string name, const OE_METHOD func, void* data){
    assert (OE_Main != nullptr);
    OE_Main->AddTask(name, func, data);
}

void OE::OE_AddTask(std::string name, const OE_METHOD func, int priority, void* data){
    assert (OE_Main != nullptr);
    OE_Main->AddTask(name, func, data);
}

void OE::OE_AddTask(std::string name, const OE_METHOD func, int priority, std::string threadname, void* data){
    assert (OE_Main != nullptr);
    OE_Main->AddTask(name, func, priority, threadname, data);
}

void OE::OE_AddTask(std::string name, const OE_METHOD func, std::string threadname, void* data){
    assert (OE_Main != nullptr);
    OE_Main->AddTask(name, func, threadname, data);
}
    
void OE::OE_RemoveTask(std::string task){
    assert (OE_Main != nullptr);
    OE_Main->RemoveTask(task);
}

void OE::OE_RemoveTask(std::string task, std::string thread){
    assert (OE_Main != nullptr);
    OE_Main->RemoveTask(task, thread);
}
    
void OE::OE_BroadcastEvent(std::string name, void* data){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.broadcastIEvent(name, data);
}
void OE::OE_CreateEvent(std::string name){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.createUserEvent(name);
}
void OE::OE_SetEventFunc(std::string name, const OE_EVENTFUNC func, void* data){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.setIEventFunc(name, func, data);
}

size_t OE::OE_GetEventActivations(std::string name){
    assert (OE_Main != nullptr);
    return OE_Main->event_handler.getEventActivations(name);
}

size_t OE::OE_GetEventCounter(std::string name){
    assert (OE_Main != nullptr);
    return OE_Main->event_handler.getEventCounter(name);
}

bool OE::OE_IsKeyJustPressed(std::string key){
    assert (OE_Main != nullptr);
    return OE_GetEventActivations(key+"+") > 0;
}

bool OE::OE_IsKeyJustReleased(std::string key){
    assert (OE_Main != nullptr);
    return OE_GetEventActivations(key+"-") > 0;
}
    
bool OE::OE_IsKeyHeld(std::string key){
    assert (OE_Main != nullptr);
    return OE_GetEventActivations(key) > 0;
}

bool OE::OE_IsKeyPressed(std::string key){
    assert (OE_Main != nullptr);
    return (OE_GetEventActivations(key) > 0) || (OE_GetEventActivations(key+"+") > 0);
}

bool OE::OE_IsMouseMoved(){
    return OE_GetEventActivations("mouse-motion") > 0;
}

int OE::OE_GetDeltaMouseX(){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.lockMutex();
    int output = OE_MouseEvent::delta_x;
    OE_Main->event_handler.unlockMutex();
    return output;
}

int OE::OE_GetDeltaMouseY(){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.lockMutex();
    int output = OE_MouseEvent::delta_y;
    OE_Main->event_handler.unlockMutex();
    return output;
}
    
int OE::OE_GetMouseX(){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.lockMutex();
    int output = OE_MouseEvent::x;
    OE_Main->event_handler.unlockMutex();
    return output;
}

int OE::OE_GetMouseY(){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.lockMutex();
    int output = OE_MouseEvent::y;
    OE_Main->event_handler.unlockMutex();
    return output;
}

void OE::OE_DestroyEvent(std::string name){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.destroyIEvent(name);
}

void OE::OE_Pause(int x){
    assert (OE_Main != nullptr);
    OE_Main->pause(x);
}

bool OE::OE_IsMouseLocked(){
    return OE_Main->window->getMouseLockedState();
}

void OE::OE_MouseLock(){
    
    OE_CreateEvent("mouse-lock");
    OE_SetEventFunc("mouse-lock", &OE_API_Helpers::manage_mouse, nullptr);
    OE_BroadcastEvent("mouse-lock", nullptr);
    
}
void OE::OE_MouseUnlock(){
    OE_CreateEvent("mouse-unlock");
    OE_SetEventFunc("mouse-unlock", &OE_API_Helpers::manage_mouse, nullptr);
    OE_BroadcastEvent("mouse-unlock", nullptr);
}

//------------------------BLOCK-------------------------//

OE_Task OE::OE_GetTaskInfo(std::string thread, std::string task){
    assert (OE_Main != nullptr);
    return OE_Main->GetTaskInfo(thread, task);
}
    
void OE::OE_CreateNewThread(std::string name){
    assert (OE_Main != nullptr);
    OE_Main->CreateNewThread(name);
}

void OE::OE_CreateUnsyncThread(std::string name, const OE_METHOD func, void* data){
    assert (OE_Main != nullptr);
    OE_Main->CreateUnsyncThread(name, func, data);
}
    
/** API functions for loading worlds/scenes/objects/etc.
 */
    
void OE::OE_LoadWorld(std::string filename, const OE_EVENTFUNC func, void* data){
    
    OE_CreateEvent("loaded-" + filename);
    OE_SetEventFunc("loaded-" + filename, func, data);
    string* argument = new string();
    *argument = filename;
    OE_CreateUnsyncThread("loading-" + filename, &OE_API_Helpers::load_world, (void*)argument);
}

/** API functions for manipulating objects and basic types
     * to be vastly extended when the physics engine comes
     */
    
std::size_t OE::OE_GetObjectID(std::string name){
    OE_World::objectsList.lockMutex();
    std::size_t output = OE_World::objectsList.name2id[name];
    OE_World::objectsList.unlockMutex();
    return output;
}

std::string OE::OE_GetObjectName(std::size_t id){
    OE_World::objectsList.lockMutex();
    std::string output;
    if (OE_World::objectsList.id2name.count(id)){
        output = OE_World::objectsList.id2name[id];
    }
    OE_World::objectsList.unlockMutex();
    return output;
}
    
std::set<std::size_t>   OE::OE_GetSceneObjects(std::size_t id){
    
    std::set<std::size_t> output;
    
    auto scene = OE_World::scenesList(id);
    if (scene != nullptr){
        scene->lockMutex();
        output = scene->objects;
        scene->unlockMutex();
    }
    else {
        //TODO output a warning
    }
    return output;
}

OE_Vec3 OE::OE_GetObjectPos(std::size_t id){
    
    OE_Vec3 output;
    
    auto object = OE_World::objectsList(id);
    if (object != nullptr){
        object->lockMutex();
        output[0] = object->current_state.pos_x;
        output[1] = object->current_state.pos_y;
        output[2] = object->current_state.pos_z;
        object->unlockMutex();
    }
    return output;
}

OE_Quat OE::OE_GetObjectRot(std::size_t id){
    
    OE_Quat output = OE_Quat();
    
    auto object = OE_World::objectsList(id);
    if (object != nullptr){
        object->lockMutex();
        output = object->GetRot();
        object->unlockMutex();
    }
    return output;
}

std::set<std::size_t>   OE::OE_GetSceneObjects(std::string name){
    
    std::set<std::size_t> output;
    
    auto scene = OE_World::scenesList(name);
    if (scene != nullptr){
        scene->lockMutex();
        output = scene->objects;
        scene->unlockMutex();
    }
    else {
        //TODO output a warning
    }
    return output;
}

OE_Vec3 OE::OE_GetObjectPos(std::string name){
    
    OE_Vec3 output;
    
    auto object = OE_World::objectsList(name);
    if (object != nullptr){
        object->lockMutex();
        output[0] = object->current_state.pos_x;
        output[1] = object->current_state.pos_y;
        output[2] = object->current_state.pos_z;
        object->unlockMutex();
    }
    return output;
}

OE_Quat OE::OE_GetObjectRot(std::string name){
    
    OE_Quat output;
    
    auto object = OE_World::objectsList(name);
    if (object != nullptr){
        object->lockMutex();
        output = object->GetRot();
        object->unlockMutex();
    }
    return output;
}
    
void OE::OE_SetObjectPos(std::size_t id, OE_Vec3 pos){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        object->current_state.pos_x = pos.x;
        object->current_state.pos_y = pos.y;
        object->current_state.pos_z = pos.z;
        object->unlockMutex();
    }
}

void OE::OE_SetObjectRot(std::size_t id, OE_Quat rot){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(rot);
        object->unlockMutex();
    }
}

void OE::OE_SetObjectRot(std::size_t id, OE_Vec4 rot){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object->unlockMutex();
    }
}
    
void OE::OE_ChangeObjectPos(std::size_t id, OE_Vec3 pos){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        object->current_state.pos_x = object->current_state.pos_x + pos.x;
        object->current_state.pos_y = object->current_state.pos_y + pos.y;
        object->current_state.pos_z = object->current_state.pos_z + pos.z;
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectRot(std::size_t id, OE_Quat rot){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(object->GetRot()*rot);
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectRot(std::size_t id, OE_Vec4 rot){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(object->GetRot()*OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object->unlockMutex();
    }
}
    
void OE::OE_SetObjectPos(std::string name, OE_Vec3 pos){
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        object->current_state.pos_x = pos.x;
        object->current_state.pos_y = pos.y;
        object->current_state.pos_z = pos.z;
        object->unlockMutex();
    }
}

void OE::OE_SetObjectRot(std::string name, OE_Quat rot){    
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(rot);
        object->unlockMutex();
    }
}

void OE::OE_SetObjectRot(std::string name, OE_Vec4 rot){
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object->unlockMutex();
    }
}
    
void OE::OE_ChangeObjectPos(std::string name, OE_Vec3 pos){
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        object->current_state.pos_x = object->current_state.pos_x + pos.x;
        object->current_state.pos_y = object->current_state.pos_y + pos.y;
        object->current_state.pos_z = object->current_state.pos_z + pos.z;
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectRot(std::string name, OE_Quat rot){
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(object->GetRot()*rot);
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectRot(std::string name, OE_Vec4 rot){
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(object->GetRot()*OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3]));
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectGlobalRot(std::size_t id, OE_Quat rot){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(rot*object->GetRot());
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectGlobalRot(std::size_t id, OE_Vec4 rot){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3])*object->GetRot());
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectGlobalRot(std::string name, OE_Quat rot){
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(rot*object->GetRot());
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectGlobalRot(std::string name, OE_Vec4 rot){
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        object->SetRot(OE_QuatFromAxisAngle(rot[0], rot[1], rot[2], rot[3])*object->GetRot());
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectLocalPos(std::size_t id, OE_Vec3 pos){
    auto object = OE_World::objectsList[id];
    if (object != nullptr){
        object->lockMutex();
        OE_Mat3x3 rot_matrix = object->GetModelMatrix();
        auto local_pos = rot_matrix * pos;
        object->current_state.pos_x = object->current_state.pos_x + local_pos.x;
        object->current_state.pos_y = object->current_state.pos_y + local_pos.y;
        object->current_state.pos_z = object->current_state.pos_z + local_pos.z;
        object->unlockMutex();
    }
}

void OE::OE_ChangeObjectLocalPos(std::string name, OE_Vec3 pos){
    auto object = OE_World::objectsList[name];
    if (object != nullptr){
        object->lockMutex();
        OE_Mat3x3 rot_matrix = object->GetModelMatrix();
        auto local_pos = rot_matrix * pos;
        object->current_state.pos_x = object->current_state.pos_x + local_pos.x;
        object->current_state.pos_y = object->current_state.pos_y + local_pos.y;
        object->current_state.pos_z = object->current_state.pos_z + local_pos.z;
        object->unlockMutex();
    }
}

