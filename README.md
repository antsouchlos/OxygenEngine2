```
 ________    _______     ________   _______   ________   _______        
/ ______ \  | ______|   |______  | | ____  | |______  | | ____  |     _
| |    | |  | |_____     ______| | | |   | |  ______| | | |   | |  __| |__
| |    | |  | ______|   |  ______| | |   | | |  ______| | |   | | |__   __|
| |____| |  | |_____    | |______  | |___| | | |______  | |___| |    |_|
\________/  |_______|   |________| |_______| |________| |_______|     
```
The Oxygen Engine (OE) is a hobbyist modular 3D game engine written in C++ in development for learning purposes.

It currently implements a custom file format with asynchronous loading (.csl), a multithreaded pipeline with game logic and renderer running in separate threads, an event handler and an OpenGL 3.1+/ES 3.0+ Renderer using SDL2, GLad and GLM. This means at least a Shader Model 4.0/DX10-capable GPU is required to run OE.

Originally it was an ongoing project in 2015-16, but it is stagnated and now is (hopefully) being finally restarted in 2020.

## Installation
This section will in the future describe how to install/compile/run on Linux and Windows.

### Compile on Linux

Prerequisites: SDL2, meson and ninja.

The CC and CXX environment variables are optional. Meson can use ```ccache``` automatically if it exists as well.
For the very first time setup run:

```
$ CC=gcc CXX=g++ meson builddir
```

Otherwise, just run:
```
$ meson compile -C builddir
```
After compilation is finished,
in order to execute the demos, just run:
```
$ ./builddir/CSL_Test
```
or:
```
$ ./builddir/OE_Test
```

### Cross compile on Linux for Windows

Prerequisites: MinGW-w64, wine, SDL2, meson, ninja.

Run this script, which takes care of the dirty work for you automatically.
Note that this will compile SDL2 together with the Oxygen Engine.
```
$ ./cross_linux2windows
```

It is also possible to use ```meson compile -C builddir```, but only AFTER the above script
has been run at least ONCE to generate the ```builddir``` directory and apply the needed hacks.

Then after compilation is finished, on Linux it is possible to test with:

```
$ wine builddir/OE_Test.exe
```
or
```
$ wine builddir/CSL_Test.exe
```

All required .dll files:

```
libwinpthread-1.dll

```

are in the ```builddir``` directory and must be distributed together with the ```.exe``` files.

### Compile for Web

Prerequisites: emscripten, meson, ninja

Run this script, which takes care of the dirty work for you automatically.
Note that SDL2 is already included in emscripten.
```
$ ./cross_web
```

It is also possible to use ```meson compile -C builddir```, but only AFTER the above script
has been run at least ONCE to generate the ```builddir``` directory and apply the needed hacks.

On another terminal in order to launch a webserver on the base directory, run:
```
emrun --no_browser builddir/oe_test.html
```

After that on your browser (Firefox or Chrome/Chromium-based) go to:
```
http://localhost:6931/oe_test.html
```
or wherever you put your webserver. Now the engine demo should run.

### Installing the blender plugin

Prerequisites: Blender (duh!)

The blender plugin now works again!
Only for materials, meshes, lights and cameras as of 2020/11/03.
But textures weren't working in the engine itself anyways, so whatever. They will be added later.

From blender:
- go to Edit->Preferences->File Paths->Data
- add the directory of the project to 'Scripts'.
- Restart blender.
- Go to Edit->Preferences->Addons
- enable 'Import-Export: Export Oxygen Engine (.csl)'

To export scenes/objects to .csl format:
- Go to File->Export->Oxygen Engine (.csl)
- Possibly choose export options on the left
- Choose path and click 'Export CSL'

In order for the objects to be exported and then displayed correctly in the engine: 

- Each object that should be visible MUST have a material. 'Use Nodes' must be on (the default).
- The camera should preferably be named "Camera". This way upon import in ```oe_main_test.cpp``` and the ```builddir/OE_Test``` executable, it is possible to move around the camera using WASD +Q/E and mouse movement with spacebar to freely take a look around the scene.

## State of the project

2020/09/19 after the README commit:

