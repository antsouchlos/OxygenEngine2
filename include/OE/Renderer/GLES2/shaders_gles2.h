#ifndef NRE_GLES2_SHADERS_H
#define NRE_GLES2_SHADERS_H

#include <OE/Renderer/shaders_gpu.h>


std::string NRE_GenGLES2VertexShader(nre::gpu::vertex_shader);
std::string NRE_GenGLES2PixelShader(nre::gpu::pixel_shader);

#endif
