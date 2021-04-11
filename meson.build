project('Oxygen Engine', ['cpp', 'c'], default_options : ['cpp_std=c++17', 'buildtype=debugoptimized'])

src = ['src/OE_SDL_WindowSystem.cpp',
       'src/Renderer/glad.c',
       'src/Renderer/NRE_GL3_API.cpp',
       'src/Renderer/NRE_GPU_API.cpp',
       'src/Renderer/NRE_GPU_Shaders.cpp',
       'src/Renderer/NRE_GL3_Shaders.cpp',
       'src/Renderer/NRE_RendererMain.cpp',
       'src/Renderer/NRE_RenderData.cpp',
       'src/Renderer/NRE_DrawCallContainer.cpp',
       'src/Events/OE_MutexCondition.cpp',
       'src/OE_API_Helpers.cpp',
       'src/OE_API.cpp',
       'src/OE_Math.cpp',
       'src/OE_DummyClasses.cpp',
       'src/OE_TaskManager.cpp',
       'src/OE_Task.cpp',
       'src/OE_Error.cpp',
       'src/Events/OE_EventHandler.cpp',
       'src/Events/OE_InputEventHandler.cpp',
       'src/Events/OE_EventParser.cpp',
       'src/Events/OE_Event.cpp',
       'src/Carbon/CSL_Interpreter.cpp',
       'src/types/OE_TypesBase.cpp',
       'src/types/OE_Camera.cpp',
       'src/types/OE_Light.cpp',
       'src/types/OE_Material.cpp',
       'src/types/OE_Mesh.cpp',
       'src/types/OE_Object.cpp',
       'src/types/OE_PolygonStorage.cpp',
       'src/types/OE_Scene.cpp',
       'src/types/OE_TCM.cpp',
       'src/types/OE_Texture.cpp',
       'src/types/OE_VertexGroup.cpp',
       'src/types/OE_ViewportConfig.cpp',
       'src/types/OE_VertexStorage.cpp',
       'src/types/OE_World.cpp']


internal_inc = include_directories('./include')

#thread_dep = dependency('threads')
sdl2_dep = dependency('sdl2')

oe_lib = static_library('OxygenEngine', sources : src, dependencies : [sdl2_dep], include_directories : internal_inc)

oe_dep = declare_dependency(link_with : oe_lib, include_directories : internal_inc)

executable('CSL_Test', sources : 'csl_main_test.cpp', dependencies : [oe_dep, sdl2_dep], win_subsystem: 'windows')
executable('OE_Test', sources : 'oe_main_test.cpp', dependencies : [oe_dep, sdl2_dep], win_subsystem: 'windows')