Old 2016 version of the code was found somewhere deep in my Documents directory. The code can currently compile and run on (Arch) Linux with an old custom script and make.
The demo loads two simple objects with materials and displays them on the screen. Also keyboard and mouse events are recognized. Most of the time, it hangs on exit. Also the blender plugin exporter is borked, since it was written for blender 2.6...

2020/10/02:

Reworked the blender plugin exporter (in the 'addons' directory, tested on blender 2.9.
Reworked OE_Mesh and the way vertices/triangles are stored, now similar to how .obj and COLLADA store them
Rewrite of C++ engine in progress, currently in a borked state.
Porting of build system hasn't even started yet

2020/10/06:

Reworked the C++ interpreter and writer classes to work with the updated .csl format. Organized and renamed CNode, CLexer etc into CSL_Node, CSL_Parser etc. into proper header and source files. Refactored old 2016 classes and renamed them with OE_ at the start. With the new design of the CSL format, The size of the challenge car was reduced from 10.4MB to 3.3MB (!).

Now I will work on integrating the event handler, renderer and the rest to create a working window with renderer,
so i can get the basics working. Then work can start by antsouchlos on the physics part.

2020/10/07:

Added optimizations to the loading of vertices. Switched to unordered_map in many OE base classes.
Meshes can now choose dynamically at runtime between unordered map and ordered map according to the number of
vertices and other mesh data.

2020/10/15:

Added the event handler and task manager. Now they are much more robust than previously and never crash.
Also added proper synchronizations and dummy physics/renderer/window system classes. They will be documented shortly. Proper unsynchronized thread support has also been added.

The design is a little different from 2016, in that synchro threads are synchronized with each other two or three times each frame normally. Now i will work on integrating the renderer and a basic API, then for the build system.

2020/10/21:

Added window system support, proper SDL and OpenGL library support. Added [GLM](https://github.com/g-truc/glm) 
as our default math library, though this can be changed in the future if desired.

Now our Oxygen Engine can properly create and destroy a windowed/fullscreen OpenGL/OpenGL ES renderer without GLEW and with less fuss.

2020/10/31:

Renderer now works! It can render the challenge car (in contrast to the 2016 version) and any other object.
Currently uses mesh normals as vertex colors. In the Demo one can move the camera with WASD to see around the object meshes.

Added several more .csl example files as a test for the renderer and parser/interpreter.

~~Next task: Proper build system (meson) integration with unit tests.~~

2020/11/01:

Added meson build system support, it was much easier than i thought. It took 30 mins.

2020/11/03:

Added cross compilation support. Some hacks were needed in order to get SDL2 to compile, but with Meson
it was still straightforward. It will have to be updated when more libraries are added.

2020/11/12

Added keyboard/mouse input API. Added basic Object movement API. Updated types database to use shared_ptr. Renderer can now update positions of different objects independently of one another. Added basic materials and hardcoded a single point light at (0, 0, 0) to the renderer. Renderer is now ready for the physics engine.

Now to evaluate whether i will use one of the 100000000 approaches possible to structure a proper renderer.
This will take a while, so the code will be touched sparingly for some time.

2020/11/24 - Andreas

CAUTION: The following does not constitute any real progress on the parser that will be used by the OxygenEngine. Nevertheless, it is a substantial part of my process for building an understanding about parsing algorithms.

Added a breadth-first implementation of a general directional top-down parser for context-free languages (with some restrictions). It is a very naive implementation,
lacking in MANY departments (frankly, at some points poorly implemented out of boredom - input tokens have to be separated by spaces for example,
because I just couldn't be bothered to write a lexical analyzer). With all it's faults, this implementation has one really nice feature: Since it is not 
a recursive-decent implementation, one is quite literaly able to input a grammar, recompile (although with a bit of effort, even recompiling isn't necessary;
the grammar could be loaded from a configuration file) and run the parser.

2020/11/24 - philsegeler

Primarily renderer stuff:
- Added NRE_GPU_Shader classes enabling the use of different shaders for different objects and modes.  Shaders are also reusable for different objects to prevent unnecessary pipeline changes.
- Two shading modes are supported: ```regular``` and ```normals```. 
- Fixed some bugs regarding the window system and OpenGL ES.
- Added a Z-pre-pass to the renderer for optimization and in preparation for implementing a Light-Indexed-Deferred Renderer.
- Draw calls are now sorted.
- The renderer can now be restarted on the fly at runtime.
- Overhauled the event handler and pushed (almost) all SDL2-specific event handling code in ```OE_SDL_WindowSystem.h/cpp```. Now it is more maintenable.
- Added possibility to change shading mode and restart the renderer in the demo using left/right mouse click.

2020/12/07 - philsegeler

Renderer stuff
- Added basic gamma correction to both OpenGL and OpenGL ES. Colors now look correct.
- Added wireframe rendering (only on desktop OpenGL and for debugging purposes)
- Added Bounding Box calculation and rendering
- Use F5/F6/F7/F8 to change render modes in the demo.

2020/12/20 - antsouchlos

Uploaded new Implementation of Lexer onto parser_v2 branch - I know I know, I should not just commit the whole thing when it's done.
This wasn't that much work though

Error Handling must be possible another way. Right now, essentially every time the char iterator is incremented, an extra if statement has to be
written, to check the iterator against the length of the string. Maybe create a wrapper for the integer that is the iterator, that does this automatically?
Does an Iterator like the wrapper just proposed already exist? Is there a way to automatically throw an exception, when one tries to access a character of
a string outside of it's size()? I really don't fancy writing a wrapper for std::basic_string_view

2020/12/23 - antsouchlos

New Lexer and Parser are now done and reasonably optimized. No memory leaks and quite the performance impovement compared to the previous ones.
We now embark on the great journey of actually integrating the new Lexer and Parser into OxygenEngine, i.e. rewriting the Interpreter while leaving
it's interface unchanged

2020/12/25 - philsegeler

Changed user-facing API of OE to starting with ```oe::``` and using underscores instead of camelCase-like naming conventions + fixes in renderer.

2020/12/28 - philsegeler

Overhauled math library to be easily extensible. 
Optimized Natrium Renderer by offloading all OpenGL buffer data calls into the ```updateSingleThread()``` method instead of ```updateData```. This would make synchronization between the renderer and the physics engine more performant.

2020/12/29 - philsegeler

NRE: Fixed an OpenGL bug on Windows/Intel. Updated with TODO list for anything but renderer.

2021/01/05 - philsegeler

API: Added variadic templates and removed all ```void*``` and ```nullptr``` unless absolutely necessary (such as in SDL). BTW Andreas will you finally update the parser readme????

2021/01/08 - philsegeler

Finalized implementation of error handling. Now everything apart form core OE (User-facing API functions, custom tasks, custom events, custom unsynchronized threads, renderer, physics) are wrapped in ```try{...}catch{...}``` blocks and the engine now should never crash unless a segfault happens.

2021/01/14 - philsegeler

Finalized rewrite of ```OE_SharedIndexMap```. Now supports iterators, asynchronous ```append``` and ```extend```, specialized iterators only for changed and deleted objects ```.changed()```, ```.deleted()```, per-frame synchronization (```synchronize()```), error handling and warnings. Performance improvements could still be made though. ```renderer->updateData()``` is can now be much cleaner and straightforward as well.

2021/02/01 - philsegeler

Two fixes, because i was bored.

2021/04/11 - philsegeler

Added scene and (basic, one fullscreen camera at any given time) viewport config handling to renderer.
Now if a viewport is undefined, it will generate one automatically, named "default".

2021/04/12 - philsegeler

One can now swap worlds in the demo with keys 1 and 2. Swapping between 'challenge_car.csl' and 'OE_Demo.csl' leak-free and gracefully!

2021/04/14 - philsegeler

Blender addon improvements. Now can export rotations and material colors correctly in all cases. Also exports a default viewport config.

2021/04/21 - philsegeler

NRE: Added initial bounding spheres rendering. Needs only a few improvements.

2021/04/26 - philsegeler

Under the hood renderer improvements and necessary additions:
- Added multiple framebuffer support (now offscreen rendering to a higher resolution buffer is happening)
- Added proper cross-platform gamma support
- Added post-processing shader possibility

2021/04/28 - philsegeler

Added ```OE_NetworkingBase``` base class for Silicon in ```OE_DummyClasses.h/.cpp```. It runs asynchronously to the main engine. the networking manager can communicate through ```oe::create_event(...)``` and ```oe::broadcast_event()``` with ```OE_TaskManager```.

2021/05/02 - philsegeler

NRE: Added point light symbolic instanced rendering with shared UBO. Works suprisingly well. Also updated ```oe_shared.zip```.

2021/05/16 - philsegeler

NRE: Prepared for GLES2 compatibility renderer. 

2021/05/21 - philsegeler

NRE: Added renderbuffer + fixed uniforms in programs

2021/05/27 - philsegeler

Added WebGL version running on Chrome only. Loading times have gone x10+.

## Initial TODO list for philsegeler
This should get the project started again with the basics working, so as to be able to add **actual** new features.

- ~~Proper cleanup of code from 2016. (mostly working, but muh legacy cruft + renaming of files and classes) (IN PROGRESS, 2020/10/15: only renderer + API missing)~~ (DONE)
- ~~Rewrite of the build system to use [Meson](https://mesonbuild.com/).~~ (DONE: Easier than i thought)
- ~~Get cross compilation done at least for Windows (and in the future Android\iOS\MacOS as well).~~(DONE: Linux-to-Windows)
- ~~Removal of outdated 'glew' dependency ([Glad](https://glad.dav1d.de/) should replace it, which is only a few auto-generated headers for OpenGL and not an external library).~~ (DONE)
- ~~Fix the blender plugin and update it for blender 2.8+.~~ (DONE)
- ~~(Possible but difficult) autogenerated parser script (reader+writer) for carbon, so that classes can change easily without having to change the Carbon code manually by hand.~~(WONTFIX)

## TODO list for philsegeler - Renderer

Finally the initial TODO list is ready and done! Now i can start working on the renderer properly!
UPDATE 2020/11/12: It was enhanced.

Sidelines:
- Refactor GPU API, ~~introduce renderbuffer~~, drawdata (NEW)
- Introduce OpenGL ES 2.0 compatibility renderer. (NEW)

- ~~Basic Material and Lighting support.~~(DONE)
- ~~Rewrite the scenegraph/renderer/API and CSL_Interpreter to a more efficient types format using a templated OE_SharedIndexMap and ```std::shared_ptr```. (depends on antsouchlos' work on the parser/interpreter)~~(DONE)
- ~~Support multiple shaders~~(DONE)
- ~~Implement rendering of bounding spheres~~(DONE)
- Implement rendering of cones
- Implement sorting front-to-back or back-to-front of objects
- Implement Directional Light rendering pass.
- Implement Point+Area Light-Indexed rendering pass.
- ~~Support multiple render targets and framebuffers in the Renderer API~~(DONE)
- Implement Light Indexed Deferred Renderer.
- Integration of the ```SDL2_image``` library and basic Texture streaming support with diffuse maps.
- Integration of the ```SDL2_ttf``` + ```'freetype``` libraries and basic font loading and text-rendering support.

In the future and in no particular order:

- Android support
- Skeletal animation and bones support (depends on File format, API and physics engine support)
- Antialiasing support
- Normal maps support.
- Stencil/Shadow Mapping support
- Basic Occlusion Culling
- Alpha transparency support
- Cube Map Reflections support
- SSAO support
- Level-of-detail and mipmapping/anisotropic filtering support
- Multi-Texturing and Stencil-Texturing support.
- (Maybe) Pseudo-2D support (Orthographic rendering, Sprite(?))
- Particle effects
- Usage of OpenGL AZDO extensions to improve performance
- Physically-based-rendering(?!)
- Tesselation (?!)
- MacOS/iOS support

## TODO list for philsegeler - Anything but renderer

Now that it was decided that C++17 will be the minimum requirement:

- ~~Investigate possibility of using variadic templates in the API task and event function type arguments. Either by storing function parameters directly or using ```std::bind``` are two possibilities.~~(DONE using ```std::bind```)
- ~~Handle errors in the API properly.~~ (DONE)
- Investigate possible optimizations in the Event handler
- ~~Refactor ```OE_SharedIndexMap``` (again!) to remove previous defects and optimize with iterators and custom internal containers.~~(DONE)
- Use the following naming convention ```this->member_``` (with underscore at the end) for class member variables.


## TODO list for antsouchlos parser
The C++ interpreter/writer is working with the new format and i have prepared a simple Makefile, so you can test them yourself. Just hit ```make``` in the top level directory and then execute ```./CSL_Test```. (UPDATE: You will need to (un)comment some stuff)

I have prepared benchmark objects exported from blender in order to measure the performance.
Test 1 and 2 is the old challenge_car.csl from 2015 with 33k vertices, 28.5k normals and 22.2 triangles.

Test 3 is a 198k vertices, 400k normals, 131k triangles, 132k uvcoords object. It is AFAIK approximately the size of some tanks (T95) in world of tanks, cars in Project Cars 2 or of a typical high-res video game character.
An impressive 2 sec loading time!!! (after reading from disk)

Performance is more than good enough (O(n)) and i like the extensible design of the parser and interpreter, BUT there are still performance optimizations possible in the parser IMHO. You basically copy the whole source code and allocate heap memory all the time...

My ideas:
 ~~- for CSL_Lexer and CSL_Parser to store indices in the source code~~
 ~~- for CSL_Node->id and CSL_Node->args to store the indices in the source code instead of copying the source code string itself.~~
 - Use string.substr() in the interpreter
 ~~- for CSL_Node->type to be stored as an enum. (because the types are firmly defined right?)~~
 ~~- Same optimizations ofr CSL_Token and CSL_Char (formerly CChar and CToken)~~


~~On the bad side, there are A LOT and i mean ABOUT 198MBs(!) of memory leaks coming from the parser after all of the 3 tests! Pleaaase fix!~~
Check with ```valgrind --leak-check=full ./builddir/CSL_Test```.

```
$ ./builddir/CSL_test
CSL TEST BEGIN
CSL TEST READ FILE: 0.015779
CSL TEST PARSE: 0.235009
CSL TEST INTERPRET: 0.038954
CSL TEST WRITER 0.135715
CSL TEST BEGIN 2
CSL TEST READ FILE: 0.005671
CSL TEST PARSE: 0.229721
CSL TEST INTERPRET: 0.040373
CSL TEST WRITER 0.133864
CSL TEST BEGIN 3
CSL TEST READ FILE: 0.040365
CSL TEST PARSE: 1.65175
CSL TEST INTERPRET: 0.407525
CSL TEST WRITER 1.18851

```

Run ```diff challenge_car_copy.csl challenge_car_copy2.csl``` to verify correctness

## TODO list for antsouchlos physics engine

The rendere, types and engine itself are now ready for the introduction of the physics engine. :)
Now all work into physics, renderer, the engine itself and the interpreter/parser can happen independently of one another.

To do this you should first subclass ```OE_PhysicsEngineBase``` found in ```OE_DummyClasses.h``` and implement the virtual functions.
You will have access to all objects through the API if you wish, which is in ```OE_API.h```. (You can include that in your .cpp files.)
To get and idea how you can do it you can take a look at ```Renderer/NRE_RenderData.cpp```.

The physics engine is supposed to handle ALL and i really mean ALL movements of the engine itself. That also includes (apart from collision detection and regular physics updates) things like child objects following their parents, updating of animations, camera following algorithms and anything that is not supposed to be calculated by the user manually. The renderer only uses existing object and position data.

I highly recommend that after collision detection is finished you go with rigid body mechanics and first/third person character physics (that means moving around, jumping, enforcing boundaries and constraints, etc.)

You are fully in charge of the whole physics and movement API and the way physics properties are stored in the .csl files and/or imported from blender. The only things that are important to work as is for the renderer are the ```OE_Object::GetModelMatrix()``` and ```OE_Object::GetViewMatrix()``` methods, since they are used to upload updated object and camera positions to the GPU.

To test the physics engine you can now reliably export from blender and import directly into .csl in the engine. You just need to make sure that:





