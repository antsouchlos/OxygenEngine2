#ifndef NRE_GPU_API_H_INCLUDED
#define NRE_GPU_API_H_INCLUDED

#include <types/OE_TypesBase.h>
#include <OE_Math.h>


/** platform-specific namespaces
  * -OpenGL ES 3.0/ OpenGL 3.2
  * -OpenGL 4.x
  * -OpenGL 4.5 + GL_ARB_bindless_texture
  * -Direct3D 9 (maybe, 5% chance to happen)
  */

struct NRE_GPU_VertexLayoutInput{

    size_t offset;
    unsigned int amount;
    unsigned int stride;
};

enum NRE_GPU_BUFFER_USAGE{
    NRE_GPU_DYNAMIC,
    NRE_GPU_STATIC,
    NRE_GPU_STREAM
};


/** NRE_GPU_API provides a platform-independent
  * interface for accessing the GPU on differing
  * systems for the renderer and conserves precious
  * development time.
  */

class NRE_GPU_API : public OE_THREAD_SAFETY_OBJECT{
public:
    NRE_GPU_API();
    ~NRE_GPU_API();
    
    virtual std::size_t newVertexBuffer();
    virtual std::size_t newVertexLayout();
    virtual std::size_t newIndexBuffer();
    virtual std::size_t newProgram();
    virtual std::size_t newUniformBuffer();
    
    virtual void setVertexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    virtual void setVertexBufferData(std::size_t, const std::vector<float>&, std::size_t);
    virtual void deleteVertexBuffer(std::size_t);
    
    virtual void setIndexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    virtual void setIndexBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t);
    virtual void deleteIndexBuffer(std::size_t);
    
    virtual void setUniformBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    virtual void setUniformBufferData(std::size_t, const std::vector<float>&, std::size_t);
    virtual void setUniformBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t);
    virtual void setProgramUniformSlot(std::size_t, std::size_t, int);
    virtual void setUniformState(std::size_t, int, uint32_t, uint32_t);
    virtual void deleteUniformBuffer(std::size_t);
    
    virtual void setVertexLayoutFormat(std::size_t, std::vector<NRE_GPU_VertexLayoutInput>);
    virtual void deleteVertexLayout(std::size_t);
    
    virtual void setProgramVS(std::size_t, std::string);
    //void setProgramGS(std::size_t, FE_GPU_Shader);
    virtual void setProgramFS(std::size_t, std::string);
    //void setProgramTCS(std::size_t, FE_GPU_Shader);
    //void setProgramTES(std::size_t, FE_GPU_Shader);
    virtual void setupProgram(std::size_t);
    
    virtual void draw(std::size_t, std::size_t, int, int);
    virtual void draw(std::size_t, std::size_t);
    
    virtual void draw(std::size_t, std::size_t, std::size_t, int, int);
    virtual void draw(std::size_t, std::size_t, std::size_t);
};

#endif 