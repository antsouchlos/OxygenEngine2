#ifndef OE_API_H_INCLUDED
#define OE_API_H_INCLUDED

#include <OE_TaskManager.h>
#include <OE_API_Helpers.h>

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
    void OE_Finish();
    //size_t OE_InitFromFile(std::string); //TODO
    
    //------------------------BLOCK-------------------------//
    // The API functions inside this block are the only ones that are 
    // safe to be called on an unsynchronized thread together with all the OE_Get* functions
    
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
    
    /** API functions for manipulating objects and basic types
     * to be vastly extended when the physics engine comes
     */
    
    std::size_t             OE_GetObjectID(std::string);
    std::string             OE_GetObjectName(std::size_t);
    
    std::set<std::size_t>   OE_GetSceneObjects(std::size_t);
    OE_Vec3                 OE_GetObjectPos(std::size_t);
    OE_Quat                 OE_GetObjectRot(std::size_t);
    std::set<std::size_t>   OE_GetSceneObjects(std::string);
    OE_Vec3                 OE_GetObjectPos(std::string);
    OE_Quat                 OE_GetObjectRot(std::string);
    
    void OE_SetObjectPos(std::size_t, OE_Vec3);
    void OE_SetObjectRot(std::size_t, OE_Quat);
    void OE_SetObjectRot(std::size_t, OE_Vec4);
    
    void OE_ChangeObjectPos(std::size_t, OE_Vec3);
    void OE_ChangeObjectRot(std::size_t, OE_Quat);
    void OE_ChangeObjectRot(std::size_t, OE_Vec4);
    
    void OE_SetObjectPos(std::string, OE_Vec3);
    void OE_SetObjectRot(std::string, OE_Quat);
    void OE_SetObjectRot(std::string, OE_Vec4);
    
    void OE_ChangeObjectPos(std::string, OE_Vec3);
    void OE_ChangeObjectRot(std::string, OE_Quat);
    void OE_ChangeObjectRot(std::string, OE_Vec4);
    
    void OE_ChangeObjectGlobalRot(std::size_t, OE_Quat);
    void OE_ChangeObjectGlobalRot(std::size_t, OE_Vec4);
    void OE_ChangeObjectGlobalRot(std::string, OE_Quat);
    void OE_ChangeObjectGlobalRot(std::string, OE_Vec4);
    
    void OE_ChangeObjectLocalPos(std::size_t, OE_Vec3);
    void OE_ChangeObjectLocalPos(std::string, OE_Vec3);
};















#endif
