#ifndef OE_SDL_WINDOWSYSTEM_H
#define OE_SDL_WINDOWSYSTEM_H

#include <OE_DummyClasses.h>
#include <Renderer/glad.h>

class OE_SDL_WindowSystem: public OE_WindowSystemBase {
public:
    OE_SDL_WindowSystem();
    ~OE_SDL_WindowSystem();
    
    bool init(int, int, std::string, bool, void*);
    bool update();
    
    void finishInit();
    
    bool getMouseLockedState();
    void lockMouse();
    void unlockMouse();
    
    bool updateEvents();
    void destroy();
    
    SDL_Window* window;
    SDL_GLContext context;
    
    SDL_Event event;
    
    int counter{0};
    
    bool mouse_moved{false};
    
protected:
    
    void updateWindowEvents();
};


#endif
