#include "FE_GPU_API.h"

namespace FE_GL3_API{

    void init(FE_GPU_API* api, string title, int xa, int ya, bool IsFullscreen){


        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
        /// do the normal OpenGL initialization on Linux/maybe-Mac OS X/whatever-crap-pc-os :)
        //#ifdef FE_PC
        #ifdef FE_DEBUG
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        #endif
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        //#endif
        /// do OpenGL ES initialization if on android/ios/ubuntu-phone/whatever-crap-mobile-os
        //#ifdef FE_MOBILE

        //#endif // FE_MOBILE

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        /// clean up previous context
        //if(api->made_context) api->destroy();

        //cout << "RUNS" << endl;
        /// create window and OpenGL (ES) context
        api->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, xa, ya, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
        api->context = SDL_GL_CreateContext(api->window);

        glewExperimental = GL_TRUE;
        glewInit();

        glEnable( GL_BLEND );
        glEnable (GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc (GL_LESS);

        glEnable (GL_CULL_FACE);
        glCullFace (GL_BACK); /// cull back face
        glFrontFace (GL_CCW); /// GL_CCW for counter clock-wise

        SDL_GL_SetSwapInterval(1);
        int wi, he, xb, yb;

        SDL_GL_GetDrawableSize(api->window, &wi, &he);
        SDL_GetWindowPosition(api->window, &xb, &yb);

        glViewport(0, 0, wi, he);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        api->made_context = true;
        #ifdef FE_DEBUG
            auto str12 = glGetString(GL_VERSION);
            writeToLog("\nAPI: OpenGL "+ string(reinterpret_cast< char const * >(str12))+ "\n");
            //glDebugMessageCallback(&DebugCallback, NULL);
        #endif
        //cout << "inita" << endl;
    }

    void update(FE_GPU_API* api){
        //cout << "update" << endl;
        SDL_GL_SwapWindow(api->window);
        int wi, he;

        SDL_GL_GetDrawableSize(api->window, &wi, &he);
        //SDL_GetWindowPosition(api->window, &xb, &yb);
        if(wi != api->width && he != api->height){
            glViewport(0, 0, wi, he);
            api->width = wi; api->height = he;
        }

        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }
    void destroy(FE_GPU_API* api){

        SDL_GL_DeleteContext(api->context);
        SDL_DestroyWindow(api->window);
    }
};
