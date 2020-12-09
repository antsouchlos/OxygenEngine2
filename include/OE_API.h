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
    
    // for events in general
    size_t OE_GetEventActivations(std::string);
    size_t OE_GetEventCounter(std::string);
    
    // for keyboard and mouse events
    bool OE_IsKeyJustPressed(std::string);
    bool OE_IsKeyJustReleased(std::string);
    
    bool OE_IsKeyHeld(std::string);
    bool OE_IsKeyPressed(std::string);
    
    bool OE_IsMouseMoved();

    int OE_GetDeltaMouseX();
    int OE_GetDeltaMouseY();
    
    int OE_GetMouseX();
    int OE_GetMouseY();
    
    bool OE_IsMouseLocked();
    void OE_MouseLock();
    void OE_MouseUnlock();
    
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
    OE_Vec3                 OE_GetObjectModelMatrix(std::size_t);
    OE_Quat                 OE_GetObjectRot(std::size_t);
    
    std::set<std::size_t>   OE_GetSceneObjects(std::string);
    OE_Vec3                 OE_GetObjectPos(std::string);
    OE_Vec3                 OE_GetObjectModelMatrix(std::string);
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
    
    void OE_SetObjectScale(std::size_t, OE_Vec3);
    void OE_SetObjectScale(std::string, OE_Vec3);
    
    void OE_ChangeObjectScale(std::size_t, OE_Vec3);
    void OE_ChangeObjectScale(std::string, OE_Vec3);
    
    /** API functions to control the renderer
     *  These should work for ANY renderer
     * All those parameters require a call to OE_RestartRenderer to take effect
     */
    
    void OE_RestartRenderer();
    void OE_SetShadingMode(OE_RENDERER_SHADING_MODE);
    OE_RENDERER_SHADING_MODE OE_GetShadingMode();
    
    void OE_RenderWireframe(bool);
    void OE_ToggleWireframeRendering();
    
    void OE_RenderBoundingBoxes(bool);
    void OE_ToggleBoundingBoxesRendering();
};















#endif
