#ifndef OE_API_H_INCLUDED
#define OE_API_H_INCLUDED

#include <OE_TaskManager.h>


// This is going to be a very long header with ALL avaiable API functions declared
// TODO: Make this extern "C" so that we have a C compatible API (Long-term goal)

namespace OE{
    
    // GLOBAL VARIABLE
    extern OE_TaskManager* OE_Main;
    
    /** Basic API functions for starting the Oxygen Engine
     *  and assigning tasks
     */
    size_t OE_Init();
    size_t OE_Init(std::string, bool);
    size_t OE_Init(std::string, int, int, bool);
    void OE_Step();
    void OE_Start();
    void OE_Destroy();
    bool OE_IsDone();
    void OE_Finish(); // ?? Where do i even need this ???
    //size_t OE_InitFromFile(std::string); //TODO
    
    //------------------------BLOCK-------------------------//
    // The API functions inside this block are the only ones that are 
    // safe to be called on an unsynchronized thread
    
    void OE_AddTask(std::string, const OE_METHOD, void*);
    void OE_AddTask(std::string, const OE_METHOD, int, void*);
    void OE_AddTask(std::string, const OE_METHOD, int, std::string, void*);
    void OE_AddTask(std::string, const OE_METHOD, std::string, void*);
    
    void OE_RemoveTask(std::string);
    void OE_RemoveTask(std::string, std::string);
    
    void OE_BroadcastEvent(std::string, void*);
    void OE_CreateEvent(std::string);
    void OE_SetEventFunc(std::string, const OE_EVENTFUNC, void*);
    
    void OE_DestroyEvent(std::string);
    
    void OE_Pause(int);
    
    //------------------------BLOCK-------------------------//
    
    OE_Task OE_GetTaskInfo(std::string, std::string);
    
    void OE_CreateNewThread(std::string);
    void OE_CreateUnsyncThread(std::string, const OE_METHOD, void*);
    
    /** API functions for loading worlds/scenes/objects/etc.
     */
    
    void OE_LoadWorld(std::string, const OE_EVENTFUNC, void*);
    
    
};















#endif
