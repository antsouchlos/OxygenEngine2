#ifndef NRE_GPU_SHADERS_H
#define NRE_GPU_SHADERS_H

#define NRE_Shader(src) #src

#include <OE/types/base_types.h>

namespace nre { namespace gpu {

    // possibility to extend with HLSL/Metal etc.
    enum SHADER_BACKEND : int { UNDEFINED, GL, GLES, GLES2 };

    enum VS_TYPE {
        VS_UNDEFINED,
        VS_Z_PREPASS,
        VS_REGULAR,
        VS_REGULAR_SOFTWARE,
        VS_BOUNDING_BOX,
        VS_BOUNDING_SPHERE,
        VS_LIGHT,
    };

    enum FS_TYPE {
        FS_UNDEFINED,
        FS_SIMPLE,
        FS_GAMMA,
        FS_MATERIAL,
        FS_NORMALS,
        FS_SIMPLE_LIGHT,
        FS_DIR_LIGHT,
        FS_INDIR_LIGHTS,
        FS_LIGHT_INDEX
    };

    std::string gen_shader_prefix();

    class shader_base_t {
    public:
        virtual ~shader_base_t();
        virtual std::string gen_shader() const;
        virtual std::string info() const;
    };

    class vertex_shader_t : public shader_base_t {
    public:
        vertex_shader_t();
        ~vertex_shader_t();

        bool operator==(const vertex_shader_t&) const;

        std::string gen_shader() const;
        std::string info() const;
        size_t      gen_hash() const;

        bool        fullscreenQuad{false};
        std::size_t num_of_uvs{0};
        VS_TYPE     type{VS_UNDEFINED};
    };

    class pixel_shader_t : public shader_base_t {
    public:
        pixel_shader_t();
        ~pixel_shader_t();

        bool operator==(const pixel_shader_t&) const;

        std::string gen_shader() const;
        std::string info() const;
        size_t      gen_hash() const;

        std::size_t num_of_uvs{0};
        FS_TYPE     type{FS_UNDEFINED};
    };

}; }; // namespace nre::gpu


// specializations for hashing

namespace std {
template <>
struct hash<nre::gpu::vertex_shader_t> {
    auto operator()(const nre::gpu::vertex_shader_t& xyz) const -> size_t {
        return hash<size_t>{}(xyz.gen_hash());
    }
};
} // namespace std

namespace std {
template <>
struct hash<nre::gpu::pixel_shader_t> {
    auto operator()(const nre::gpu::pixel_shader_t& xyz) const -> size_t {
        return hash<size_t>{}(xyz.gen_hash());
    }
};
} // namespace std

#endif
