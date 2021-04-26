#ifndef NRE_GPU_API_H_INCLUDED
#define NRE_GPU_API_H_INCLUDED

#include <types/OE_TypesBase.h>
#include <Renderer/NRE_GPU_Shaders.h>
#include <Renderer/NRE_Exceptions.h>
#include <atomic>

/** platform-specific namespaces
  * -OpenGL ES 3.0/ OpenGL 3.2
  * -OpenGL 4.x
  * -OpenGL 4.5 + GL_ARB_bindless_texture (UPDATE 2020: OpenGL 3.3/ OpenGL ES 3.0 + extensions
  * -Direct3D 9 (maybe, 5% chance to happen UPDATE 2020: ZERO chance to happen) 
  */

struct NRE_GPU_VertexLayoutInput{
    std::size_t vertex_buffer;
    std::size_t offset;
    unsigned int amount;
    unsigned int stride;
    
    NRE_GPU_VertexLayoutInput();
    NRE_GPU_VertexLayoutInput(std::size_t, std::size_t, unsigned int, unsigned int);
};

enum NRE_GPU_BUFFER_USAGE{
    NRE_GPU_DYNAMIC,
    NRE_GPU_STATIC,
    NRE_GPU_STREAM
};

enum NRE_GPU_RENDERMODE{
    NRE_GPU_REGULAR_BACKFACE,
    NRE_GPU_Z_PREPASS_BACKFACE,
    NRE_GPU_AFTERPREPASS_BACKFACE,
    NRE_GPU_REGULAR_BOTH,
    NRE_GPU_TRANSPARENT_BACKFACE
};

enum NRE_GPU_TEXTURE_TYPE{
    NRE_GPU_RGB,
    NRE_GPU_SRGB,
    NRE_GPU_RGB_U16,
    NRE_GPU_FLOAT,
    NRE_GPU_RGBA,
    NRE_GPU_RGB10_A2,
    NRE_GPU_SRGBA,
    NRE_GPU_RGBA_U16,
    NRE_GPU_DEPTHSTENCIL
};

enum NRE_GPU_TEXTURE_FILTER{
    NRE_GPU_LINEAR,
    NRE_GPU_NEAREST
};


/** NRE_GPU_API provides a platform-independent
  * interface for accessing the GPU on differing
  * systems for the renderer and conserves precious
  * development time.
  */

class NRE_GPU_API : public OE_THREAD_SAFETY_OBJECT{
public:
    NRE_GPU_API();
    virtual ~NRE_GPU_API();
    
    virtual void update(uint32_t, uint32_t);
    virtual void destroy();
    
    virtual std::string getRenderingAPI();
    
    virtual std::size_t newVertexBuffer();
    virtual std::size_t newVertexLayout();
    virtual std::size_t newIndexBuffer();
    virtual std::size_t newProgram();
    virtual std::size_t newUniformBuffer();
    virtual std::size_t newFrameBuffer();
    virtual std::size_t newTexture();
    
    virtual void setVertexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    virtual void setVertexBufferData(std::size_t, const std::vector<float>&, std::size_t);
    virtual void setVertexBufferMemoryData(std::size_t, const std::vector<float>&, NRE_GPU_BUFFER_USAGE);
    virtual void deleteVertexBuffer(std::size_t);
    
    virtual void setIndexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    virtual void setIndexBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t);
    virtual void setIndexBufferMemoryData(std::size_t, const std::vector<uint32_t>&, NRE_GPU_BUFFER_USAGE);
    virtual void deleteIndexBuffer(std::size_t);
    
    virtual void setUniformBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    virtual void setUniformBufferData(std::size_t, const std::vector<float>&, std::size_t);
    virtual void setUniformBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t);
    virtual void setProgramUniformBlockSlot(std::size_t, std::string, int);
    virtual int  getProgramUniformBlockSlot(std::size_t, std::string);
    
    virtual void setProgramTextureSlot(std::size_t, std::string, int);
    virtual void setProgramUniformData(std::size_t, std::string, uint32_t);
    virtual void setProgramUniformData(std::size_t, std::string, std::vector<uint32_t>);
    virtual int  getProgramUniformSlot(std::size_t, std::string);
    
    virtual void setUniformBlockState(std::size_t, std::size_t, int, std::size_t, std::size_t);
    virtual void deleteUniformBuffer(std::size_t);
    
    virtual void setVertexLayoutFormat(std::size_t, std::vector<NRE_GPU_VertexLayoutInput>);
    virtual void deleteVertexLayout(std::size_t);
    
    virtual void setTextureFormat(std::size_t, NRE_GPU_TEXTURE_TYPE, NRE_GPU_TEXTURE_FILTER, uint32_t, uint32_t, int);
    virtual void setFrameBufferTexture(std::size_t, std::size_t, int);
    virtual void setTextureSlot(std::size_t, int);
    virtual void deleteTexture(std::size_t);
    
    virtual void copyFrameBuffer(std::size_t, std::size_t);
    virtual void useFrameBuffer(std::size_t);
    virtual void clearFrameBuffer(std::size_t);
    virtual void deleteFrameBuffer(std::size_t);
    
    virtual void setProgramVS(std::size_t, NRE_GPU_VertexShader);
    virtual void setProgramFS(std::size_t, NRE_GPU_PixelShader);
    
    // WARNING: Do not use the setProgram*S with string argument directly
    //void setProgramGS(std::size_t, FE_GPU_Shader);
    //void setProgramTCS(std::size_t, FE_GPU_Shader);
    //void setProgramTES(std::size_t, FE_GPU_Shader);
    virtual void setupProgram(std::size_t);
    virtual void deleteProgram(std::size_t);
    
    virtual void draw(std::size_t, std::size_t, int, int);
    virtual void draw(std::size_t, std::size_t);
    
    virtual void draw(std::size_t, std::size_t, std::size_t, int, int);
    virtual void draw(std::size_t, std::size_t, std::size_t);
    
    virtual void setRenderMode(NRE_GPU_RENDERMODE);
    
    std::atomic<bool>           use_wireframe{false};
    
    uint32_t x{0};
    uint32_t y{0};
};

#endif 
