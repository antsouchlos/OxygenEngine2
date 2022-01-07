```
 ________    _______     ________   _______   ________   _______        
/ ______ \  | ______|   |______  | | ____  | |______  | | ____  |     _
| |    | |  | |_____     ______| | | |   | |  ______| | | |   | |  __| |__
| |    | |  | ______|   |  ______| | |   | | |  ______| | |   | | |__   __|
| |____| |  | |_____    | |______  | |___| | | |______  | |___| |    |_|
\________/  |_______|   |________| |_______| |________| |_______|     
```
The Oxygen Engine (OE) is a hobbyist modular 3D game engine written in C++, in development for learning purposes.

It currently implements a custom file format with asynchronous loading (.csl), a multithreaded pipeline with game logic and renderer running in separate threads, an event handler and an OpenGL 3.1+/ES 3.0+ Renderer using SDL2, GLad and GLM. This means at least a Shader Model 4.0/DX10-capable GPU is required to run OE.

Originally it was an ongoing project in 2015-16, but it is stagnated and now is (hopefully) being finally restarted in 2020.

## Installation

### Before running


Since the assets needed to run the example programs are quite large, they are located
in a separate repositry, which is linked as a submodule. This means that before the example
programs can be executed, the assets submodule as to be cloned:

```shell
$ git submodule init && git submodule update
```

### Compile on Linux

Prerequisites: SDL2, meson and ninja.

The CC and CXX environment variables are optional. Meson can use ```ccache``` automatically if it exists as well.
For the very first time setup run:

```shell
$ CC=gcc CXX=g++ meson builddir
```

Otherwise, just run:
```shell
$ meson compile -C builddir
```
After compilation is finished,
in order to execute the demos, just run:
```shell
$ ./builddir/CSL_Test
```
or:
```shell
$ ./builddir/OE_Test
```

### Cross compile on Linux for Windows

Prerequisites: MinGW-w64, wine, SDL2, meson, ninja.

Run this script, which takes care of the dirty work for you automatically.
Note that this will compile SDL2 together with the Oxygen Engine.
```shell
$ ./cross_linux2windows
```

It is also possible to use ```meson compile -C builddir```, but only **after** the above script
has been run **at least once** to generate the ```builddir``` directory and apply the needed hacks.

Then after compilation is finished, on Linux it is possible to test with:

```shell
$ wine builddir/OE_Test.exe
```
or
```shell
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
```shell
$ ./cross_web
```

It is also possible to use ```meson compile -C builddir```, but only AFTER the above script
has been run at least ONCE to generate the ```builddir``` directory and apply the needed hacks.

On another terminal in order to launch a webserver on the base directory, run:
```shell
$ emrun --no_browser builddir/oe_test.html
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
