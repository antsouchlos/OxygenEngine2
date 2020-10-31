#ifndef OE_OBJECT_H
#define OE_OBJECT_H

#include <types/OE_TypesBase.h>

// NOTE:  the structure of this struct (yeah i know) is subject to change
struct OE_ObjectData{
    float pos_x{0.0f}, pos_y{0.0f}, pos_z{0.0f}, rot_w{0.0f}, rot_x{0.0f}, rot_y{0.0f}, rot_z{0.0f}, sca_x{1.0f}, sca_y{1.0f}, sca_z{1.0f};
    
    std::vector<float> to_arr() const;
};

/** OE_Object is the base class for all objects in a scene
 *  It is the basis for Mesh, Light, Camera, Particle etc.
 */

class OE_Object : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase{
    
    friend class OE_CSL_Writer;
    friend class OE_CSL_Interpreter;
    friend class NRE_Renderer;
    
    public:
        
        static std::unordered_map<std::size_t, std::string> id2name;
        static std::size_t current_id;
        static OE_Name2ID name2id;
        std::size_t id;
        
        OE_Object();
        OE_Object(const std::string&);
        virtual ~OE_Object();
        
        virtual std::string getType() const;
        virtual std::string to_str() const;
    
    //protected:
        OE_ObjectData       current_state;
        
        std::size_t         parent{0};
        std::size_t         parent_type;
        bool                visible{true};
        
};




#endif
