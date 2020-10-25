#ifndef NRE_GL3_API_H_INCLUDED
#define NRE_GL3_API_H_INCLUDED


#include <Renderer/NRE_GPU_API.h>
#include <Renderer/glad.h>

struct NRE_GL3_VertexBuffer{
    GLuint                  handle;
    std::size_t             size;
    NRE_GPU_BUFFER_USAGE    usage;
};

struct NRE_GL3_IndexBuffer{
    GLuint                  handle;
    std::size_t             size;
    NRE_GPU_BUFFER_USAGE    usage;
};

struct NRE_GL3_VertexArray{
    GLuint                                  handle;
    std::vector<NRE_GPU_VertexLayoutInput>  layout;
};

struct NRE_GL3_UniformBuffer{
    GLuint                  handle;
    std::size_t             size;
    NRE_GPU_BUFFER_USAGE    usage;
    GLint                   slot;
};

struct NRE_GL3_ProgramUniformState{
    std::string name;
    GLint slot;
};

struct NRE_GL3_Program{
    std::string vs;
    GLuint vs_handle{0};
    bool vs_setup{false};
    std::string fs;
    GLuint fs_handle{0};
    bool fs_setup{false};
    
    bool setup{false};
    bool prog_created{false};
    GLuint handle{0};
    
    std::vector<NRE_GL3_ProgramUniformState> uniforms;
    
    std::size_t hasUniform(std::string);
};

GLenum NRE2GL_BufferUse(NRE_GPU_BUFFER_USAGE);

class NRE_GL3_API : public NRE_GPU_API{
public:
    NRE_GL3_API();
    ~NRE_GL3_API();
    std::string getRenderingAPI();
    
    std::size_t newVertexBuffer();
    std::size_t newVertexLayout();
    std::size_t newIndexBuffer();
    std::size_t newProgram();
    std::size_t newUniformBuffer();
    
    void setVertexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    void setVertexBufferData(std::size_t, const std::vector<float>&, std::size_t);
    void deleteVertexBuffer(std::size_t);
    
    void setIndexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    void setIndexBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t);
    void deleteIndexBuffer(std::size_t);
    
    void setUniformBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    void setUniformBufferData(std::size_t, const std::vector<float>&, std::size_t);
    void setUniformBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t);
    void setProgramUniformSlot(std::size_t, std::string, int);
    void setUniformState(std::size_t, int, std::size_t, std::size_t);
    void deleteUniformBuffer(std::size_t);
    
    void setVertexLayoutFormat(std::size_t, std::vector<NRE_GPU_VertexLayoutInput>);
    void deleteVertexLayout(std::size_t);
    
    void setProgramVS(std::size_t, std::string);
    //void setProgramGS(std::size_t, FE_GPU_Shader);
    void setProgramFS(std::size_t, std::string);
    //void setProgramTCS(std::size_t, FE_GPU_Shader);
    //void setProgramTES(std::size_t, FE_GPU_Shader);
    void setupProgram(std::size_t);
    void deleteProgram(std::size_t);
    
    void draw(std::size_t, std::size_t, int, int);
    void draw(std::size_t, std::size_t);
    
    void draw(std::size_t, std::size_t, std::size_t, int, int);
    void draw(std::size_t, std::size_t, std::size_t);
    
protected:
    
    std::size_t cur_vbo{0};
    std::size_t cur_ibo{0};
    std::size_t cur_vao{0};
    std::size_t cur_ubo{0};
    std::size_t cur_prog{0};
    
    std::unordered_map<std::size_t, NRE_GL3_VertexBuffer>     vbos;
    std::unordered_map<std::size_t, NRE_GL3_IndexBuffer>      ibos;
    std::unordered_map<std::size_t, NRE_GL3_VertexArray>      vaos;
    std::unordered_map<std::size_t, NRE_GL3_UniformBuffer>    ubos;
    std::unordered_map<std::size_t, NRE_GL3_Program>          progs;
    
    std::size_t getVAOSize(std::size_t);
};











#endif 
