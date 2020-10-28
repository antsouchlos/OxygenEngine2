#include <Renderer/NRE_GL3_API.h>

#define NRE_GL3_VERTEXL_LAYOUT_OFFSET(i) ((GLvoid*)(i))

using namespace std;

// small utility function to translate the buffer usages to something opengl understands
// This should be different on other APIs
GLenum NRE2GL_BufferUse(NRE_GPU_BUFFER_USAGE usage){
    GLenum buf_usage;
    switch(usage){
        case NRE_GPU_STATIC: buf_usage = GL_STATIC_DRAW; break;
        case NRE_GPU_DYNAMIC: buf_usage = GL_DYNAMIC_DRAW; break;
        case NRE_GPU_STREAM: buf_usage = GL_STREAM_DRAW; break;
        default: buf_usage = GL_STATIC_DRAW;
    }
    return buf_usage;
}

// get index of a uniform variable in a shader program
std::size_t NRE_GL3_Program::hasUniform(std::string name){
    size_t index = 0;
    for (auto x: this->uniforms){
        if (x.name == name){
            return index; 
        }
        index++;
    }
    return this->uniforms.size();
}

std::size_t NRE_GL3_API::getVAOSize(std::size_t id){
    assert (this->vaos.count(id) != 0);
    assert (this->vaos[id].layout.size() > 0);
    return this->vbos[this->vaos[id].layout[0].vertex_buffer].size;
}


NRE_GL3_API::NRE_GL3_API(){
    
}

NRE_GL3_API::~NRE_GL3_API(){
    for (auto x: std::exchange(vbos, {}))
        glDeleteBuffers(1, &x.second.handle);
    for (auto x: std::exchange(vaos, {}))
        glDeleteVertexArrays(1, &x.second.handle);
    for (auto x: std::exchange(ibos, {}))
        glDeleteBuffers(1, &x.second.handle);
    for (auto x: std::exchange(ubos, {}))
        glDeleteBuffers(1, &x.second.handle);
    for (auto x: std::exchange(progs, {})){
        glDeleteShader(x.second.vs_handle);
        glDeleteShader(x.second.fs_handle);
        glDeleteProgram(x.second.handle);
    }
}

std::string NRE_GL3_API::getRenderingAPI(){
    return "OpenGL";
}

//---------------------Create Objects-----------------------------//

std::size_t NRE_GL3_API::newVertexBuffer(){ 
    cur_vbo++;
    this->vbos[cur_vbo] = NRE_GL3_VertexBuffer();
    glGenBuffers(1, &vbos[cur_vbo].handle);
    return cur_vbo;
     
}
std::size_t NRE_GL3_API::newVertexLayout(){  
    cur_vao++;
    this->vaos[cur_vao] = NRE_GL3_VertexArray();
    glGenVertexArrays(1, &vaos[cur_vao].handle);
    return cur_vao;
}
std::size_t NRE_GL3_API::newIndexBuffer(){
    cur_ibo++;
    this->ibos[cur_ibo] = NRE_GL3_IndexBuffer();
    glGenBuffers(1, &ibos[cur_ibo].handle);
    return cur_ibo;
}
std::size_t NRE_GL3_API::newProgram(){
    cur_prog++;
    this->progs[cur_prog] = NRE_GL3_Program();
    this->progs[cur_prog].handle = glCreateProgram();
    return cur_prog;
}

std::size_t NRE_GL3_API::newUniformBuffer(){ 
    cur_ubo++;
    this->ubos[cur_ubo] = NRE_GL3_UniformBuffer();
    glGenBuffers(1, &ubos[cur_ubo].handle);
    return cur_ubo;
}

//---------------------Vertex Buffer-----------------------------//

