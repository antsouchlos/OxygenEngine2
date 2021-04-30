#ifndef OE_CAMERA_H
#define OE_CAMERA_H

#include <types/OE_Object.h>

/* OE_Camera represents a scene camera
 */

class OE_Camera : public OE_Object {
    
    friend class CSL_Interpreter;
    friend class NRE_Renderer;

    public:
        OE_Camera();
        OE_Camera(const std::string&);
        ~OE_Camera();
        
        OE_OBJECT_TYPE getType() const;
        std::string to_str() const;
        
//    protected:
        float aspect_ratio;
        float fov;

        int near;
        int far;
};

#endif
