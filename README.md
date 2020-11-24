```
 ________    _______     ________   _______   ________   _______        
/ ______ \  | ______|   |______  | | ____  | |______  | | ____  |     _
| |    | |  | |_____     ______| | | |   | |  ______| | | |   | |  __| |__
| |    | |  | ______|   |  ______| | |   | | |  ______| | |   | | |__   __|
| |____| |  | |_____    | |______  | |___| | | |______  | |___| |    |_|
\________/  |_______|   |________| |_______| |________| |_______|     
```
The Oxygen Engine (OE) is a hobbyist modular 3D game engine written in C++ in development for learning purposes.
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

### Installing the blender plugin

Prerequisites: Blender (duh!)

The blender plugin now works again!
Only for materials, meshes, lights and cameras as of 2020/11/03.
But textures weren't working anyway in the engine itself, so whatever. They will be added later.

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

## State of the project

2020/09/19 after the README commit:

Old 2016 version of the code was found somewhere deep in my Documents directory. The code can currently compile and run on (Arch) Linux with an old custom script and make.
The demo loads two simple objects with materials and displays them on the screen. Also keyboard and mouse events are recognized. Most of the time, it hangs on exit. Also the blender plugin exporter is borked, since it was written for blender 2.6...

2020/10/02:

Reworked the blender plugin exporter (in the 'addons' directory, tested on blender 2.9.
Reworked OE_Mesh and the way vertices/triangles are stored, now similar to how .obj and COLLADA store them
Rewrite of C++ engine in progress, currently in a borked state.
Porting of build system hasn't even started yet

2020/10//06:

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

Added an implementation of a general directional top-down parser for context-free languages (with some restrictions). It is a very naive implementation,
lacking in MANY departments (frankly, at some points poorly implemented out of boredom - input tokens have to be separated by spaces for example,
because I just couldn't be bothered to write a lexical analyzer). With all it's faults, this implementation has one really nice feature: Since it is not 
recursive-decent implementation, one is quite literaly able to input a grammar, recompile (although with a bit of effort, even recompiling isn't necessary;
the grammar could be loaded from a configuration file) and run the parser.

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

- ~~Basic Material and Lighting support.~~(DONE)
- ~~Rewrite the scenegraph/renderer/API and CSL_Interpreter to a more efficient types format using a templated OE_SharedIndexMap and ```std::shared_ptr```. (depends on antsouchlos' work on the parser/interpreter)~~(DONE)
- Support multiple shaders
- Support multiple render targets and framebuffers in the Renderer API
- Implement Clustered Forward Renderer or Deferred Renderer or Tiled renderer etc, but probably the clustered one.
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


## TODO list for antsouchlos parser
The C++ interpreter/writer is working with the new format and i have prepared a simple Makefile, so you can test them yourself. Just hit ```make``` in the top level directory and then execute ```./CSL_Test```. (UPDATE: You will need to (un)comment some stuff)

I have prepared benchmark objects exported from blender in order to measure the performance.
Test 1 and 2 is the old challenge_car.csl from 2015 with 33k vertices, 28.5k normals and 22.2 triangles.

Test 3 is a 198k vertices, 400k normals, 131k triangles, 132k uvcoords object. It is AFAIK approximately the size of some tanks (T95) in world of tanks, cars in Project Cars 2 or of a typical high-res video game character.
An impressive 2 sec loading time!!! (after reading from disk)

Performance is more than good enough (O(n)) and i like the extensible design of the parser and interpreter, BUT there are still performance optimizations possible in the parser IMHO. You basically copy the whole source code and allocate heap memory all the time...

My ideas:
 - for CSL_Lexer and CSL_Parser to store indices in the source code
 - for CSL_Node->id and CSL_Node->args to store the indices in the source code instead of copying the source code string itself.
 - Use string.substr() in the interpreter
 - for CSL_Node->type to be stored as an enum. (because the types are firmly defined right?)
 - Same optimizations ofr CSL_Token and CSL_Char (formerly CChar and CToken)


On the bad side, there are A LOT and i mean ABOUT 198MBs(!) of memory leaks coming from the parser after all of the 3 tests! Pleaaase fix!
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

1. All object rotation modes inside blender must be ```Quaternion WXYZ```, not ```XYZ Euler``` or anything else, otherwise the rotations won't be written correctly.
2. Each object that should be visible MUST have a material.
3. The materials MUST use nodes and have a ```Specular``` node as the surface, otherwise there will be errors on the export into .csl. (More will be added later on)
4. There must be one scene and one camera (as of 2020/11/12) and the camera should be named "Camera". This way upon import in ```oe_main_test.cpp``` and the ```builddir/OE_Test``` executable, it is possible to move around the camera using WASD +Q/E and mouse movement with spacebar to freely take a look around the scene.