void NRE_GL3_API::setVertexBufferMemory(std::size_t id, std::size_t memory_size, NRE_GPU_BUFFER_USAGE buf_usage){
    
    assert (this->vbos.count(id) != 0);
    this->vbos[id].size = memory_size;
    this->vbos[id].usage = buf_usage;
    glBindBuffer(GL_ARRAY_BUFFER, this->vbos[id].handle);
    glBufferData(GL_ARRAY_BUFFER, memory_size*sizeof(float), NULL, NRE2GL_BufferUse(buf_usage));
}
void NRE_GL3_API::setVertexBufferData(std::size_t id, const std::vector<float>& v, std::size_t offset){
    
    assert (this->vbos.count(id) != 0);
    assert ((offset + v.size() <= vbos[id].size));
    glBindBuffer(GL_ARRAY_BUFFER, this->vbos[id].handle);
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLuint>(offset)*sizeof(float), v.size()*sizeof(float), &v[0]);
}

void NRE_GL3_API::deleteVertexBuffer(std::size_t id){
    assert (this->vbos.count(id) != 0);
    glDeleteBuffers(1, &this->vbos[id].handle);
    this->vbos.erase(id);
}

//--------------------Index Buffer-------------------------------//

void NRE_GL3_API::setIndexBufferMemory(std::size_t id, std::size_t memory_size, NRE_GPU_BUFFER_USAGE buf_usage){
    
    assert (this->ibos.count(id) != 0);
    this->ibos[id].size = memory_size;
    this->ibos[id].usage = buf_usage;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibos[id].handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, memory_size*sizeof(uint32_t), NULL, NRE2GL_BufferUse(buf_usage));
}

void NRE_GL3_API::setIndexBufferData(std::size_t id, const std::vector<uint32_t>& v, std::size_t offset){
    
    assert (this->ibos.count(id) != 0);
    assert ((offset + v.size() <= ibos[id].size));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibos[id].handle);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(offset)*sizeof(uint32_t), v.size()*sizeof(uint32_t), &v[0]);
}

void NRE_GL3_API::deleteIndexBuffer(std::size_t id){
    assert (this->ibos.count(id) != 0);
    glDeleteBuffers(1, &this->ibos[id].handle);
    this->ibos.erase(id);
}

//--------------------Uniform Buffer-----------------------------//

void NRE_GL3_API::setUniformBufferMemory(std::size_t id, std::size_t memory_size, NRE_GPU_BUFFER_USAGE buf_usage){
    assert (this->ubos.count(id) != 0);
    this->ubos[id].size = memory_size;
    this->ubos[id].usage = buf_usage;
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubos[id].handle);
    glBufferData(GL_UNIFORM_BUFFER, memory_size*sizeof(float), NULL, NRE2GL_BufferUse(buf_usage));
}

void NRE_GL3_API::setUniformBufferData(std::size_t id, const std::vector<float>& v, std::size_t offset){
    assert (this->ubos.count(id) != 0);
    assert ((offset + v.size() <= ubos[id].size));
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubos[id].handle);
    glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLuint>(offset)*sizeof(float), v.size()*sizeof(float), &v[0]);
}

void NRE_GL3_API::setUniformBufferData(std::size_t id, const std::vector<uint32_t>& v, std::size_t offset){
    assert (this->ubos.count(id) != 0);
    assert ((offset + v.size() <= ubos[id].size));
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubos[id].handle);
    glBufferSubData(GL_UNIFORM_BUFFER, static_cast<GLuint>(offset)*sizeof(uint32_t), v.size()*sizeof(uint32_t), &v[0]);
}

void NRE_GL3_API::setProgramUniformSlot(std::size_t id, std::string name, int slot){
    
    assert (this->progs.count(id) != 0);
    assert (this->progs[id].hasUniform(name) != this->progs[id].uniforms.size());
    
    this->progs[id].uniforms[this->progs[id].hasUniform(name)].slot = slot;
    
    glUniformBlockBinding(this->progs[id].handle, this->progs[id].hasUniform(name), slot);
}

void NRE_GL3_API::setUniformState(std::size_t id, int slot, std::size_t offset, std::size_t length){
    
    assert (this->ubos.count(id) != 0);
    assert ((offset + length <= ubos[id].size));
    if(length == 0)
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, this->ubos[id].handle);
    else
        glBindBufferRange(GL_UNIFORM_BUFFER, slot, this->ubos[id].handle, static_cast<GLuint>(offset), static_cast<GLuint>(length));
}

