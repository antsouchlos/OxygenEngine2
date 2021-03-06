#ifndef NRE_GL3_API_H_INCLUDED
#define NRE_GL3_API_H_INCLUDED


#include <Renderer/NRE_GPU_API.h>
#include <Renderer/glad.h>
#include <Renderer/GL3/NRE_GL3_Shaders.h>

struct NRE_GL3_RenderBuffer{
    GLuint handle{0};
    NRE_GPU_TEXTURE_TYPE type;
    int x{0};
    int y{0};
    
    bool hasNotChanged(NRE_GPU_TEXTURE_TYPE, int, int);
};

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

struct NRE_GL3_FrameBuffer{
    GLuint handle;
    //int components{0};
    //bool depth{false};
    //bool stencil{false};
    std::size_t texture{0};
};

struct NRE_GL3_Texture{
    GLuint handle;
    NRE_GPU_TEXTURE_TYPE type;
    NRE_GPU_TEXTURE_FILTER filter;
    int x{0};
    int y{0};
    int mipmaps{0};
    
    bool hasNotChanged(NRE_GPU_TEXTURE_TYPE, NRE_GPU_TEXTURE_FILTER, int, int, int);
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
    GLint slot{0};
    GLenum type{GL_FLOAT}; //unused in Uniform blocks
    size_t size{0}; //unused in Uniform Blocks
};

struct NRE_GL3_Program{
    
    NRE_GPU_VertexShader vs;
    GLuint vs_handle{0};
    bool vs_setup{false};
    
    NRE_GPU_PixelShader fs;
    GLuint fs_handle{0};
    bool fs_setup{false};
    
    bool setup{false};
    bool prog_created{false};
    GLuint handle{0};
    
    //std::vector<NRE_GL3_ProgramUniformState> uniform_blocks;
    //std::size_t hasUniformBlock(std::string);
    
    //std::vector<NRE_GL3_ProgramUniformState> uniforms;
    //std::size_t hasUniform(std::string);
    
    // this is needed for it to be in an std::set
    bool operator< (const NRE_GL3_Program&) const;
};

struct NRE_GL3_ProgramData{
    GLuint handle{0};
    std::vector<NRE_GL3_ProgramUniformState> uniform_blocks;
    std::size_t hasUniformBlock(std::string);
    
    std::vector<NRE_GL3_ProgramUniformState> uniforms;
    std::size_t hasUniform(std::string);
};

GLenum NRE2GL_BufferUse(NRE_GPU_BUFFER_USAGE);

class NRE_GL3_API : public NRE_GPU_API{
public:
    
    
    
    NRE_GL3_API();
    ~NRE_GL3_API();
    
    void update(uint32_t, uint32_t);
    
    void destroy();
    
    std::string getRenderingAPI();
    
    std::size_t newVertexBuffer();
    std::size_t newVertexLayout();
    std::size_t newIndexBuffer();
    std::size_t newProgram();
    std::size_t newUniformBuffer();
    std::size_t newFrameBuffer();
    std::size_t newTexture();
    std::size_t newRenderBuffer();
    
    void setRenderBufferType(std::size_t, NRE_GPU_TEXTURE_TYPE, int, int);
    void setFrameBufferRenderBuffer(std::size_t, std::size_t, int);
    
    void setVertexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    void setVertexBufferData(std::size_t, const std::vector<float>&, std::size_t);
    void setVertexBufferMemoryData(std::size_t, const std::vector<float>&, NRE_GPU_BUFFER_USAGE);
    void deleteVertexBuffer(std::size_t);
    
