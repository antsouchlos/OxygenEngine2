#ifndef OE_CAMERA_H
#define OE_CAMERA_H

#include <types/OE_Object.h>

/* OE_Camera represents a scene camera
 */

class OE_Camera : public OE_Object {
    
//    friend class Interpreter;
    friend class NRE_Renderer;

    public:
        OE_Camera();
        ~OE_Camera();
        
        std::string getType() const;
        std::string to_str() const;
        
//    protected:
        float aspect_ratio;
        float fov;

        int near;
        int far;
};

#endif
