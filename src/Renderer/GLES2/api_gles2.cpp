#include <OE/Renderer/GLES2/api_gles2.h>
#include <OE/Renderer/api_gpu.h>
#include <OE/types/libs_oe.h>
#include <iterator>

#define NRE_GLES2_VERTEXL_LAYOUT_OFFSET(i) ((GLvoid*)(i))

using namespace std;

// small utility function to translate the buffer usages to something opengl understands
// This should be different on other APIs
GLenum nre::gles2::buffer_use(nre::gpu::BUFFER_USAGE usage) {
    GLenum buf_usage;
    switch (usage) {
    case nre::gpu::STATIC:
        buf_usage = GL_STATIC_DRAW;
        break;
    case nre::gpu::DYNAMIC:
        buf_usage = GL_DYNAMIC_DRAW;
        break;
    case nre::gpu::STREAM:
        buf_usage = GL_STREAM_DRAW;
        break;
    default:
        buf_usage = GL_STATIC_DRAW;
    }
    return buf_usage;
}

// get index of a uniform variable in a shader program
bool nre::gles2::program_data_t::has_uniform(std::string_view name) {
    return this->uniforms.contains(name);
}

bool nre::gles2::program_t::operator==(const nre::gles2::program_t& other) const {
    return std::tie(this->vs, this->fs) == std::tie(other.vs, other.fs);
}
size_t nre::gles2::program_t::gen_hash() const {
    return this->vs.gen_hash() + this->fs.gen_hash();
}

bool nre::gles2::texture_t::has_not_changed(nre::gpu::TEXTURE_TYPE type_in, nre::gpu::TEXTURE_FILTER filter_in, int x_in,
                                            int y_in, int mipmaps_in) {
    return std::tie(this->type, this->filter, this->x, this->y, this->mipmaps) ==
           std::tie(type_in, filter_in, x_in, y_in, mipmaps_in);
}

bool nre::gles2::renderbuffer_t::has_not_changed(nre::gpu::TEXTURE_TYPE type_in, int x_in, int y_in) {
    return std::tie(this->type, this->x, this->y) == std::tie(type_in, x_in, y_in);
}

// ------------------------ API ---------------------- //

std::size_t nre::gles2::api_t::get_vao_size(std::size_t id) {
    this->check_vao_id(id, "get_vao_size");
    return this->vbos_[this->vaos_[id].layout[0].vertex_buffer].size / this->vaos_[id].layout[0].stride;
}


nre::gles2::api_t::api_t(nre::gpu::info_struct& backend_info) {
    this->vao_ibos_[0] = 0;
    major_             = backend_info.major;
    minor_             = backend_info.minor;

    // TODO: assume no extensions by default
    backend_info.has_indexed_ranged_draws = true;
    backend_info.has_uniform_buffers      = true;
    backend_info.has_occlusion_query      = true;
    backend_info.has_cubemap_array        = true;
    backend_info.has_shadow_sampler       = true;
    backend_info.has_instancing           = true;
    backend_info.has_depth_textures       = true;
    backend_info.has_float_textures       = true;
    backend_info.has_float_render_targets = true;
    backend_info.has_depth_24             = true;

    // TODO: check for the right extensions
    // glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
    std::string              extensions_gl = std::string((const char*)glGetString(GL_EXTENSIONS));
    std::istringstream       iss(extensions_gl);
    std::vector<std::string> extensions_split(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());


    for (auto extension_es : extensions_split) {

        OE_WriteToLog(extension_es + "\n");

        if (extension_es == "GL_OES_packed_depth_stencil")
            has_oes_packed_depth_stencil_ = true;
        else if (extension_es == "GL_OES_depth24")
            has_oes_depth24_ = true;
        else if ((extension_es == "GL_OES_depth_texture") or (extension_es == "GL_ANGLE_depth_texture"))
            has_oes_depth_texture_ = true;
        else if (extension_es == "GL_EXT_color_buffer_float")
            has_ext_color_buffer_float_ = true;
        else if (extension_es == "GL_EXT_color_buffer_half_float")
            has_ext_color_buffer_half_float_ = true;
        else if (extension_es == "GL_OES_element_index_uint")
            has_oes_element_index_uint_ = true;
        else if (extension_es == "GL_OES_texture_float")
            has_oes_texture_float_ = true;
        else if (extension_es == "GL_EXT_discard_framebuffer")
            has_ext_discard_framebuffer_ = true;
        else
            continue;
    }
#ifdef OE_PLATFORM_WEB
    has_oes_element_index_uint_ = true; // always available on webgl
#endif
}
nre::gles2::api_t::~api_t() {
}

