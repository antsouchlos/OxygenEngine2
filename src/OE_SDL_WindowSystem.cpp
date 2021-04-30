#include <OE_SDL_WindowSystem.h>
#include <types/OE_Libs.h>
#include <iostream>

using namespace std;

OE_SDL_WindowSystem::OE_SDL_WindowSystem(){
    this->winsys = OE_SDL;
#ifdef OE_PLATFORM_LINUX
    this->os = OE_LINUX;
#endif
#ifdef OE_PLATFORM_WINDOWS
    this->os = OE_WINDOWS;
#endif
}

OE_SDL_WindowSystem::~OE_SDL_WindowSystem(){
    
    SDL_Quit();
    
}

bool OE_SDL_WindowSystem::init(int x, int y, string titlea, bool isFullscreen, void* data){
    
    this->title = titlea;
    this->fullscreen = isFullscreen;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        cout << "OE ERROR: Could not initialize SDL2, " << SDL_GetError() << endl;
        exit(-1);
    }
    
    SDL_GL_LoadLibrary(NULL);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    
    // Request an OpenGL 3.2 core context
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    this->major = 3; this->minor = 2; this->isES = false;
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 0);

    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    if (!this->fullscreen)
        this->window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
    else
        this->window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
    
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
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    this->context = SDL_GL_CreateContext(this->window);
    if (context == NULL){
         cout << "OE WARNING: Could not initialize OpenGL 3.1 Context, " << SDL_GetError() << endl;
    }
    else {
        this->finishInit();
        return true;
    }//*/
    
    // Request an OpenGL ES 3.0 context
    
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
     this->major = 3; this->minor = 0; this->isES = true;
     
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 0);
    
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
    
    if (!this->isES)
        gladLoadGLLoader(SDL_GL_GetProcAddress);
    else
        gladLoadGLES2Loader(SDL_GL_GetProcAddress);
    
    printf("Vendor:   '%s'\n", glGetString(GL_VENDOR));
    printf("Renderer: '%s'\n", glGetString(GL_RENDERER));
    printf("Version:  '%s'\n", glGetString(GL_VERSION));
    
    OE_WriteToLog(string("Vendor:   '") + string((const char*)glGetString(GL_VENDOR)) + "'\n");
    OE_WriteToLog(string("Renderer: '") + string((const char*)glGetString(GL_RENDERER)) + "'\n");
    OE_WriteToLog(string("Version:  '") + string((const char*)glGetString(GL_VERSION)) + "'\n");
    SDL_GL_SetSwapInterval(1);
    
    SDL_GetWindowSize(window, &this->resolution_x, &this->resolution_y);
    glViewport(0, 0, this->resolution_x, this->resolution_y);
    
    //SDL_GL_SwapWindow(this->window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    SDL_GL_SwapWindow(this->window);
    
    this->event_handler.init();
    
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
    
    lockMutex();
    this->resolution_x = x;
    this->resolution_y = y;
    unlockMutex();
    
    this->event_handler.updateInput();
    this->mouse_moved = false;
    
    while(SDL_PollEvent(&this->event)){
        
        // exit before handling SDL events
        if(event.type == SDL_QUIT){ 
            this->event_handler.done = true;
            return this->event_handler.done;
        }
        this->updateEvents();
        this->updateWindowEvents();
    }
    
    if(this->mouse_moved){
        //fetch mouse position, since this IS needed
        this->event_handler.lockMutex();
        SDL_GetMouseState(&OE_MouseEvent::x, &OE_MouseEvent::y);
        SDL_GetRelativeMouseState(&OE_MouseEvent::delta_x, &OE_MouseEvent::delta_y);
        this->event_handler.unlockMutex();
        this->event_handler.broadcastIEvent("mouse-motion");
    }

    // This is needed to support things like OE_Finish()
    return this->event_handler.done;
    
}

