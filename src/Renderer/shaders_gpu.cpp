#include <OE/Renderer/GL3/shaders_gl3.h>
#include <OE/Renderer/GLES2/shaders_gles2.h>
#include <OE/Renderer/api_gpu.h>
#include <bitset>
#include <iostream>
#include <sstream>
using namespace std;
using namespace nre::gpu;


// General boilerplate

nre::gpu::shader_base_t::~shader_base_t() {
}

std::string nre::gpu::gen_shader_prefix() {

    std::string output;
    auto        backend = nre::gpu::get_api();
    int         major   = nre::gpu::get_major_api_version();
    int         minor   = nre::gpu::get_minor_api_version();

    if (backend == GLES) {
        output = "#version 300 es \n";
    }
    else if (backend == GL) {

        if (major == 3 && minor == 1) {
            output = "#version 140 \n"
                     "#extension GL_ARB_explicit_attrib_location"
                     ": require\n";
        }
        else if (major == 3 && minor >= 2) {
            output = "#version 150 core \n"
                     "#extension GL_ARB_explicit_attrib_location"
                     ": require\n";
        }
        else {
            cout << "WTF" << endl;
        }
    }
    else if (backend == GLES2) {
        output = "#version 100 \n";
    }
    else {
        cout << "Undefined NRE GPU Shader backend. Nothing is rendered" << endl;
    }
    return output;
}


std::string nre::gpu::shader_base_t::gen_shader() const {
    return "";
}

std::string nre::gpu::shader_base_t::info() const {
    return "";
}

// Vertex Shader

nre::gpu::vertex_shader_t::vertex_shader_t() {
}

nre::gpu::vertex_shader_t::~vertex_shader_t() {
}

bool nre::gpu::vertex_shader_t::operator==(const nre::gpu::vertex_shader_t& other) const {
    return std::tie(this->fullscreenQuad, this->type, this->num_of_uvs) ==
           std::tie(other.fullscreenQuad, other.type, other.num_of_uvs);
}


std::string nre::gpu::vertex_shader_t::gen_shader() const {
    if (nre::gpu::get_api() == GL || nre::gpu::get_api() == GLES)
        return nre::gl3::gen_vertex_shader(*this);
    else if (nre::gpu::get_api() == GLES2)
        return nre::gles2::gen_vertex_shader(*this);
    else
        return "";
}

std::string nre::gpu::vertex_shader_t::info() const {

    stringstream ss;
    switch (this->type) {
    case VS_UNDEFINED:
        ss << "VS_UNDEFINED";
        break;
    case VS_Z_PREPASS:
        ss << "VS_Z_PREPASS";
        break;
    case VS_REGULAR:
        ss << "VS_REGULAR";
        break;
    case VS_REGULAR_SOFTWARE:
        ss << "VS_REGULAR_SOFTWARE";
        break;
    case VS_BOUNDING_BOX:
        ss << "VS_BOUNDING_BOX";
        break;
    case VS_BOUNDING_SPHERE:
        ss << "VS_BOUNDING_SPHERE";
        break;
    case VS_LIGHT:
        ss << "VS_LIGHT";
        break;
    }
    ss << " " << this->fullscreenQuad << " " << this->num_of_uvs;
    return ss.str();
}


size_t nre::gpu::vertex_shader_t::gen_hash() const {
    // should only use the first 32
    std::bitset<64> output_bits(this->fullscreenQuad);

    output_bits |= (num_of_uvs & 7) << 1;
    output_bits |= (this->type << 4);


    return output_bits.to_ullong();
}

// Fragment/Pixel Shader

nre::gpu::pixel_shader_t::pixel_shader_t() {
}

nre::gpu::pixel_shader_t::~pixel_shader_t() {
}

bool nre::gpu::pixel_shader_t::operator==(const nre::gpu::pixel_shader_t& other) const {
    return std::tie(this->type, this->num_of_uvs) == std::tie(other.type, other.num_of_uvs);
}

std::string nre::gpu::pixel_shader_t::gen_shader() const {
    if (nre::gpu::get_api() == GL || nre::gpu::get_api() == GLES)
        return nre::gl3::gen_pixel_shader(*this);
    else if (nre::gpu::get_api() == GLES2)
        return nre::gles2::gen_pixel_shader(*this);
    else
        return "";
    return "";
}

std::string nre::gpu::pixel_shader_t::info() const {
    stringstream ss;
    switch (this->type) {
    case FS_UNDEFINED:
        ss << "FS_UNDEFINED";
        break;
    case FS_SIMPLE:
        ss << "FS_SIMPLE";
        break;
    case FS_GAMMA:
        ss << "FS_GAMMA";
        break;
    case FS_MATERIAL:
        ss << "FS_MATERIAL";
        break;
    case FS_NORMALS:
        ss << "FS_NORMALS";
        break;
    case FS_SIMPLE_LIGHT:
        ss << "FS_SIMPLE_LIGHT";
        break;
    case FS_DIR_LIGHT:
        ss << "FS_DIR_LIGHT";
        break;
    case FS_INDIR_LIGHTS:
        ss << "FS_INDIR_LIGHTS";
        break;
    case FS_LIGHT_INDEX:
        ss << "FS_LIGHT_INDEX";
        break;
    }
    ss << " " << this->num_of_uvs;
    return ss.str();
}


size_t nre::gpu::pixel_shader_t::gen_hash() const {
    // should only use the last 32
    std::bitset<64> output_bits(0);

    output_bits |= (num_of_uvs & 7) << 29;
    output_bits |= (this->type << 26);
    output_bits = output_bits << 32;

    return output_bits.to_ullong();
}
