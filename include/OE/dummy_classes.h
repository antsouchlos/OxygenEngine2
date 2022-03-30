#ifndef OE_DUMMYCLASSES_H
#define OE_DUMMYCLASSES_H

#include <Events/event_handler.h>
#include <task.h>
#include <types/base_types.h>
#include <types/world.h>



enum OE_OS : int { OE_UNDEFINED = 0, OE_LINUX = 1, OE_WINDOWS = 2, OE_ANDROID = 2 };

enum OE_WINSYS : int { OE_NONE = 0, OE_SDL = 1, OE_SOMETHING_ELSE = 2 };

enum OE_RENDERER_SHADING_MODE : int {
    OE_RENDERER_NORMALS_SHADING        = 0,
    OE_RENDERER_NO_LIGHTS_SHADING      = 1,
    OE_RENDERER_DIR_LIGHTS_SHADING     = 2,
    OE_RENDERER_INDEXED_LIGHTS_SHADING = 3,
    OE_RENDERER_REGULAR_SHADING        = 4
};

/** This a dummy class aimed to be used as a base class for windowing systems
 * It
 */
class OE_WindowSystemBase : public OE_THREAD_SAFETY_OBJECT {
public:
    OE_WindowSystemBase();
    virtual ~OE_WindowSystemBase();


    virtual bool init(int, int, std::string, bool, bool, void*);
    virtual bool update();

    virtual bool getMouseLockedState();
    virtual void lockMouse();
    virtual void unlockMouse();

    virtual bool updateEvents();
    virtual void destroy();

    int  resolution_x{0};
    int  resolution_y{0};
    int  dpi{96};
    bool vsync{true};

    OE_OS     os{OE_UNDEFINED};
    OE_WINSYS winsys{OE_NONE};

    // For different OpenGL versions
    std::string title;
    bool        fullscreen{false};
    int         major{0};
    int         minor{0};
    bool        isES{false};

    bool mouse_locked{false};

    std::atomic<bool> reset_renderer{false};
    std::atomic<bool> restart_renderer{false};

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

    bool                      isMultiThreaded{false};
    std::shared_ptr<OE_World> world{nullptr};
    OE_WindowSystemBase*      screen{nullptr};
    std::string               name{"default"};

    OE_RENDERER_SHADING_MODE shading_mode{OE_RENDERER_REGULAR_SHADING};
    std::atomic<bool>        use_wireframe{false};
    std::atomic<bool>        render_bounding_boxes{false};
    std::atomic<bool>        render_bounding_spheres{false};
    std::atomic<bool>        use_HDR{false};
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

    bool                      isMultiThreaded{false};
    std::shared_ptr<OE_World> world{nullptr};
    // You do not actually need this since you can use API functions directly in your
    // Physics engine .cpp :))
    // OE_EventHandler*    handler{nullptr};
    std::string name{"default"};
};

/** This is a dummy class aimed to be a base class for
 *  a netwotking manager
 */

class OE_NetworkingBase : public OE_THREAD_SAFETY_OBJECT {
public:
    OE_NetworkingBase();
    virtual ~OE_NetworkingBase();

    virtual void init();
    virtual int  execute(OE_Task);
    virtual void destroy();

    std::atomic<bool> done{false};
    std::string       name{"default"};
};

#endif
