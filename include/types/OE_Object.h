#ifndef OE_OBJECT_H
#define OE_OBJECT_H

#include <types/OE_TypesBase.h>
#include <OE_Math.h>

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
        
        static std::size_t current_id;
        std::size_t id;
        
        OE_Object();
        OE_Object(const std::string&);
        virtual ~OE_Object();
        
        virtual std::string getType() const;
        virtual std::string to_str() const;
        
        OE_Quat GetRot();
        void SetRot(OE_Quat);
        
        OE_Mat4x4 GetModelMatrix();
        OE_Mat4x4 GetViewMatrix();
        
    //protected:
        OE_ObjectData       current_state;
        
        std::size_t         parent{0};
        std::size_t         parent_type;
        bool                visible{true};
        
};




#endif
