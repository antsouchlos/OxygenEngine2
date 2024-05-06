project('Oxygen Engine', ['cpp', 'c'], default_options : ['cpp_std=c++20', 'buildtype=debugoptimized'])

src = ['src/Renderer/glad.c',
       'src/Renderer/api_gpu.cpp',
       'src/Renderer/shaders_gpu.cpp',
       'src/Renderer/renderer_main.cpp',
       'src/Renderer/renderer_legacy.cpp',
       'src/Renderer/renderer_utils.cpp',
       'src/Renderer/drawcall_container.cpp',
       'src/Renderer/GL3/api_gl3.cpp',
       'src/Renderer/GL3/shaders_gl3.cpp',
       'src/Renderer/GLES2/api_gles2.cpp',
       'src/Renderer/GLES2/shaders_gles2.cpp',
       'src/Renderer/DataHandler/render_data.cpp',
       'src/Renderer/DataHandler/data_handler.cpp',
       'src/winsys_sdl2.cpp',
       'src/api_helpers_oe.cpp',
       'src/api_oe.cpp',
       'src/math_oe.cpp',
       'src/dummy_classes.cpp',
       'src/global_variables.cpp',
       'src/task_manager.cpp',
       'src/task.cpp',
       'src/error_oe.cpp',
       'src/Events/mutex_condition.cpp',
       'src/Events/event_handler.cpp',
       'src/Events/input_event_handler.cpp',
       'src/Events/event_parser.cpp',
       'src/Events/event.cpp',
       'src/Carbon/interpreter.cpp',
       'src/types/base_types.cpp',
       'src/types/camera.cpp',
       'src/types/light.cpp',
       'src/types/material.cpp',
       'src/types/mesh.cpp',
       'src/types/object.cpp',
       'src/types/polygon_storage.cpp',
       'src/types/scene.cpp',
       'src/types/tcm.cpp',
       'src/types/texture.cpp',
       'src/types/vertex_group.cpp',
       'src/types/viewport_config.cpp',
       'src/types/vertex_storage.cpp',
       'src/types/world.cpp']


internal_inc = include_directories('./include') 


# external dependencies
cmake = import('cmake')
sub_proj = cmake.subproject('ctre')
ctre_dep = sub_proj.dependency('ctre')

glm_dep = dependency('glm')


# Compile examples


if meson.get_compiler('c', native : false).get_id() != 'emscripten'
    sdl2_dep = dependency('sdl2')
    dl_dep = dependency('dl', required : false)
    oe_lib = static_library('OxygenEngine', sources : src, dependencies : [sdl2_dep, dl_dep, glm_dep, ctre_dep], include_directories : [internal_inc])
else
    thread_dep = dependency('threads')
    oe_lib = static_library('OxygenEngine', sources : src, dependencies : [thread_dep, glm_dep, ctre_dep], include_directories : [internal_inc])
endif

oe_dep = declare_dependency(link_with : oe_lib, include_directories : [internal_inc])

if meson.get_compiler('c', native : false).get_id() != 'emscripten'
    executable('CSL_Test', sources : 'examples/csl_main_test.cpp', dependencies : [oe_dep, glm_dep, ctre_dep], win_subsystem: 'windows')
    executable('OE_Test', sources : 'examples/oe_main_test.cpp', dependencies : [oe_dep, glm_dep], win_subsystem: 'windows')
    executable('EventsTasksTest', sources : 'examples/events_tasks_test.cpp', dependencies : [oe_dep, glm_dep], win_subsystem: 'windows')
else
    #executable('CSL_Test', sources : 'esamples/csl_main_test.cpp', dependencies : [oe_dep, ctre_dep], win_subsystem: 'windows')
    executable('OE_Test', sources : 'examples/oe_main_test.cpp', dependencies : [oe_dep, glm_dep, ctre_dep], win_subsystem: 'windows')
endif


# Compile unit tests


if not meson.is_cross_build()
    gtest_dep = dependency('gtest_main')

    test_executable = executable('testprog', 'test/example.cpp', dependencies : gtest_dep)
    test('gtest test', test_executable)
endif
