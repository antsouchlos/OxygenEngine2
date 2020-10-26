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
    
void OE::OE_DestroyEvent(std::string name){
    assert (OE_Main != nullptr);
    OE_Main->event_handler.destroyIEvent(name);
}

void OE::OE_Pause(int x){
    assert (OE_Main != nullptr);
    OE_Main->pause(x);
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
