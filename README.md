```
 ________   ________     ________   _______   ________   _______        
/ ______ \  | ______|   |______  | | ____  | |______  | | ____  |     _
| |    | |  | |_____     ______| | | |   | |  ______| | | |   | |  __| |__
| |    | |  | ______|   |  ______| | |   | | |  ______| | |   | | |__   __|
| |____| |  | |_____    | |______  | |___| | | |______  | |___| |    |_|
\________/  |_______|   |________| |_______| |________| |_______|     
```
The Oxygen Engine (OE) is a hobbyist modular game engine written in C++ in development for learning purposes.
Originally it was an ongoing project in 2015-16, but it is stagnated and now is (hopefully) being finally restarted in 2020.

## Installation
This section will in the future describe how to install/compile/run on Linux and Windows.

## State of the project

2020/09/19 after the README commit:

Old 2016 version of the code was found somewhere deep in my Documents directory. The code can currently compile and run on (Arch) Linux with an old custom script and make.
The demo loads two simple objects with materials and displays them on the screen. Also keyboard and mouse events are recognized. Most of the time, it hangs on exit. Also the blender plugin exporter is borked, since it was written for blender 2.6...

## Initial TODO list
This should get the project started again with the basics working, so as to be able to add **actual** new features.

- Proper cleanup of code from 2016. (mostly working, but muh legacy cruft + renaming of files and classes)
- Rewrite of the build system to use [Meson](https://mesonbuild.com/).
- Get cross compilation done at least for Windows (and in the future Android\iOS\MacOS as well).
- Removal of outdated 'glew' dependency ([Glad](https://glad.dav1d.de/) should replace it, which is only a few auto-generated headers for OpenGL and not an external library). 
- Fix the blender plugin and update it for blender 2.8+.
- (Possible but difficult) autogenerated parser script (reader+writer) for carbon, so that classes can change easily without having to change the Carbon code manually by hand.