void nre::gles2::api_t::update(uint32_t x_in, uint32_t y_in, bool sanity_checks) {

    sanity_checks_ = sanity_checks;

    this->discard_framebuffer(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (x_in != x_ or y_in != y_) {
        glViewport(0, 0, x_in, y_in);
        x_ = x_in;
        y_ = y_in;
    }
    // cout << this->prog_db_.size() << " " << this->vs_db_.size() << " " << this->fs_db_.size() << endl;
    glDepthMask(GL_TRUE);
    glStencilMask(0xFF);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    this->active_prog_ = 0;
    this->active_vao_  = 0;
    this->active_vbo_  = 0;

    for (auto x : this->vao_ibos_) {
        this->vao_ibos_[x.first] = 0;
    }
    for (auto a_prog : this->prog_db_) {
        this->prog_db_[a_prog.first].checked = false;
        for (auto uniform : a_prog.second.uniforms) {
            this->prog_db_[a_prog.first].uniforms[uniform.first].checked = false;
        }
    }
}

void nre::gles2::api_t::destroy() {
    for (auto x : std::exchange(rbos_, {}))
        glDeleteRenderbuffers(1, &x.second.handle);
    for (auto x : std::exchange(vbos_, {}))
        glDeleteBuffers(1, &x.second.handle);
    for (auto x : std::exchange(ibos_, {}))
        glDeleteBuffers(1, &x.second.handle);
    for (auto x : std::exchange(fbos_, {}))
        glDeleteFramebuffers(1, &x.second.handle);
    for (auto x : std::exchange(textures_, {}))
        glDeleteTextures(1, &x.second.handle);
    for (auto x : std::exchange(prog_db_, {})) {
        glDeleteProgram(x.second.handle);
    }
    for (auto x : std::exchange(vs_db_, {})) {
        glDeleteShader(x.second);
    }
    for (auto x : std::exchange(fs_db_, {})) {
        glDeleteShader(x.second);
    }
}

std::string nre::gles2::api_t::get_rendering_api() {

    return "OpenGL ES 2/WebGL 1.0";
}

//-------------------Handle errors--------------------------------//

void nre::gles2::api_t::check_rbo_id(std::size_t id, std::string_view func) {
    if (not sanity_checks_) return;
    if (this->rbos_.count(id) == 0) {
        throw nre::gpu::invalid_render_buffer(id, func);
    }
}

void nre::gles2::api_t::check_vbo_id(std::size_t id, std::string_view func) {
    if (not sanity_checks_) return;
    if (this->vbos_.count(id) == 0) {
        throw nre::gpu::invalid_vertex_buffer(id, func);
    }
}
void nre::gles2::api_t::check_ibo_id(std::size_t id, std::string_view func) {
    if (not sanity_checks_) return;
    if (this->ibos_.count(id) == 0) {
        throw nre::gpu::invalid_index_buffer(id, func);
    }
}

void nre::gles2::api_t::check_vbo_offset_length(std::size_t id, std::size_t off_len, std::string_view func) {
    if (not sanity_checks_) return;
    if (off_len > this->vbos_[id].size) {
        throw nre::gpu::invalid_buffer_offset_length(id, off_len, "vertex", func);
    }
}

void nre::gles2::api_t::check_ibo_offset_length(std::size_t id, std::size_t off_len, std::string_view func) {
    if (not sanity_checks_) return;
    if (off_len > this->ibos_[id].size) {
        throw nre::gpu::invalid_buffer_offset_length(id, off_len, "index", func);
    }
}

void nre::gles2::api_t::check_vao_id(std::size_t id, std::string_view func) {
    if (not sanity_checks_) return;
    if (this->vaos_.count(id) == 0) {
        throw nre::gpu::invalid_vertex_layout(id, func);
    }
}

void nre::gles2::api_t::check_prog_id(std::size_t id, std::string_view func) {
    if (not sanity_checks_) return;
    if (not this->progs_.contains(id)) {
        throw nre::gpu::invalid_program_id(id, func);
    }
}

void nre::gles2::api_t::check_prog_complete(std::size_t id, std::string_view func) {
    if (not sanity_checks_) return;
    if (not this->progs_[id].setup) {
        throw nre::gpu::incomplete_program(id, func);
    }
}

void nre::gles2::api_t::check_prog_uniform(std::size_t id, std::string_view name, std::string_view func) {
    if (not sanity_checks_) return;
    if (not this->prog_db_[this->progs_[id]].has_uniform(name)) {
        throw nre::gpu::invalid_program_uniform(id, name, func);
    }
}

void nre::gles2::api_t::check_prog_uniform_property(std::size_t id, std::string_view name, std::size_t length,
                                                    std::string_view func, bool is_type_problem) {
    if (not sanity_checks_) return;
    auto uniform_typ = this->prog_db_[this->progs_[id]].uniforms[std::string(name)].type;
    bool is_vec2     = (uniform_typ == GL_FLOAT_VEC2) and (length >= 2);
    bool is_vec3     = (uniform_typ == GL_FLOAT_VEC3) and (length >= 3);
    bool is_vec4     = (uniform_typ == GL_FLOAT_VEC4) and (length >= 4);
    bool is_mat2     = (uniform_typ == GL_FLOAT_MAT2) and (length >= 4);
    bool is_mat3     = (uniform_typ == GL_FLOAT_MAT3) and (length >= 9);
    bool is_mat4     = (uniform_typ == GL_FLOAT_MAT4) and (length >= 16);
    if (not(is_vec2 or is_vec3 or is_vec4 or is_mat2 or is_mat3 or is_mat4)) {
        throw nre::gpu::invalid_uniform_property(id, name, length, func, is_type_problem);
    }
}

void nre::gles2::api_t::check_vao_vbo(std::size_t id, std::size_t vbo_id, std::string_view func) {
    if (not sanity_checks_) return;
    if (this->vbos_.count(vbo_id) == 0) {
        throw nre::gpu::invalid_vertex_layout_buffer(id, vbo_id, func);
    }
}

void nre::gles2::api_t::check_fbo_id(std::size_t id, std::string_view func) {
    if (not sanity_checks_) return;
    if (this->fbos_.count(id) == 0) {
        throw nre::gpu::invalid_framebuffer(id, func);
    }
}

void nre::gles2::api_t::check_texture_id(std::size_t id, std::string_view func) {
    if (not sanity_checks_) return;
    if (this->textures_.count(id) == 0) {
        throw nre::gpu::invalid_texture(id, func);
    }
}

void nre::gles2::api_t::check_draw_range(std::size_t id, std::size_t length, std::size_t offset, std::size_t count,
                                         std::string_view func) {
    if (not sanity_checks_) return;
    if ((offset + count) > length) {
        throw nre::gpu::invalid_draw_range(id, length, offset, count, func);
    }
}

void nre::gles2::api_t::get_program_all_uniforms(std::size_t id) {

    GLint numUniforms = 0;
    glGetProgramiv(this->prog_db_[this->progs_[id]].handle, GL_ACTIVE_UNIFORMS, &numUniforms);
    for (GLint ida = 0; ida < numUniforms; ida++) {

        GLint name_length = 0;

        glGetProgramiv(this->prog_db_[this->progs_[id]].handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &name_length);
        // cout << "uniforms:" << ida << " name length:" << name_length << endl;

        GLchar uniform_name[name_length];
        GLenum var_enum;
        GLint  uniform_size;
        glGetActiveUniform(this->prog_db_[this->progs_[id]].handle, ida, name_length, &name_length, &uniform_size, &var_enum,
                           &uniform_name[0]);

        string actual_name   = string(uniform_name);
        auto   uniform_state = nre::gles2::program_uniform_state_t();
        uniform_state.slot   = ida;
        uniform_state.type   = var_enum;
        uniform_state.size   = uniform_size;
        // cout << uniform_state.name << " " << uniform_state.type << " " << uniform_state.size << endl;
        this->prog_db_[this->progs_[id]].uniforms[actual_name] = uniform_state;
    }
}

int nre::gles2::api_t::teximage_internalformat(nre::gpu::TEXTURE_TYPE type) {
    switch (type) {

    case nre::gpu::FLOAT:
        return GL_RGB32F;
    case nre::gpu::RGBA:
#ifndef OE_PLATFORM_WEB
        return GL_RGBA;
#else
        return GL_RGBA;
#endif
    case nre::gpu::RGB10_A2:
        return GL_RGB10_A2;
    case nre::gpu::RGBA16F:
        return GL_RGBA16F;
    case nre::gpu::SRGBA:
        return GL_SRGB8_ALPHA8;
    case nre::gpu::RGBA_U16:
        return GL_RGBA16UI;
    case nre::gpu::RGBA_U8:
        return GL_RGBA8UI;
    case nre::gpu::DEPTHSTENCIL:
#ifndef OE_PLATFORM_WEB
        return GL_DEPTH24_STENCIL8;
#else
        return GL_DEPTH_STENCIL;
        ;
#endif
    };
    return GL_RGB;
}

int nre::gles2::api_t::teximage_format(nre::gpu::TEXTURE_TYPE type) {
    switch (type) {
    case nre::gpu::FLOAT:
        return GL_RGB32F;
    case nre::gpu::RGBA:
        return GL_RGBA;
    case nre::gpu::RGB10_A2:
        return GL_RGBA;
    case nre::gpu::RGBA16F:
        return GL_RGBA;
    case nre::gpu::SRGBA:
        return GL_RGBA;
    case nre::gpu::RGBA_U16:
        return GL_RGBA_INTEGER;
    case nre::gpu::RGBA_U8:
        return GL_RGBA_INTEGER;
    case nre::gpu::DEPTHSTENCIL:
#ifndef OE_PLATFORM_WEB
        return GL_DEPTH_STENCIL_OES;
#else
        return GL_DEPTH_STENCIL;
#endif
    };
    return GL_RGB;
}

int nre::gles2::api_t::teximage_textype(nre::gpu::TEXTURE_TYPE type) {
    switch (type) {
    case nre::gpu::FLOAT:
        return GL_FLOAT;
    case nre::gpu::RGBA:
        return GL_UNSIGNED_BYTE;
    case nre::gpu::RGB10_A2:
        return GL_UNSIGNED_INT_2_10_10_10_REV;
    case nre::gpu::RGBA16F:
        return GL_FLOAT;
    case nre::gpu::SRGBA:
        return GL_UNSIGNED_BYTE;
    case nre::gpu::RGBA_U16:
        return GL_UNSIGNED_SHORT;
    case nre::gpu::RGBA_U8:
        return GL_UNSIGNED_BYTE;
    case nre::gpu::DEPTHSTENCIL:
#ifndef OE_PLATFORM_WEB
        return GL_UNSIGNED_INT_24_8_OES;
#else
        return GL_RGBA;
#endif
    };
    return GL_UNSIGNED_BYTE;
}



//---------------------Create Objects-----------------------------//

std::size_t nre::gles2::api_t::new_vertex_buffer() {
    cur_vbo_++;
    this->vbos_[cur_vbo_] = nre::gles2::vertex_buffer_t();
    glGenBuffers(1, &vbos_[cur_vbo_].handle);
    return cur_vbo_;
}
std::size_t nre::gles2::api_t::new_vertex_layout() {
    cur_vao_++;
    this->vaos_[cur_vao_]                   = nre::gles2::vertex_array_t();
    this->vao_ibos_[vaos_[cur_vao_].handle] = 0;
    return cur_vao_;
}
std::size_t nre::gles2::api_t::new_index_buffer() {
    cur_ibo_++;
    this->ibos_[cur_ibo_] = nre::gles2::index_buffer_t();
    glGenBuffers(1, &ibos_[cur_ibo_].handle);
    return cur_ibo_;
}
std::size_t nre::gles2::api_t::new_program() {
    cur_prog_++;
    this->progs_[cur_prog_] = nre::gles2::program_t();
    return cur_prog_;
}

std::size_t nre::gles2::api_t::new_framebuffer() {
    cur_fbo_++;
    this->fbos_[cur_fbo_] = nre::gles2::framebuffer_t();
    glGenFramebuffers(1, &fbos_[cur_fbo_].handle);
    return cur_fbo_;
}

std::size_t nre::gles2::api_t::new_texture() {
    cur_texture_++;
    this->textures_[cur_texture_] = nre::gles2::texture_t();
    glGenTextures(1, &textures_[cur_texture_].handle);
    return cur_texture_;
}

std::size_t nre::gles2::api_t::new_renderbuffer() {
    cur_rbo_++;
    this->rbos_[cur_rbo_] = nre::gles2::renderbuffer_t();
    glGenRenderbuffers(1, &rbos_[cur_rbo_].handle);
    return cur_rbo_;
}

//--------------------Render Buffer -------------------------------//

void nre::gles2::api_t::set_renderbuffer_textype(std::size_t id, nre::gpu::TEXTURE_TYPE a_type, int x, int y) {
    this->check_rbo_id(id, "set_renderbuffer_textype");

    if (this->rbos_[id].has_not_changed(a_type, x, y)) {
        return;
    }

    this->rbos_[id].type = a_type;
    this->rbos_[id].x    = a_type;
    this->rbos_[id].y    = a_type;

    glBindRenderbuffer(GL_RENDERBUFFER, this->rbos_[id].handle);
    glRenderbufferStorage(GL_RENDERBUFFER, this->teximage_internalformat(a_type), x, y);
    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::set_framebuffer_renderbuffer(std::size_t fbo_id, std::size_t rbo_id, int slot) {
    this->check_rbo_id(rbo_id, "set_framebuffer_renderbuffer");
    this->check_fbo_id(fbo_id, "set_framebuffer_renderbuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbos_[fbo_id].handle);
    glBindRenderbuffer(GL_RENDERBUFFER, this->rbos_[rbo_id].handle);

    if (this->rbos_[rbo_id].type != nre::gpu::DEPTHSTENCIL) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_RENDERBUFFER, this->rbos_[rbo_id].handle);
    }
    else {
#ifdef OE_PLATFORM_WEB
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbos_[rbo_id].handle);
#else
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->rbos_[rbo_id].handle);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rbos_[rbo_id].handle);
#endif
    }

    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