    void setIndexBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    void setIndexBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t);
    void setIndexBufferMemoryData(std::size_t, const std::vector<uint32_t>&, NRE_GPU_BUFFER_USAGE);
    void deleteIndexBuffer(std::size_t);
    
    void setUniformBufferMemory(std::size_t, std::size_t, NRE_GPU_BUFFER_USAGE);
    void setUniformBufferData(std::size_t, const std::vector<float>&, std::size_t);
    void setUniformBufferData(std::size_t, const std::vector<uint32_t>&, std::size_t);
    
    void setProgramUniformBlockSlot(std::size_t, std::string, int);
    int  getProgramUniformBlockSlot(std::size_t, std::string);
    
    void setProgramTextureSlot(std::size_t, std::string, int);
    void setProgramUniformData(std::size_t, std::string, uint32_t);
    void setProgramUniformData(std::size_t, std::string, std::vector<uint32_t>);
    int  getProgramUniformSlot(std::size_t, std::string);
    
    void setUniformBlockState(std::size_t, std::size_t, int, std::size_t, std::size_t);
    void deleteUniformBuffer(std::size_t);
    
    void setVertexLayoutFormat(std::size_t, std::vector<NRE_GPU_VertexLayoutInput>);
    void deleteVertexLayout(std::size_t);
    
    void setTextureFormat(std::size_t, NRE_GPU_TEXTURE_TYPE, NRE_GPU_TEXTURE_FILTER, uint32_t, uint32_t, int);
    void setFrameBufferTexture(std::size_t, std::size_t, int);
    void setTextureSlot(std::size_t, int);
    void deleteTexture(std::size_t);
    
    void copyFrameBuffer(std::size_t, std::size_t, NRE_GPU_FRAMEBUFFER_COPY);
    void useFrameBuffer(std::size_t);
    void clearFrameBuffer(std::size_t, NRE_GPU_FRAMEBUFFER_COPY, float);
    void deleteFrameBuffer(std::size_t);
    
    void setProgramVS(std::size_t, NRE_GPU_VertexShader);
    void setProgramFS(std::size_t, NRE_GPU_PixelShader);
    
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
    
    void draw_instanced(std::size_t, std::size_t, std::size_t);
    void draw_instanced(std::size_t, std::size_t, std::size_t, std::size_t);
    
    void setRenderMode(NRE_GPU_RENDERMODE);
    
protected:
    
    std::size_t cur_rbo{0};
    std::size_t cur_vbo{0};
    std::size_t cur_ibo{0};
    std::size_t cur_vao{0};
    std::size_t cur_ubo{0};
    std::size_t cur_fbo{0};
    std::size_t cur_texture{0};
    std::size_t cur_prog{0};
    
    std::unordered_map<std::size_t, NRE_GL3_RenderBuffer>     rbos;
    std::unordered_map<std::size_t, NRE_GL3_VertexBuffer>     vbos;
    std::unordered_map<std::size_t, NRE_GL3_IndexBuffer>      ibos;
    std::unordered_map<std::size_t, NRE_GL3_VertexArray>      vaos;
    std::unordered_map<std::size_t, NRE_GL3_UniformBuffer>    ubos;
    std::unordered_map<std::size_t, NRE_GL3_FrameBuffer>      fbos;
    std::unordered_map<std::size_t, NRE_GL3_Texture>          textures;
    std::unordered_map<std::size_t, NRE_GL3_Program>          progs;
    
    std::size_t getVAOSize(std::size_t);
    
    std::map<NRE_GPU_VertexShader,  GLuint> vs_db;
    std::map<NRE_GPU_PixelShader,   GLuint> fs_db;
    std::map<NRE_GL3_Program,       NRE_GL3_ProgramData> prog_db;
    
private:
    
    void check_rbo_id_(std::size_t, const std::string&);
    void check_vbo_id_(std::size_t, const std::string&);
    void check_ubo_id_(std::size_t, const std::string&);
    void check_ibo_id_(std::size_t, const std::string&);
    
    void check_vbo_offset_length_(std::size_t, std::size_t, const std::string&);
    void check_ubo_offset_length_(std::size_t, std::size_t, const std::string&);
    void check_ibo_offset_length_(std::size_t, std::size_t, const std::string&);
    
    void check_vao_id_(std::size_t, const std::string&);
    void check_prog_id_(std::size_t, const std::string&);
    
    void check_prog_uniform_block_(std::size_t, const std::string&, const std::string&);
    void check_prog_uniform_(std::size_t, const std::string&, const std::string&);
    void check_vao_vbo_(std::size_t, std::size_t, const std::string&);
    
    void check_fbo_id_(std::size_t, const std::string&);
    void check_texture_id_(std::size_t, const std::string&);
    
    void get_program_all_uniforms_(std::size_t);
    
    int teximage_internalformat_(NRE_GPU_TEXTURE_TYPE);
    int teximage_format_(NRE_GPU_TEXTURE_TYPE);
    int teximage_type_(NRE_GPU_TEXTURE_TYPE);
    
    // this is useful for preventing OpenGL glBind* command repetitions
    GLuint active_vbo_{0};
    GLuint active_vao_{0};
    GLuint active_ubo_{0};
    GLuint active_prog_{0};
    // every VAO in OpenGL stores its Index Buffer
    std::unordered_map<GLuint, GLuint> vao_ibos_;
    
    
    
};











#endif 
