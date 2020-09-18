#ifndef FE_LIBS_H_INCLUDED
#define FE_LIBS_H_INCLUDED

#define FE_DEBUG 1
//#define FE_ATTRIB_OPENGL
#ifdef __gnu_linux__
    #define FE_PC
    #define FE_ATTRIB_OPENGL
    #define FE_LINUX
    #include <cstdlib>
    #include <cstdint>
    #include <GL/glew.h>
    #include <SDL2/SDL.h>
    //#include <SDL2/SDL_opengl.h>
#endif

#ifdef _WIN32
    #define FE_PC
    #define FE_ATTRIB_OPENGL
    #define FE_ATTRIB_DIRECT3D
    #define FE_WINDOWS
    // include necessary libraries
    #include <cstdlib>
    //#define GLEW_STATIC
    #include <GL/glew.h>
    #define SDL_MAIN_HANDLED
    #include "SDL.h"
    //#include <GL/glx.h>
#endif

#include <assert.h>
#include <array>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <list>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <typeinfo>
#include <ctime>
#include <cmath>
#include <exception>
#include "FError.h"

class FE_THREAD_SAFETY_OBJECT{
    public:
        FE_THREAD_SAFETY_OBJECT();
        virtual ~FE_THREAD_SAFETY_OBJECT() = 0;

        SDL_mutex* wmutex = nullptr;
        bool changed = false;
        void createMutex();
        void lockMutex();
        void unlockMutex();

};
void writeToLog(string);
void writeToLog(const char*);




#endif // FE_LIBS_H_INCLUDED
