#ifndef NRE_GPU_API_H_INCLUDED
#define NRE_GPU_API_H_INCLUDED

#include <types/OE_TypesBase.h>
#include <OE_Math.h>
#include <Renderer/NRE_GPU_Shaders.h>

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
    NRE_GPU_TRANSPARENT_BACKFACE
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
    
    virtual void destroy();
    
    virtual std::string getRenderingAPI();
    
    virtual std::size_t newVertexBuffer();
    virtual std::size_t newVertexLayout();
    virtual std::size_t newIndexBuffer();
    virtual std::size_t newProgram();
    virtual std::size_t newUniformBuffer();
    
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
    virtual void setProgramUniformSlot(std::size_t, std::string, int);
    virtual void setUniformState(std::size_t, std::size_t, int, std::size_t, std::size_t);
    virtual void deleteUniformBuffer(std::size_t);
    
    virtual void setVertexLayoutFormat(std::size_t, std::vector<NRE_GPU_VertexLayoutInput>);
    virtual void deleteVertexLayout(std::size_t);
    
    
    virtual void setProgramVS(std::size_t, NRE_GPU_VertexShader);
    virtual void setProgramFS(std::size_t, NRE_GPU_PixelShader);
    
    // WARNING: Do not use the setProgram*S with string argument directly
    virtual void setProgramVS(std::size_t, std::string);
    //void setProgramGS(std::size_t, FE_GPU_Shader);
    virtual void setProgramFS(std::size_t, std::string);
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
};

#endif 