bool OE_SDL_WindowSystem::updateEvents(){
    
    switch(this->event.type){
        //check for key presses
        case SDL_KEYDOWN:
            for(auto key: this->event_handler.input_handler.keyList){
                if (this->event.key.keysym.sym == key.first){
                    this->event_handler.internalBroadcastKeyDownEvent("keyboard-" + key.second);
                }
            }
            break;

        // check for releases
        case SDL_KEYUP:
			for(auto key: this->event_handler.input_handler.keyList){
				if (this->event.key.keysym.sym == key.first){
                    this->event_handler.internalBroadcastKeyUpEvent("keyboard-" + key.second);
				}			
			}
			break;

		// update mouse position
        case SDL_MOUSEMOTION:
        	//cout << "MOUSE_MOTION EVENt" << endl;
        	this->mouse_moved = true;			
			break;

		// update mouse down events
        case SDL_MOUSEBUTTONDOWN:
        	for(auto key: this->event_handler.input_handler.mouseList){
        		if (this->event.button.button == key.first){
        			//fetch mouse position, since this may be needed
                    this->event_handler.lockMutex();
                    SDL_GetMouseState(&OE_MouseEvent::x, &OE_MouseEvent::y);
                    SDL_GetRelativeMouseState(&OE_MouseEvent::delta_x, &OE_MouseEvent::delta_y);
                    this->event_handler.unlockMutex();
                    
                    this->event_handler.internalBroadcastKeyDownEvent("mouse-" + key.second);
				}
        	}
        	break;
            
        // update mouse up events
        case SDL_MOUSEBUTTONUP:
            for(auto key: this->event_handler.input_handler.mouseList){
                if (this->event.button.button == key.first){
                    //fetch mouse position, since this may be needed
                    this->event_handler.lockMutex();
                    SDL_GetMouseState(&OE_MouseEvent::x, &OE_MouseEvent::y);
                    SDL_GetRelativeMouseState(&OE_MouseEvent::delta_x, &OE_MouseEvent::delta_y);
                    this->event_handler.unlockMutex();
                    
                    this->event_handler.internalBroadcastKeyUpEvent("mouse-" + key.second);
                }
            }
            break;
        
        // update mouse wheel events
        case SDL_MOUSEWHEEL:
            
            this->event_handler.lockMutex();
            OE_MouseEvent::mouse_wheel = event.wheel.y;
            //fetch mouse position, since this may be needed
            SDL_GetMouseState(&OE_MouseEvent::x, &OE_MouseEvent::y);
            SDL_GetRelativeMouseState(&OE_MouseEvent::delta_x, &OE_MouseEvent::delta_y);
            this->event_handler.unlockMutex();
            
            this->event_handler.broadcastIEvent("mouse-wheel");
            break;
	}
	
    return true;
}

void OE_SDL_WindowSystem::updateWindowEvents(){

    if (this->event.type == SDL_WINDOWEVENT) {
        switch (this->event.window.event) {
            case SDL_WINDOWEVENT_SHOWN:
                
                printf("Window %d shown", this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                printf("Window %d hidden", this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                printf("Window %d exposed", this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_MOVED:
                printf("Window %d moved to %d,%d",
                        this->event.window.windowID, this->event.window.data1,
                        this->event.window.data2);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_RESIZED:
                printf("Window %d resized to %dx%d",
                        this->event.window.windowID, this->event.window.data1,
                        this->event.window.data2);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                printf("Window %d size changed to %dx%d",
                        this->event.window.windowID, this->event.window.data1,
                        this->event.window.data2);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                printf("Window %d minimized", this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                printf("Window %d maximized", this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_RESTORED:
                printf("Window %d restored", this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_ENTER:
                printf("Mouse entered window %d",
                        this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_LEAVE:
                printf("Mouse left window %d", this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                printf("Window %d gained keyboard focus",
                        this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                printf("Window %d lost keyboard focus",
                        this->event.window.windowID);
                cout << endl;
                break;
            case SDL_WINDOWEVENT_CLOSE:
                printf("Window %d closed", this->event.window.windowID);
                cout << endl;
                break;
        }
    }
}


void OE_SDL_WindowSystem::destroy(){
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
}
