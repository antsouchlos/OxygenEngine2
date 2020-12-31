#ifndef OE_LIGHT_H
#define OE_LIGHT_H

#include <types/OE_Object.h>

//TODO: Subject to change

struct OE_RGBColor{
    float r{0.0f}, g{0.0f}, b{0.0f};
    
    std::vector<float> to_arr() const;
};

/* OE_Light represents a light in the scene, that can be directional, point or sun.
 */

class OE_Light : public OE_Object{
    
    friend class CSL_Interpreter;
    
    public:
        OE_Light();
        OE_Light(const std::string&);
        ~OE_Light();
        
        std::string getType() const;
        std::string to_str() const;
        
    protected:
        std::size_t                 light_type;
        std::vector<std::size_t>    objects;
        OE_RGBColor                 color;
        float                       intensity;
        float                       fov, range;
};



#endif
