#ifndef OE_DUMMYCLASSES_H
#define OE_DUMMYCLASSES_H

#include <OE_Task.h>
#include <Events/OE_EventHandler.h>
#include <types/OE_TypesBase.h>
#include <types/OE_World.h>

enum OE_OS : int{
    OE_UNDEFINED=0,
    OE_LINUX=1,
    OE_WINDOWS=2
};

enum OE_WINSYS : int{
    OE_NONE=0,
    OE_SDL=1,
    OE_SOMETHING_ELSE=2
};

/** This a dummy class aimed to be used as a base class for windowing systems
  * It 
  */
class OE_WindowSystemBase: public OE_THREAD_SAFETY_OBJECT {
public:
    OE_WindowSystemBase();
    virtual ~OE_WindowSystemBase();
    
    
    virtual bool init(int, int, std::string, bool, void*);
    virtual bool update();
    
    virtual bool getMouseLockedState();
    virtual void lockMouse();
    virtual void unlockMouse();
    
    virtual bool updateEvents();
    virtual void destroy();
    
    int resolution_x{0}; int resolution_y{0};
    int dpi{96};
    bool vsync{true};
    
    OE_OS os{OE_UNDEFINED};
    OE_WINSYS winsys{OE_NONE};
    
    // For different OpenGL versions
    std::string title;
    bool fullscreen{false};
    int major{0}; int minor{0}; bool isES{false};
    
    bool mouse_locked{false};
    
    bool reset_renderer{false};
    bool restart_renderer{false};
    
    // The global event handler is here and must be initialized in all sub classes
    OE_EventHandler event_handler;
};

/** This is a dummy class aimed to be a base class for
 *  a renderer
 */
class OE_RendererBase : public OE_THREAD_SAFETY_OBJECT {
    public:
    OE_RendererBase();
    virtual ~OE_RendererBase();
    
    virtual bool init();
    virtual bool updateSingleThread();
    virtual bool updateData();
    
    virtual bool updateMultiThread(OE_Task*, int);
    virtual void destroy();
    
    bool                    isMultiThreaded{false};
    std::shared_ptr<OE_World>   world{nullptr};
    OE_WindowSystemBase*    screen{nullptr};
    std::string             name{"default"};
};

/** This is a dummy class aimed to be a base class for
 *  a physics engine
 */
class OE_PhysicsEngineBase : public OE_THREAD_SAFETY_OBJECT {
    public:
    OE_PhysicsEngineBase();
    virtual ~OE_PhysicsEngineBase();
    
    virtual bool init();
    
    virtual bool updateMultiThread(OE_Task*, int);
    virtual void destroy();
    
    bool                isMultiThreaded{false};
    std::shared_ptr<OE_World>   world{nullptr};
    // You do not actually need this since you can use API functions directly in your 
    // Physics engine .cpp :))
    //OE_EventHandler*    handler{nullptr};
    std::string         name{"default"};
};

#endif
