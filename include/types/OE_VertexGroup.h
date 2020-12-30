#ifndef OE_VERTEXGROUP_H
#define OE_VERTEXGROUP_H

#include <types/OE_TypesBase.h>

/** OE_VertexGroup is a basis for groups of triangles (never mind the name being 'vertices')
 *  Each instance is assigned a material and a bone id and each instance of these usually gets their own drawcall, unless optimizations are enabled
 *  Each instance can also be set to hidden as a result
 */

class OE_VertexGroup : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase{
    
//    friend class CSL_Interpreter;
    friend class OE_PolygonStorage32;
    friend class NRE_Renderer;
    
    public:
        
        static std::unordered_map<std::size_t, std::string> id2name;
        static std::atomic<std::size_t> current_id;
        static OE_Name2ID name2id;
        std::size_t id;
    
        OE_VertexGroup();
        OE_VertexGroup(const std::string &name);
        ~OE_VertexGroup();
        
        std::string to_str() const;
        
//    protected:
        
        std::vector<uint32_t>       polygons;
        std::size_t                 material_id;
        std::size_t                 bone_id;
        bool                        visible;
        
};



#endif
