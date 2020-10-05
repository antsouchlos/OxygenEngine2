#ifndef OE_POLYGONSTORAGE_H
#define OE_POLYGONSTORAGE_H

#include <types/OE_VertexStorage.h>
#include <types/OE_VertexGroup.h>

/* OE_Triangle32 stores for each vertex a uint32_t array
 * with the indices of positions, normals and UVs, for example [0/0/0, 1/2/1, 2/3/2] 
 * in a similar fashion to .obj and COLLADA formats
 */

class OE_Triangle32 : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase {
    
    friend class OE_PolygonStorage;
    friend class CSL_Interpreter;
    
    public:
        
        OE_Triangle32();
        ~OE_Triangle32();
        
        std::string to_str(const std::size_t&) const;
    
    protected:
        uint32_t*   v1{nullptr};
        uint32_t*   v2{nullptr};
        uint32_t*   v3{nullptr};
};

/** OE_PolygonStorage32 stores:
 *  - vertices          : the actual vertex/normal/UV data
 *  - isDynamic         : specifies if the mesh vertices will change frequently or not (GL_STATIC_DRAW vs GL_STREAM_DRAW in OpenGL)
 *  - triangles         : the OE_Triangle32 array with the indices
 *  - triangle_groups   : the vertex groups for materials/bones etc.
 * 
 *  - vertex_buffer     : upon loading of the object, duplicates are removed and the same array pointers that are in the OE_Triangle32
 *                        array are stored here
 *  - index_buffer      : for easy lookup and manipulation of the index buffer given to the graphics API by not having to find each unique vertex manually. (32-bit indices)
 * 
 * addTriangle()        : tries to add a specific index sequence ({vertex, normal, uv1...}). If the sequence
 *                      already exists, then it returns true. 
 */

class OE_PolygonStorage32 {
    
    friend class CSL_Interpreter;
    friend class OE_Mesh32;
    
    public:
        OE_PolygonStorage32();
        ~OE_PolygonStorage32();
        
        uint32_t* addTriangle(uint32_t* indices);
        //int removeTriangle(size_t);
        
        std::size_t                                 num_of_uvs;
    protected:
        OE_VertexStorage                            vertices;
        bool                                        isDynamic{true};
        std::vector<OE_Triangle32>                  triangles;
        std::map<std::size_t, OE_VertexGroup*>      triangle_groups;

    private:
        std::vector<uint32_t*>                      vertex_buffer;
        std::map<uint32_t*, uint32_t, std::function<bool(uint32_t*, uint32_t*)>>               index_buffer;
};

void printArray(const uint32_t* x, const uint32_t& arrsize);

#endif
