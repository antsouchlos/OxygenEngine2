#ifndef OE_POLYGONSTORAGE_H
#define OE_POLYGONSTORAGE_H

#include <OE/types/vertex_group.h>
#include <OE/types/vertex_storage.h>
#include <bitset>
#include <functional>
#include <memory>

class OE_Mesh32;

struct OE_IndexBufferReady {
    std::vector<uint32_t> data;
};

// TODO: Don't do that.
// This is a helper function for the unordered index buffer
void OE_ReverseBitset32(std::bitset<64>&);



/* This classes optimize the storage by either using
 * an ordered or an unordered map transparently
 */

class OE_IndexBufferWrapperBase {
public:
    OE_IndexBufferWrapperBase();
    virtual ~OE_IndexBufferWrapperBase();

    virtual uint32_t& operator[](uint32_t*);

    virtual std::size_t count(uint32_t*);
    virtual std::size_t size();

    bool     isInit{false};
    uint32_t dummy_var{0};
};

class OE_IndexBufferUnorderedMap : public OE_IndexBufferWrapperBase {
public:
    OE_IndexBufferUnorderedMap(uint32_t num_of_indices);
    ~OE_IndexBufferUnorderedMap();

    uint32_t& operator[](uint32_t*);

    std::size_t count(uint32_t*);
    std::size_t size();

    std::unordered_map<uint32_t*, uint32_t, std::function<size_t(uint32_t*)>, std::function<bool(uint32_t*, uint32_t*)>> data;
};

class OE_IndexBufferMap : public OE_IndexBufferWrapperBase {
public:
    OE_IndexBufferMap(uint32_t num_of_indices);
    ~OE_IndexBufferMap();

    uint32_t& operator[](uint32_t*);

    std::size_t count(uint32_t*);
    std::size_t size();

    std::map<uint32_t*, uint32_t, std::function<bool(uint32_t*, uint32_t*)>> data;
};


/* OE_Triangle32 stores for each vertex a uint32_t array
 * with the indices of positions, normals and UVs, for example [0/0/0, 1/2/1, 2/3/2]
 * in a similar fashion to .obj and COLLADA formats
 */

class OE_Triangle32 : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase {

    friend class OE_PolygonStorage32;
    friend class CSL_Interpreter;

    using CSL_WriterBase::to_str;

public:
    OE_Triangle32();
    ~OE_Triangle32();

    std::string to_str(const std::size_t&) const;

    //	protected:
    uint32_t* v1{nullptr};
    uint32_t* v2{nullptr};
    uint32_t* v3{nullptr};
};

/** OE_PolygonStorage32 stores:
 *  - vertices		  : the actual vertex/normal/UV data
 *  - isDynamic		 : specifies if the mesh vertices will change frequently or not (GL_STATIC_DRAW vs GL_STREAM_DRAW in
 *OpenGL)
 *  - triangles		 : the OE_Triangle32 array with the indices
 *  - triangle_groups   : the vertex groups for materials/bones etc.
 *
 *  - vertex_buffer	 : upon loading of the object, duplicates are removed and the same array pointers that are in the
 *OE_Triangle32 array are stored here
 *  - index_buffer	  : for easy lookup and manipulation of the index buffer given to the graphics API by not having to find
 *each unique vertex manually. (32-bit indices)
 *
 * addTriangle()		: tries to add a specific index sequence ({vertex, normal, uv1...}). If the sequence
 *					  already exists, then it returns true.
 *
 *
 * IMPORTANT: DO NOT TOUCH anything protected or private unless you really know what you are doing
 * A LOT of stuff will be broken if you do sth wrong
 */

class OE_PolygonStorage32 {

    //	friend class CSL_Interpreter;
    friend class OE_Mesh32;
    friend class NRE_Renderer;
    friend class NRE_RendererLegacy;
    friend class NRE_DataHandler;

public:
    OE_PolygonStorage32(uint32_t num_of_vertices, uint32_t num_of_normals, uint32_t num_of_triangles, uint32_t num_of_uvs,
                        uint32_t max_uv_num);
    // TODO: Rule of 5
    ~OE_PolygonStorage32();


    std::vector<float>    genVertexBuffer();
    std::vector<uint32_t> genIndexBuffer(const std::size_t& vgroup_id);

    void genVertexBufferInternally();
    void genIndexBuffersInternally();


    // TODO: Is this necessary the way it is, or is it more logical to store
    // num_of_indices (being num_of_uvs + 2)?
    std::size_t num_of_uvs;



    //	protected:
    uint32_t* addTriangleVertexIndexTuple(uint32_t* vertex_arr, uint32_t len);

    // TODO: Make these private (Need the new Interpreter for this)
    // TODO: Smart pointers
    void initUnorderedIB(uint32_t num_of_indices);
    void initOrderedIB(uint32_t num_of_indices);


    std::vector<OE_Triangle32>                             triangles;
    std::map<std::size_t, std::shared_ptr<OE_VertexGroup>> triangle_groups;

    OE_VertexStorage vertices;

    bool isDynamic{true};
    bool changed{false};

    // TODO: Smart pointers / Proper containers
    std::vector<uint32_t*>                     vertex_buffer;
    std::shared_ptr<OE_IndexBufferWrapperBase> index_buffer;



protected:
    // This stuff is only of interest to the renderer
    // Those vectors should be cleared after first use by the renderer on loading the object
    // both should be guarded by mutexes
    OE_THREAD_SAFETY_OBJECT vbo_mutex;
    OE_THREAD_SAFETY_OBJECT ibos_mutex;


    std::vector<float>                                   vbo;
    std::unordered_map<std::size_t, OE_IndexBufferReady> ibos;

    bool vbo_exists{false};
    bool ibos_exist{false};
};



void printArray(const uint32_t* x, const uint32_t& arrsize);

#endif
