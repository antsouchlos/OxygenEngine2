#include <OE_API.h>

using namespace OE;

size_t OE_Init(){ 
    return 0;
}
size_t OE_Init(std::string, bool){return 0;}
size_t OE_Init(std::string, int, int, bool){return 0;}
void OE_Step(){}
void OE_Start(){}
void OE_Destroy(){}
bool OE_IsDone(){return 0;}
void OE_Finish(){}
//size_t OE_InitFromFile(std::string); //TODO
    
//------------------------BLOCK-------------------------//
// The API functions inside this block are the only ones that are 
// safe to be called on an unsynchronized thread
    
void OE_AddTask(std::string, const OE_METHOD, void*){}
void OE_AddTask(std::string, const OE_METHOD, int, void*){}
void OE_AddTask(std::string, const OE_METHOD, int, std::string, void*){}
void OE_AddTask(std::string, const OE_METHOD, std::string, void*){}
    
void OE_RemoveTask(std::string){}
void OE_RemoveTask(std::string, std::string){}
    
void OE_BroadcastEvent(std::string, void*){}
void OE_CreateEvent(std::string){}
void OE_SetEventFunc(std::string, const OE_EVENTFUNC, void*){}
    
void OE_DestroyEvent(std::string){}
    
//------------------------BLOCK-------------------------//

OE_Task OE_GetTaskInfo(std::string thread, std::string task){
    assert (OE_Main != nullptr);
    return OE_Main->GetTaskInfo(thread, task);
}
    
void OE_CreateNewThread(std::string){}
void OE_CreateUnsyncThread(std::string, const OE_METHOD, void*){}
    
/** API functions for loading worlds/scenes/objects/etc.
 */
    
void OE_LoadWorld(std::string, const OE_EVENTFUNC, void*){}