//---------------------Vertex Buffer-----------------------------//

void nre::gles2::api_t::set_vertex_buffer_memory(std::size_t id, std::size_t memory_size, nre::gpu::BUFFER_USAGE buf_usage) {

    this->check_vbo_id(id, "set_vertex_buffer_memory");

    this->vbos_[id].size  = memory_size;
    this->vbos_[id].usage = buf_usage;
    if (this->active_vbo_ != this->vbos_[id].handle) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbos_[id].handle);
        this->active_vbo_ = this->vbos_[id].handle;
    }
    glBufferData(GL_ARRAY_BUFFER, memory_size * sizeof(float), NULL, nre::gles2::buffer_use(buf_usage));
}
void nre::gles2::api_t::set_vertex_buffer_data(std::size_t id, const std::vector<float>& v, std::size_t offset) {

    this->check_vbo_id(id, "set_vertex_buffer_data");
    this->check_vbo_offset_length(id, offset + v.size(), "set_vertex_buffer_data");

    if (this->active_vbo_ != this->vbos_[id].handle) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbos_[id].handle);
        this->active_vbo_ = this->vbos_[id].handle;
    }
    glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLuint>(offset) * sizeof(float), v.size() * sizeof(float), &v[0]);
}

void nre::gles2::api_t::set_vertex_buffer_memory_data(std::size_t id, const std::vector<float>& v,
                                                      nre::gpu::BUFFER_USAGE buf_usage) {

    this->check_vbo_id(id, "set_vertex_buffer_memory_data");

    this->vbos_[id].size  = v.size();
    this->vbos_[id].usage = buf_usage;
    if (this->active_vbo_ != this->vbos_[id].handle) {
        glBindBuffer(GL_ARRAY_BUFFER, this->vbos_[id].handle);
        this->active_vbo_ = this->vbos_[id].handle;
    }
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), &v[0], nre::gles2::buffer_use(buf_usage));
}