void NRE_GL3_API::deleteUniformBuffer(std::size_t id){
    
    assert (this->ubos.count(id) != 0);
    glDeleteBuffers(1, &this->ubos[id].handle);
    this->ubos.erase(id);
}

//---------------------Vertex Layout-----------------------------//

void NRE_GL3_API::setVertexLayoutFormat(std::size_t id, std::vector<NRE_GPU_VertexLayoutInput> inputs){ 
    assert (this->vaos.count(id) != 0);
    this->vaos[id].layout = inputs;
    for(size_t x=0; x < inputs.size(); x++){

        assert (this->vbos.count(inputs[x].vertex_buffer) != 0);
        glBindBuffer(GL_ARRAY_BUFFER, this->vbos[inputs[x].vertex_buffer].handle);
        glVertexAttribPointer(x, inputs[x].amount, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*inputs[x].stride, NRE_GL3_VERTEXL_LAYOUT_OFFSET(sizeof(GLfloat)*static_cast<GLuint>(inputs[x].offset)));
        glEnableVertexAttribArray(x);
    }
}

void NRE_GL3_API::deleteVertexLayout(std::size_t id){
    assert (this->vaos.count(id) != 0);
    glDeleteVertexArrays(1, &this->vaos[id].handle);
    this->vaos.erase(id);
}

//---------------------Shader Programs-----------------------------//

void NRE_GL3_API::setProgramVS(std::size_t id, std::string data){
    
    assert (this->progs.count(id) != 0);
    GLuint shader_id;
    if(! this->progs[id].vs_setup)
        shader_id = glCreateShader(GL_VERTEX_SHADER);
    else
        shader_id = this->progs[id].vs_handle;
    
    this->progs[id].vs_setup = true;
    this->progs[id].vs_handle = shader_id;
    this->progs[id].setup = false;
    
    /// compile and attach shader
    const char* c_str = data.c_str();
    glShaderSource(shader_id, 1, &c_str, NULL);
    glCompileShader(shader_id);

    /// IMPORTANT: get shader compiler info
    int parameters = -1;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &parameters);
    if(parameters != GL_TRUE){

        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetShaderInfoLog(shader_id, max_length, &actual_length, log);
        OE_WriteToLog(log);
    }
}

//void setProgramGS(std::size_t, FE_GPU_Shader){}
void NRE_GL3_API::setProgramFS(std::size_t id, std::string data){
    
    assert (this->progs.count(id) != 0);
    GLuint shader_id;
    if(! this->progs[id].fs_setup)
        shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    else
        shader_id = this->progs[id].fs_handle;
    
    this->progs[id].fs_setup = true;
    this->progs[id].fs_handle = shader_id;
    this->progs[id].setup = false;
    
    /// compile and attach shader
    const char* c_str = data.c_str();
    glShaderSource(shader_id, 1, &c_str, NULL);
    glCompileShader(shader_id);

    /// IMPORTANT: get shader compiler info
    int parameters = -1;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &parameters);
    if(parameters != GL_TRUE){

        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetShaderInfoLog(shader_id, max_length, &actual_length, log);
        OE_WriteToLog(log);
    }
}

//void setProgramTCS(std::size_t, FE_GPU_Shader){}
//void setProgramTES(std::size_t, FE_GPU_Shader){}

