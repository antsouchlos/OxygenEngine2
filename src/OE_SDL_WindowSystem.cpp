#include <OE_SDL_WindowSystem.h>

using namespace std;

OE_SDL_WindowSystem::OE_SDL_WindowSystem(){
    this->winsys = OE_SDL;
    this->os = OE_LINUX;
}

OE_SDL_WindowSystem::~OE_SDL_WindowSystem(){

}

bool OE_SDL_WindowSystem::init(int x, int y, string titlea, bool isFullscreen, void* data){
    
    this->title = titlea;
    this->fullscreen = isFullscreen;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        cout << "OE ERROR: Could not initialize SDL2, " << SDL_GetError() << endl;
        exit(-1);
    }
    
    SDL_GL_LoadLibrary(NULL);
    
    // Request an OpenGL 3.2 core context
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    this->major = 3; this->minor = 2; this->isES = false;
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    if (!this->fullscreen)
        this->window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
    else
        this->window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL );
    
    this->context = SDL_GL_CreateContext(this->window);
    if (context == NULL){
         cout << "OE WARNING: Could not initialize OpenGL 3.2 Core Context, " << SDL_GetError() << endl;
    }
    else {
        this->finishInit();
        return true;
    }
    
    // Request an OpenGL 3.1 context
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    
     this->major = 3; this->minor = 1; this->isES = false;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);
    
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    this->context = SDL_GL_CreateContext(this->window);
    if (context == NULL){
         cout << "OE WARNING: Could not initialize OpenGL 3.1 Core Context, " << SDL_GetError() << endl;
    }
    else {
        this->finishInit();
        return true;
    }
    
    // Request an OpenGL ES 3.0 context
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
     this->major = 3; this->minor = 0; this->isES = true;
     
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    this->context = SDL_GL_CreateContext(this->window);
    if (context == NULL){
         cout << "OE WARNING: Could not initialize OpenGL ES 3.0 Context, " << SDL_GetError() << endl;
    }
    else {
        this->finishInit();
        return true;
    }
    
    return false;
}

void OE_SDL_WindowSystem::finishInit(){
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));
    
    SDL_GL_SetSwapInterval(1);
    
    glEnable( GL_BLEND );
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS);

    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK); /// cull back face
    glFrontFace (GL_CCW);
    
    SDL_GetWindowSize(window, &this->resolution_x, &this->resolution_y);
    glViewport(0, 0, this->resolution_x, this->resolution_y);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    SDL_GL_SwapWindow(this->window);
    
}

bool OE_SDL_WindowSystem::getMouseLockedState(){
    lockMutex();
    bool output = mouse_locked;
    unlockMutex();
    return output;
}

void OE_SDL_WindowSystem::lockMouse(){
    SDL_SetRelativeMouseMode(SDL_TRUE);
    lockMutex();
    this->mouse_locked = true;
    unlockMutex();
}

void OE_SDL_WindowSystem::unlockMouse(){
    SDL_SetRelativeMouseMode(SDL_FALSE);
    lockMutex();
    this->mouse_locked = false;
    unlockMutex();
}

bool OE_SDL_WindowSystem::update(){
    
    this->counter++;
    this->counter = this->counter % 100;
    
    SDL_GL_SwapWindow(this->window);
    // Change viewport resolution if desired
    int x; int y;
    SDL_GetWindowSize(window, &x, &y);
    if (x != this->resolution_x || y != this->resolution_y){
        glViewport(0, 0, this->resolution_x, this->resolution_y);
        this->resolution_x = x;
        this->resolution_y = y;
    }
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    return true;
}

bool OE_SDL_WindowSystem::updateEvents(){
    return true;
}

void OE_SDL_WindowSystem::destroy(){
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
}