void nre::gles2::api_t::delete_vertex_buffer(std::size_t id) {
    this->check_vbo_id(id, "delete_vertex_buffer");

    glDeleteBuffers(1, &this->vbos_[id].handle);
    this->active_vbo_ = 0;
    this->vbos_.erase(id);
}

//--------------------Index Buffer-------------------------------//

void nre::gles2::api_t::set_index_buffer_memory(std::size_t id, std::size_t memory_size, nre::gpu::BUFFER_USAGE buf_usage) {

    this->check_ibo_id(id, "set_index_buffer_memory");

    this->ibos_[id].size  = memory_size;
    this->ibos_[id].usage = buf_usage;
    this->ibos_[id].type_ = GL_UNSIGNED_INT;
    if (this->vao_ibos_[this->active_vao_] != this->ibos_[id].handle) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibos_[id].handle);
        this->vao_ibos_[this->active_vao_] = this->ibos_[id].handle;
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, memory_size * sizeof(uint32_t), NULL, nre::gles2::buffer_use(buf_usage));
}

void nre::gles2::api_t::set_index_buffer_data(std::size_t id, const std::vector<uint32_t>& v, std::size_t offset) {

    this->check_ibo_id(id, "set_index_buffer_data");
    this->check_ibo_offset_length(id, offset + v.size(), "set_index_buffer_data");

    if (this->vao_ibos_[this->active_vao_] != this->ibos_[id].handle) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibos_[id].handle);
        this->vao_ibos_[this->active_vao_] = this->ibos_[id].handle;
    }
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(offset) * sizeof(uint32_t), v.size() * sizeof(uint32_t),
                    &v[0]);
}

void nre::gles2::api_t::set_index_buffer_memory_data(std::size_t id, const std::vector<uint32_t>& v,
                                                     nre::gpu::BUFFER_USAGE buf_usage) {

    this->check_ibo_id(id, "set_index_buffer_memory_data");

    this->ibos_[id].size  = v.size();
    this->ibos_[id].usage = buf_usage;
    if (v.size() == 0) {
        return;
    }

    if (this->vao_ibos_[this->active_vao_] != this->ibos_[id].handle) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibos_[id].handle);
        this->vao_ibos_[this->active_vao_] = this->ibos_[id].handle;
    }
    if ((*std::max_element(v.begin(), v.end()) >= 65536) and has_oes_element_index_uint_) {
        // 32-bit index buffer needed
        this->ibos_[id].type_ = GL_UNSIGNED_INT;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, v.size() * sizeof(uint32_t), &v[0], nre::gles2::buffer_use(buf_usage));
    }
    else if (not has_oes_element_index_uint_) {
        cout << "NRE WARNING: 32-bit index buffers are not supported on this GPU. Please split your mesh into smaller ones."
             << endl;
    }
    else {
        // optimization to 16-bit buffer can happen
        this->ibos_[id].type_ = GL_UNSIGNED_SHORT;

        // copy to 16-bit buffer
        std::vector<uint16_t> v_16bit;
        v_16bit.reserve(v.size());
        for (auto x : v) {
            v_16bit.push_back(x);
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, v.size() * sizeof(uint16_t), &v_16bit[0], nre::gles2::buffer_use(buf_usage));
    }
}

void nre::gles2::api_t::delete_index_buffer(std::size_t id) {

    this->check_ibo_id(id, "delete_index_buffer");

    glDeleteBuffers(1, &this->ibos_[id].handle);
    this->vao_ibos_[this->active_vao_] = 0;
    this->ibos_.erase(id);
}

//----------------------Uniform State ----------------//

void nre::gles2::api_t::set_program_texture_slot(std::size_t id, std::string_view name, int slot) {
    this->check_prog_id(id, "set_program_texture_slot");
    this->check_prog_complete(id, "set_program_texture_slot");
    this->check_prog_uniform(id, name, "set_program_texture_slot");

    if (this->active_prog_ != this->progs_[id].handle) {
        glUseProgram(this->progs_[id].handle);
        this->active_prog_ = this->progs_[id].handle;
    }
    auto uniform_type_enum = this->prog_db_[this->progs_[id]].uniforms[std::string(name)].type;
    if ((uniform_type_enum == GL_SAMPLER_2D) or (uniform_type_enum == GL_SAMPLER_2D_SHADOW)) {
        // This is really stupid. I wasted a week trying to find what appears an intentional change in emscripten
        // apparently glUniform* function only accept glGetUniformLocation as arguments, else it is "undefined"
        // I do not understand why they did this. It just seems slow for me for no reason
#ifndef OE_PLATFORM_WEB
        glUniform1i(this->prog_db_[this->progs_[id]].uniforms[std::string(name)].slot, slot);
#else
        glUniform1i(glGetUniformLocation(this->progs_[id].handle, std::string(name).c_str()), slot);
#endif
    }
    else {
        cout << "[NRE Warning] No sampler2D uniform named '" << name << "' in program ID: " << id << "." << endl;
        OE_WriteToLog("[NRE Warning] No sampler2D uniform named '" + std::string(name) + "' in program ID: " + to_string(id) +
                      ".");
    }
}

void nre::gles2::api_t::set_program_uniform_data(std::size_t id, std::string_view name, uint32_t data) {
    this->check_prog_id(id, "set_program_uniform_data");
    this->check_prog_complete(id, "set_program_uniform_data");
    this->check_prog_uniform(id, name, "set_program_uniform_data");

    auto prog_handle    = this->progs_[id].handle;
    auto uniform_handle = this->prog_db_[this->progs_[id]].uniforms[std::string(name)];

    if (this->active_prog_ != prog_handle) {
        glUseProgram(prog_handle);
        this->active_prog_ = prog_handle;
    }
    auto uniform_type_enum = uniform_handle.type;
    if (uniform_type_enum == GL_INT) {
        // This is really stupid. I wasted a week trying to find what appears an intentional change in emscripten
        // apparently glUniform* function only accept glGetUniformLocation as arguments, else it is "undefined"
        // I do not understand why they did this. It just seems slow for me for no reason
#ifndef OE_PLATFORM_WEB
        glUniform1i(uniform_handle.slot, data);
#else
        glUniform1i(glGetUniformLocation(prog_handle, std::string(name).c_str()), data);
#endif
    }
    else {
        cout << "[NRE Warning] No integer uniform named '" << name << "' in program ID: " << id << "." << endl;
        OE_WriteToLog("[NRE Warning] No integer uniform named '" + std::string(name) + "' in program ID: " + to_string(id) +
                      ".");
    }
}

