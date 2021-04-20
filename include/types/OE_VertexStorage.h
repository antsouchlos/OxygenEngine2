#ifndef OE_VERTEXSTORAGE_H
#define OE_VERTEXSTORAGE_H

#include <types/OE_TypesBase.h>
#include <cassert>


struct OE_UVMapElement{
    float u{0.0f};
    float v{0.0f};
};

typedef std::map<std::size_t, OE_UVMapElement> OE_UVMaps;


/// This struct is intended to be used for storing a complete UV map with ALL texture coordinates.

class OE_UVMapData : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase {
    
    friend class CSL_Interpreter;
    
    public:
        static std::unordered_map<std::size_t, std::string> id2name;
        static OE_Name2ID name2id;
        std::size_t id;
        
        OE_UVMapData();
        OE_UVMapData(const std::size_t&, const std::string&);
        ~OE_UVMapData();
        
        std::string to_str() const;
        
        std::vector<float> elements;
};



/** OE_VertexStorage32 is intended to be used for storing vertices positions normals and UVs to be directly fed to the renderer.
 *  It stores vertex data in a 32-bit floating point value.
 */

class OE_VertexStorage{
    
    friend class CSL_Interpreter;
    friend class OE_Mesh32;
    friend class OE_PolygonStorage32;
    friend class NRE_Renderer;
    
    public:
        OE_VertexStorage();
        ~OE_VertexStorage();
        
        //int removeVertex(std::size_t);
        // These are the bounding box coordinates
        
        float max_x{0.0f}, max_y{0.0f}, max_z{0.0f};
        float min_x{0.0f}, min_y{0.0f}, min_z{0.0f};
        
        float max_radius{0.0f};
        float min_radius{0.0f};
        
        void calculateNaiveBoundingBox();
        
        bool calculatedBoundingBox{false};
        
//    protected:
        
        std::vector<float>              positions;
        std::vector<float>              normals;
        std::vector<OE_UVMapData>       uvmaps;
};



#endif
