#ifndef FE_GPU_API_H_INCLUDED
#define FE_GPU_API_H_INCLUDED
#include "FE_GPU_Thread.h"


/** platform-specific namespaces
  * -OpenGL ES 3.0/ OpenGL 3.2
  * -OpenGL 4.1
  * -OpenGL 4.4 + GL_ARB_bindless_texture
  * -Direct3D 9 (maybe, 5% chance to happen)
  */
#ifdef FE_WINDOWS
void DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam);
#endif
//MALAKIA
#ifdef FE_LINUX
void DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, void* userParam);
#endif

class FE_GPU_API;
//class FE_GPU_Thread;

#ifdef FE_ATTRIB_OPENGL
namespace FE_GL3_API{
    void init(FE_GPU_API*, string, int, int, bool);
    void update(FE_GPU_API*);
    void destroy(FE_GPU_API*);
};
#endif // FE_ATTRIB_OPENGL


/** FE_GPU_API provides a platform-independent
  * interface for accessing the GPU on differing
  * systems for the renderer and conserves precious
  * development time.
  */

class FE_GPU_API : public FE_THREAD_SAFETY_OBJECT{
public:
    FE_GPU_API();
    ~FE_GPU_API();
    void init(FE_RENDERTYPE, FE_GPU_Info*, string, int, int, bool);

    void update();
    FE_GPU_Thread* createThread(string);
    void removeThread(string);
    void destroy();



    vector<FE_GPU_Thread*> threads;
    bool made_context;
    FE_RENDERTYPE rend_type;
    //#if defined(FE_PC) || defined(FE_MOBILE)

    SDL_Window* window;
    SDL_GLContext context;

    //#endif
    int width;
    int height;
    string title;
    bool fullscreen;
};

#endif // FE_GPU_API_H_INCLUDED
