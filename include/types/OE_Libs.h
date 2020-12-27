#ifndef OE_LIBS_H_INCLUDED
#define OE_LIBS_H_INCLUDED

// ALL platform specific includes must go here

#ifdef __gnu_linux__

#define OE_PLATFORM_PC
#define OE_PLATFORM_LINUX
#define OE_RENDERER_OPENGL

#include <SDL2/SDL.h>

#elif defined(_WIN32)

#define OE_PLATFORM_PC
#define OE_PLATFORM_WINDOWS
#define OE_RENDERER_OPENGL
#define SDL_MAIN_HANDLED

#include <SDL.h>

#else

#define OE_PLATFORM_PC
#define OE_PLATFORM_UNKNOWN
#define OE_RENDERER_OPENGL

#include <SDL2/SDL.h>

#endif


#include <cassert>
#include <array>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <set>
#include <unordered_map>
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
#include <functional>
#include <iomanip>
#include <ctime>
#include <bitset>
#include <memory>
#include <atomic>

#endif // OE_LIBS_H_INCLUDED