void NRE_GL3_API::setupProgram(std::size_t id){
    
    assert (this->progs.count(id) != 0);
    
    // ignore if the program is already setup
    if (this->progs[id].setup) return;
    this->progs[id].setup = true;
    
    assert (this->progs[id].vs_setup && this->progs[id].fs_setup);
    
    glAttachShader(this->progs[id].handle, this->progs[id].vs_handle);
    glAttachShader(this->progs[id].handle, this->progs[id].fs_handle);

    //glBindAttribLocation(this->progs[id].handle, 0, "oe_position");
    //glBindAttribLocation(this->progs[id].handle, 1, "oe_normals");
    //glBindAttribLocation(this->progs[id].handle, 2, "oe_uvs");

    glLinkProgram(this->progs[id].handle);


    /// IMPORTANT: check if program is linked correctly
    int parameters = -1;
    glGetProgramiv(this->progs[id].handle, GL_LINK_STATUS, &parameters);
    if(GL_TRUE != parameters){

        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetProgramInfoLog(this->progs[id].handle, max_length, &actual_length, log);
        cout << log << endl;
        OE_WriteToLog(log);
    }

    /// IMPORTANT: make sure program is runnable
    glValidateProgram(this->progs[id].handle);
    parameters = -1;
    glGetProgramiv(this->progs[id].handle, GL_VALIDATE_STATUS, &parameters);
    if(GL_TRUE != parameters){

        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetProgramInfoLog(this->progs[id].handle, max_length, &actual_length, log);
        cout << log << endl;
        OE_WriteToLog(log);

    }
    glUseProgram(this->progs[id].handle);

    /// get all active uniform blocks
    GLint numBlocks=0;
    glGetProgramiv(this->progs[id].handle, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
    for(int ida=0; ida< numBlocks; ida++){

        GLint name_length=0;

        glGetActiveUniformBlockiv(this->progs[id].handle, ida, GL_UNIFORM_BLOCK_NAME_LENGTH, &name_length);

        GLchar ubo_name[name_length];
        glGetActiveUniformBlockName(this->progs[id].handle, ida, name_length, NULL, &ubo_name[0]);

        string actual_name = string(ubo_name);
        OE_WriteToLog(actual_name);
        auto ubo_state = NRE_GL3_ProgramUniformState();
        ubo_state.name = actual_name;
        ubo_state.slot = -1;
        this->progs[id].uniforms.push_back(ubo_state);
    }
    
}

void NRE_GL3_API::deleteProgram(std::size_t id){
    assert (this->progs.count(id) != 0);
    glDeleteShader(this->progs[id].vs_handle);
    glDeleteShader(this->progs[id].fs_handle);
    glDeleteProgram(this->progs[id].handle);
    this->progs.erase(id);
}

//---------------------Draw calls-----------------------------//

void NRE_GL3_API::draw(std::size_t prog_id, std::size_t vao_id, int offset, int count){
    
    assert (this->progs.count(prog_id) != 0);
    assert (this->vaos.count(vao_id) != 0);
    this->setupProgram(prog_id);
    glUseProgram(this->progs[prog_id].handle);
    glBindVertexArray(this->vaos[vao_id].handle);
    glDrawArrays(GL_TRIANGLES, offset, count);
}

void NRE_GL3_API::draw(std::size_t prog_id, std::size_t vao_id){
    assert (this->progs.count(prog_id) != 0);
    assert (this->vaos.count(vao_id) != 0);
    this->setupProgram(prog_id);
    glUseProgram(this->progs[prog_id].handle);
    glBindVertexArray(this->vaos[vao_id].handle);
    glDrawArrays(GL_TRIANGLES, 0, this->getVAOSize(vao_id));
}
    
void NRE_GL3_API::draw(std::size_t prog_id, std::size_t vao_id, std::size_t ibo_id, int offset, int count){
    assert (this->progs.count(prog_id) != 0);
    assert (this->vaos.count(vao_id) != 0);
    assert (this->ibos.count(ibo_id) != 0);
    this->setupProgram(prog_id);
    glUseProgram(this->progs[prog_id].handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibos[ibo_id].handle);
    glBindVertexArray(this->vaos[vao_id].handle);
    glDrawRangeElements(GL_TRIANGLES, offset, count, 1, GL_UNSIGNED_INT, (GLvoid*)NULL);
}

void NRE_GL3_API::draw(std::size_t prog_id, std::size_t vao_id, std::size_t ibo_id){
    assert (this->progs.count(prog_id) != 0);
    assert (this->vaos.count(vao_id) != 0);
    assert (this->ibos.count(ibo_id) != 0);
    this->setupProgram(prog_id);
    glUseProgram(this->progs[prog_id].handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibos[ibo_id].handle);
    glBindVertexArray(this->vaos[vao_id].handle);
    glDrawElements(GL_TRIANGLES, 1, GL_UNSIGNED_INT, (GLvoid*)NULL);
}