void nre::gles2::api_t::set_program_uniform_data(std::size_t id, std::string_view name, float data) {
    this->check_prog_id(id, "set_program_uniform_data");
    this->check_prog_complete(id, "set_program_uniform_data");
    this->check_prog_uniform(id, name, "set_program_uniform_data");

    auto prog_handle    = this->progs_[id].handle;
    auto uniform_handle = this->prog_db_[this->progs_[id]].uniforms[std::string(name)];

    if (this->active_prog_ != prog_handle) {
        glUseProgram(prog_handle);
        this->active_prog_ = prog_handle;
    }
    auto uniform_type_enum = uniform_handle.type;
    if (uniform_type_enum == GL_FLOAT) {
        // This is really stupid. I wasted a week trying to find what appears an intentional change in emscripten
        // apparently glUniform* function only accept glGetUniformLocation as arguments, else it is "undefined"
        // I do not understand why they did this. It just seems slow for me for no reason
#ifndef OE_PLATFORM_WEB
        glUniform1f(uniform_handle.slot, data);
#else
        glUniform1f(glGetUniformLocation(prog_handle, std::string(name).c_str()), data);
#endif
    }
    else {
        cout << "[NRE Warning] No float uniform named '" << name << "' in program ID: " << id << "." << endl;
        OE_WriteToLog("[NRE Warning] No float uniform named '" + std::string(name) + "' in program ID: " + to_string(id) + ".");
    }
}

void nre::gles2::api_t::set_program_uniform_data(std::size_t id, std::string_view name, const std::vector<float>& data) {
    this->check_prog_id(id, "set_program_uniform_data");
    this->check_prog_complete(id, "set_program_uniform_data");
    this->check_prog_uniform(id, name, "set_program_uniform_data");
    this->check_prog_uniform_property(id, name, data.size(), "set_program_uniform_data", false);

    auto prog_handle    = this->progs_[id].handle;
    auto uniform_handle = this->prog_db_[this->progs_[id]].uniforms[std::string(name)];

    if (this->active_prog_ != prog_handle) {
        glUseProgram(prog_handle);
        this->active_prog_ = prog_handle;
    }
    auto uniform_type_enum = uniform_handle.type;
    // This is really stupid. I wasted a week trying to find what appears an intentional change in emscripten
    // apparently glUniform* function only accept glGetUniformLocation as arguments, else it is "undefined"
    // I do not understand why they did this. It just seems slow for me for no reason
#ifndef OE_PLATFORM_WEB
    auto uniform_id = uniform_handle.slot;
    if (uniform_type_enum == GL_FLOAT_VEC2) {
        glUniform2f(uniform_id, data[0], data[1]);
    }
    else if (uniform_type_enum == GL_FLOAT_VEC3) {
        glUniform3f(uniform_id, data[0], data[1], data[2]);
    }
    else if ((uniform_type_enum == GL_FLOAT_VEC4) or (uniform_type_enum == GL_FLOAT_MAT2)) {
        glUniform4f(uniform_id, data[0], data[1], data[2], data[3]);
    }
    else if (uniform_type_enum == GL_FLOAT_MAT3) {
        glUniformMatrix3fv(uniform_id, 1, false, &data[0]);
    }
    else if (uniform_type_enum == GL_FLOAT_MAT4) {
        // cout << "SUCCESS" << id << " " << name <<  endl;

        glUniformMatrix4fv(uniform_id, 1, false, &data[0]);
    }
#else
    if (uniform_type_enum == GL_FLOAT_VEC2) {
        glUniform2f(glGetUniformLocation(prog_handle, std::string(name).c_str()), data[0], data[1]);
    }
    else if (uniform_type_enum == GL_FLOAT_VEC3) {
        glUniform3f(glGetUniformLocation(prog_handle, std::string(name).c_str()), data[0], data[1], data[2]);
    }
    else if ((uniform_type_enum == GL_FLOAT_VEC4) or (uniform_type_enum == GL_FLOAT_MAT2)) {
        glUniform4f(glGetUniformLocation(prog_handle, std::string(name).c_str()), data[0], data[1], data[2], data[3]);
    }
    else if (uniform_type_enum == GL_FLOAT_MAT3) {
        glUniformMatrix3fv(glGetUniformLocation(prog_handle, std::string(name).c_str()), 1, false, &data[0]);
    }
    else if (uniform_type_enum == GL_FLOAT_MAT4) {
        // cout << "SUCCESS" << id << " " << name <<  endl;

        glUniformMatrix4fv(glGetUniformLocation(prog_handle, std::string(name).c_str()), 1, false, &data[0]);
    }
#endif
    else {
        throw nre::gpu::invalid_uniform_property(id, name, data.size(), "set_program_uniform_data", true);
    }
}

int nre::gles2::api_t::get_program_uniform_slot(std::size_t id, std::string_view name) {
    this->check_prog_id(id, "get_program_uniform_slot");
    this->check_prog_complete(id, "get_program_uniform_slot");
    if (this->prog_db_[this->progs_[id]].has_uniform(name)) {
        return this->prog_db_[this->progs_[id]].uniforms[std::string(name)].slot;
    }
    return -2;
}

//---------------------Vertex Layout-----------------------------//

void nre::gles2::api_t::set_vertex_layout_format(std::size_t id, std::vector<nre::gpu::vertex_layout_input> inputs) {

    this->check_vao_id(id, "setVertexlayout_format");

    this->vaos_[id].layout = inputs;
    for (size_t x = 0; x < inputs.size(); x++) {
        this->check_vao_vbo(id, inputs[x].vertex_buffer, "set_vertex_layout_format");
    }
}

void nre::gles2::api_t::delete_vertex_layout(std::size_t id) {
    this->check_vao_id(id, "delete_vertex_layout");
    // glDeleteVertexArrays(1, &this->vaos_[id].handle);
    this->active_vao_ = 0;
    this->vaos_.erase(id);
}

//-----------------------Textures and Framebuffers -------------//

void nre::gles2::api_t::set_texture_format(std::size_t id, nre::gpu::TEXTURE_TYPE type, nre::gpu::TEXTURE_FILTER filter,
                                           uint32_t x_in, uint32_t y_in, int mipmap_count) {
    this->check_texture_id(id, "set_texture_format");

    if (this->textures_[id].has_not_changed(type, filter, x_in, y_in, mipmap_count)) return;

    this->textures_[id].type    = type;
    this->textures_[id].filter  = filter;
    this->textures_[id].x       = x_in;
    this->textures_[id].y       = y_in;
    this->textures_[id].mipmaps = mipmap_count;


    glBindTexture(GL_TEXTURE_2D, this->textures_[id].handle);
    glTexImage2D(GL_TEXTURE_2D, 0, teximage_internalformat(type), x_in, y_in, 0, teximage_format(type), teximage_textype(type),
                 0);
    if (mipmap_count == 0 and filter == nre::gpu::LINEAR) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef OE_PLATFORM_WEB
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
    }
    else if (mipmap_count == 0 and filter == nre::gpu::NEAREST) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#ifdef OE_PLATFORM_WEB
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif
    }
    else {
        // TODO
    }
    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::set_framebuffer_texture(std::size_t fbo_id, std::size_t texture_id, int slot) {
    this->check_fbo_id(fbo_id, "set_framebufferTexture");
    this->check_texture_id(texture_id, "set_framebufferTexture");
    assert((slot >= 0) and (slot < 3));

    this->fbos_[fbo_id].texture = texture_id;

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbos_[fbo_id].handle);
    glBindTexture(GL_TEXTURE_2D, this->textures_[texture_id].handle);

    if (this->textures_[texture_id].type != nre::gpu::DEPTHSTENCIL) {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, this->textures_[texture_id].handle,
                               0);
    }
    else {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, this->textures_[texture_id].handle,
                               0);
    }

    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::set_texture_slot(std::size_t id, int slot) {
    this->check_texture_id(id, "set_texture_slot");
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->textures_[id].handle);
    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::delete_texture(std::size_t id) {
    this->check_texture_id(id, "delete_texture");
    glDeleteTextures(1, &this->textures_[id].handle);
    this->textures_.erase(id);
}

void nre::gles2::api_t::copy_framebuffer(std::size_t src, std::size_t target, nre::gpu::FRAMEBUFFER_COPY method) {
    this->check_fbo_id(src, "copy_framebuffer");
    this->check_texture_id(this->fbos_[src].texture, "copy_framebuffer");

    auto x_tmp = this->textures_[this->fbos_[src].texture].x;
    auto y_tmp = this->textures_[this->fbos_[src].texture].y;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fbos_[src].handle);
    if (target != 0) {
        this->check_fbo_id(target, "copy_framebuffer");
        this->check_texture_id(this->fbos_[target].texture, "copy_framebuffer");
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fbos_[target].handle);
    }
    else {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
    if (method == nre::gpu::FBO_COLOR) {
        glBlitFramebuffer(0, 0, x_tmp, y_tmp, 0, 0, x_tmp, y_tmp, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    else if (method == nre::gpu::FBO_DEPTHSTENCIL) {
        glBlitFramebuffer(0, 0, x_tmp, y_tmp, 0, 0, x_tmp, y_tmp, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
    else {
        glBlitFramebuffer(0, 0, x_tmp, y_tmp, 0, 0, x_tmp, y_tmp,
                          GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
    }
    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::clear_framebuffer(std::size_t id, nre::gpu::FRAMEBUFFER_COPY clear, float alpha_value) {
    this->check_fbo_id(id, "clear_framebuffer");

    glBindFramebuffer(GL_FRAMEBUFFER, this->fbos_[id].handle);
    glDepthMask(GL_TRUE);
    glColorMask(1, 1, 1, 1);
    glClearColor(0.0f, 0.0f, 0.0f, alpha_value);

    if (clear == nre::gpu::FBO_ALL) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    else if (clear == nre::gpu::FBO_COLOR) {
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else if (clear == nre::gpu::FBO_DEPTHSTENCIL) {
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    else if (clear == nre::gpu::FBO_COLORSTENCIL) {

        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    else {
    }
    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::discard_framebuffer(std::size_t id) {

#ifndef __EMSCRIPTEN__

    if (id != 0) {
        this->check_fbo_id(id, "discard_framebuffer");
        if (not has_ext_discard_framebuffer_) return;
        const GLenum attachment_discards[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT};
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbos_[id].handle);
        glDiscardFramebufferEXT(GL_FRAMEBUFFER, 3, &attachment_discards[0]);
    }
    else {
        if (not has_ext_discard_framebuffer_) return;
        const GLenum attachment_discards[] = {GL_COLOR, GL_DEPTH};
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDiscardFramebufferEXT(GL_FRAMEBUFFER, 2, &attachment_discards[0]);
    }
    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
#endif
}

void nre::gles2::api_t::use_framebuffer(std::size_t id) {
    if (id != 0) {
        this->check_fbo_id(id, "use_framebuffer");
        glBindFramebuffer(GL_FRAMEBUFFER, this->fbos_[id].handle);
    }
    else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::delete_framebuffer(std::size_t id) {
    this->check_fbo_id(id, "delete_framebuffer");
    glDeleteFramebuffers(1, &this->fbos_[id].handle);
    this->fbos_.erase(id);
}

//---------------------Shader Programs-----------------------------//

void nre::gles2::api_t::set_program_vs(std::size_t id, nre::gpu::vertex_shader_t vs) {
    this->check_prog_id(id, "set_program_vs");

    this->progs_[id].vs_setup = false;
    this->progs_[id].setup    = false;
    this->progs_[id].vs       = vs;
}
void nre::gles2::api_t::set_program_fs(std::size_t id, nre::gpu::pixel_shader_t fs) {
    this->check_prog_id(id, "set_program_fs");

    this->progs_[id].fs_setup = false;
    this->progs_[id].setup    = false;
    this->progs_[id].fs       = fs;
}

void nre::gles2::api_t::set_program_vs(std::size_t id, std::string data) {

    data = data + "";
    this->check_prog_id(id, "set_program_vs - internal");
    GLuint shader_id;
    if (!this->progs_[id].vs_setup)
        shader_id = glCreateShader(GL_VERTEX_SHADER);
    else
        shader_id = this->progs_[id].vs_handle;

    this->progs_[id].vs_setup  = true;
    this->progs_[id].vs_handle = shader_id;
    this->progs_[id].setup     = false;

    /// compile and attach shader
    const char* c_str = data.c_str();

    GLint data_size = data.length();
    glShaderSource(shader_id, 1, &c_str, &data_size);
    glCompileShader(shader_id);

    /// IMPORTANT: get shader compiler info
    int parameters = -1;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &parameters);
    if (parameters != GL_TRUE) {

        int  max_length    = 2048;
        int  actual_length = 0;
        char log[2048];
        glGetShaderInfoLog(shader_id, max_length, &actual_length, log);

        stringstream ss;
        ss << "[NRE GL Shader Warning] in vertex shader: " << this->progs_[id].vs.info() << endl << log << endl;
        cout << ss.str();
        OE_WriteToLog(ss.str());
    }
}

// void set_programGS(std::size_t, FE_GPU_Shader){}
void nre::gles2::api_t::set_program_fs(std::size_t id, std::string data) {

    data = data + "";
    this->check_prog_id(id, "set_program_fs - internal");
    GLuint shader_id;
    if (!this->progs_[id].fs_setup)
        shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    else
        shader_id = this->progs_[id].fs_handle;

    this->progs_[id].fs_setup  = true;
    this->progs_[id].fs_handle = shader_id;
    this->progs_[id].setup     = false;

    /// compile and attach shader
    const char* c_str     = data.c_str();
    GLint       data_size = data.length();
    glShaderSource(shader_id, 1, &c_str, &data_size);
    glCompileShader(shader_id);

    /// IMPORTANT: get shader compiler info
    int parameters = -1;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &parameters);
    if (parameters != GL_TRUE) {

        int  max_length    = 2048;
        int  actual_length = 0;
        char log[2048];
        glGetShaderInfoLog(shader_id, max_length, &actual_length, log);

        stringstream ss;
        ss << "[NRE GL Shader Warning] in pixel shader: " << this->progs_[id].fs.info() << endl << log << endl;
        cout << ss.str();
        OE_WriteToLog(ss.str());
    }
}

// void set_programTCS(std::size_t, FE_GPU_Shader){}
// void set_programTES(std::size_t, FE_GPU_Shader){}

void nre::gles2::api_t::setup_program(std::size_t id) {

    this->check_prog_id(id, "setup_program");

    // ignore if the program is already setup
    if (this->progs_[id].setup) return;

    // setup vertex shader
    if (!this->progs_[id].vs_setup) {
        if (this->vs_db_.count(this->progs_[id].vs) == 0) {

            // vertex shader does not exist, make a new entry
            this->set_program_vs(id, this->progs_[id].vs.gen_shader());
            this->vs_db_[this->progs_[id].vs] = this->progs_[id].vs_handle;
        }
        else {

            // vertex shader already exists, reuse that
            this->progs_[id].vs_handle = this->vs_db_[this->progs_[id].vs];
            this->progs_[id].vs_setup  = true;
        }
    }

    // setup pixel (fragment) shader
    if (!this->progs_[id].fs_setup) {
        if (this->fs_db_.count(this->progs_[id].fs) == 0) {

            // pixel (fragment) shader does not exist, make a new entry
            this->set_program_fs(id, this->progs_[id].fs.gen_shader());

            this->fs_db_[this->progs_[id].fs] = this->progs_[id].fs_handle;
            this->progs_[id].fs_setup         = true;
        }
        else {

            // pixel (fragment) shader already exists, reuse that
            this->progs_[id].fs_handle = this->fs_db_[this->progs_[id].fs];
            this->progs_[id].fs_setup  = true;
        }
    }

    // check if program already exists
    if (this->prog_db_.count(this->progs_[id]) > 0) {

        this->progs_[id].handle = this->prog_db_[this->progs_[id]].handle;
        this->progs_[id].setup  = true;

        return;
    }
    else {
        this->progs_[id].handle = glCreateProgram();
        // this->prog_db_[this->progs_[id]] = this->progs_[id].handle;
        this->prog_db_[this->progs_[id]]        = nre::gles2::program_data_t();
        this->prog_db_[this->progs_[id]].handle = this->progs_[id].handle;
    }

    // make sure that the vertex and fragment shaders actually compile
    // in case the program does not already exist
    assert(this->progs_[id].vs_setup && this->progs_[id].fs_setup);

    glAttachShader(this->progs_[id].handle, this->progs_[id].vs_handle);
    glAttachShader(this->progs_[id].handle, this->progs_[id].fs_handle);

    glBindAttribLocation(this->progs_[id].handle, 0, "oe_position");

    if (not(this->progs_[id].vs.fullscreenQuad or (this->progs_[id].vs.type == nre::gpu::VS_Z_PREPASS))) {
        glBindAttribLocation(this->progs_[id].handle, 1, "oe_normals");

        int offset = 2;
        if (this->progs_[id].vs.type == nre::gpu::VS_REGULAR_SOFTWARE) {
            glBindAttribLocation(this->progs_[id].handle, 2, "oe_position_clip");
            offset = offset + 1;
        }

        for (size_t i = 0; i < this->progs_[id].vs.num_of_uvs; i++) {
            std::string attrib_name = "oe_uvs" + to_string(i);
            glBindAttribLocation(this->progs_[id].handle, i + offset, attrib_name.c_str());
        }
    }


    glLinkProgram(this->progs_[id].handle);


    /// IMPORTANT: check if program is linked correctly
    int parameters = -1;
    glGetProgramiv(this->progs_[id].handle, GL_LINK_STATUS, &parameters);
    if (GL_TRUE != parameters) {

        int  max_length    = 2048;
        int  actual_length = 0;
        char log[2048];
        glGetProgramInfoLog(this->progs_[id].handle, max_length, &actual_length, log);
        cout << log << endl;
        OE_WriteToLog(log);
    }

    /// IMPORTANT: make sure program is runnable
    glValidateProgram(this->progs_[id].handle);
    parameters = -1;
    glGetProgramiv(this->progs_[id].handle, GL_VALIDATE_STATUS, &parameters);
    if (GL_TRUE != parameters) {

        int  max_length    = 2048;
        int  actual_length = 0;
        char log[2048];
        glGetProgramInfoLog(this->progs_[id].handle, max_length, &actual_length, log);

        stringstream ss;
        ss << "[NRE GL Shader Linking Warning] with shaders: " << this->progs_[id].vs.info() << " "
           << this->progs_[id].fs.info() << endl
           << log << endl;
        cout << ss.str();
        OE_WriteToLog(ss.str());
    }
    glUseProgram(this->progs_[id].handle);
    this->active_prog_ = this->progs_[id].handle;

    /// get all active uniform blocks
    this->get_program_all_uniforms(id);

    this->progs_[id].setup = true;

    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::delete_program(std::size_t id) {

    this->check_prog_id(id, "delete_program");

    this->active_prog_ = 0;
    this->progs_.erase(id);
}

//---------------------Draw calls-----------------------------//

void nre::gles2::api_t::draw(nre::gpu::draw_call dc_info) {

    this->check_prog_id(dc_info.program, "draw");
    this->check_vao_id(dc_info.vertex_layout, "draw");

    bool is_ranged_rendering = (dc_info.offset != 0) or (dc_info.amount != 0);

    // program
    this->setup_program(dc_info.program);
    if (this->active_prog_ != this->progs_[dc_info.program].handle) {
        glUseProgram(this->progs_[dc_info.program].handle);
        this->active_prog_ = this->progs_[dc_info.program].handle;
    }

    // vao
    auto vao_id = dc_info.vertex_layout;
    if (this->active_vao_ != vao_id) {

        // disable unused vertex attributes if enabled before
        if (this->enabled_vao_attribs_ > this->vaos_[vao_id].layout.size()) {
            for (std::size_t i = this->enabled_vao_attribs_; i > this->vaos_[vao_id].layout.size(); i--) {
                glDisableVertexAttribArray(i);
            }
        }

        for (size_t x = 0; x < this->vaos_[vao_id].layout.size(); x++) {

            this->check_vao_vbo(vao_id, this->vaos_[vao_id].layout[x].vertex_buffer, "draw");

            glBindBuffer(GL_ARRAY_BUFFER, this->vbos_[this->vaos_[vao_id].layout[x].vertex_buffer].handle);
            glVertexAttribPointer(
                x, this->vaos_[vao_id].layout[x].amount, GL_FLOAT, GL_FALSE,
                sizeof(GLfloat) * this->vaos_[vao_id].layout[x].stride,
                NRE_GLES2_VERTEXL_LAYOUT_OFFSET(sizeof(GLfloat) * static_cast<GLuint>(this->vaos_[vao_id].layout[x].offset)));
            glEnableVertexAttribArray(x);
        }
        this->enabled_vao_attribs_ = this->vaos_[vao_id].layout.size();
        this->active_vbo_          = 0;

        this->active_vao_ = vao_id;
    }

    // optional elements handling
    if (dc_info.index_buf != 0) {

        this->check_ibo_id(dc_info.index_buf, "draw (indexed)");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibos_[dc_info.index_buf].handle);

        if (is_ranged_rendering) {
            this->check_ibo_offset_length(dc_info.index_buf, dc_info.offset + dc_info.amount, "draw (indexed)");
            if (this->ibos_[dc_info.index_buf].type_ == GL_UNSIGNED_INT and has_oes_element_index_uint_)
                glDrawElements(GL_TRIANGLES, dc_info.amount, GL_UNSIGNED_INT,
                               (GLvoid*)((sizeof(uint32_t)) * 3 * dc_info.offset));
            else if (not has_oes_element_index_uint_)
                cout << "NRE WARNING: Draw call hidden due to too many vertices (no 32-bit index support)" << endl;
            else
                glDrawElements(GL_TRIANGLES, dc_info.amount, GL_UNSIGNED_SHORT,
                               (GLvoid*)((sizeof(uint16_t)) * 3 * dc_info.offset));
        }
        else {
            if ((this->ibos_[dc_info.index_buf].type_ == GL_UNSIGNED_SHORT) or has_oes_element_index_uint_)
                glDrawElements(GL_TRIANGLES, this->ibos_[dc_info.index_buf].size, this->ibos_[dc_info.index_buf].type_,
                               (GLvoid*)NULL);
            else
                cout << "NRE WARNING: Draw call hidden due to too many vertices (no 32-bit index support)" << endl;
        }
    }
    else {
        if (is_ranged_rendering) {
            this->check_draw_range(vao_id, this->get_vao_size(vao_id), dc_info.offset, dc_info.amount, "draw");
            glDrawArrays(GL_TRIANGLES, dc_info.offset, dc_info.amount);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, this->get_vao_size(vao_id));
        }
    }
    auto possible_error = glGetError();
    if (possible_error > 0) {
        cout << "GL Error: " << possible_error << endl;
        OE_WriteToLog(std::to_string(possible_error));
    }
}

void nre::gles2::api_t::set_render_mode(nre::gpu::RENDERMODE rendermode) {

    if (rendermode == nre::gpu::Z_PREPASS_BACKFACE) {
        glDisable(GL_BLEND);
        glDisable(GL_STENCIL_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); /// cull back face
        glFrontFace(GL_CCW);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glColorMask(0, 0, 0, 0);
        glDepthMask(GL_TRUE);
    }
    else if (rendermode == nre::gpu::REGULAR_FRONTFACE) {
        glDisable(GL_BLEND);
        glDisable(GL_STENCIL_TEST);

        glEnable(GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc(GL_LESS);
        glColorMask(1, 1, 1, 1);
        glDepthMask(GL_TRUE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT); /// cull back face
        glFrontFace(GL_CCW);
    }
    else if (rendermode == nre::gpu::REGULAR_BACKFACE) {
        glDisable(GL_BLEND);
        glDisable(GL_STENCIL_TEST);

        glEnable(GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc(GL_LESS);
        glColorMask(1, 1, 1, 1);
        glDepthMask(GL_TRUE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); /// cull back face
        glFrontFace(GL_CCW);
    }
    else if (rendermode == nre::gpu::AFTERPREPASS_BACKFACE) {
        glDisable(GL_BLEND);
        glDisable(GL_STENCIL_TEST);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glColorMask(1, 1, 1, 1);
        glDepthMask(GL_FALSE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); /// cull back face
        glFrontFace(GL_CCW);
    }
    else if (rendermode == nre::gpu::REGULAR_BOTH) {
        glDisable(GL_BLEND);
        glDisable(GL_STENCIL_TEST);

        glEnable(GL_DEPTH_TEST); // enable depth-testing
        glDepthFunc(GL_LESS);
        glColorMask(1, 1, 1, 1);
        glDepthMask(GL_TRUE);

        glDisable(GL_CULL_FACE);
    }
    else if (rendermode == nre::gpu::FULLSCREEN_QUAD) {
        glDisable(GL_BLEND);

        glDisable(GL_STENCIL_TEST);
        glDisable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LEQUAL);
        glColorMask(1, 1, 1, 1);
        glDepthMask(GL_TRUE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); /// cull back face
        glFrontFace(GL_CCW);
    }
    else if (rendermode == nre::gpu::LIGHT_PREPASS) {
        glDisable(GL_BLEND);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glStencilOp(GL_KEEP, GL_REPLACE, GL_ZERO);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glColorMask(0, 0, 0, 0);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); /// cull back face
        glFrontFace(GL_CCW);
    }
    else if (rendermode == nre::gpu::LIGHT_AFTERPASS) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);
        glBlendColor(0.25, 0.25, 0.25, 0.25);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_EQUAL, 0, 0xFF);
        glStencilMask(0x00);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_GEQUAL);
        glColorMask(1, 1, 1, 1);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT); /// cull front face
        glFrontFace(GL_CCW);
    }
    else if (rendermode == nre::gpu::LIGHT_PREPASS_2) {
        glDisable(GL_BLEND);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 2, 0xFF);
        // glStencilFunc(GL_GEQUAL, 2, 0xFF);
        glStencilMask(0xFF);
        glStencilOp(GL_KEEP, GL_REPLACE, GL_ZERO);
        // glStencilOp(GL_KEEP, GL_INCR, GL_DECR);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glColorMask(0, 0, 0, 0);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK); /// cull back face
        glFrontFace(GL_CCW);
    }
    else if (rendermode == nre::gpu::LIGHT_AFTERPASS_RG) {
        glDisable(GL_BLEND);
        // glBlendEquation(GL_MAX);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_GREATER, 2, 0xFF);
        glStencilMask(0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_GEQUAL);
        glColorMask(1, 1, 0, 0);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT); /// cull front face
        glFrontFace(GL_CCW);
    }
    else if (rendermode == nre::gpu::LIGHT_AFTERPASS_BA) {
        glDisable(GL_BLEND);
        // glBlendEquation(GL_MAX);

        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_EQUAL, 0, 0xFF);
        glStencilMask(0xFF);
        glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_GEQUAL);
        glColorMask(0, 0, 1, 1);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT); /// cull front face
        glFrontFace(GL_CCW);
    }
    else {
        // TODO
    }
}

void nre::gles2::api_t::use_wireframe(bool value_in) {
    // TODO: Warning
}
