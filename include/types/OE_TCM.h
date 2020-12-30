#ifndef OE_TCM_H
#define OE_TCM_H

#include <types/OE_Texture.h>

class OE_TCM_Texture: public CSL_WriterBase{
    public:
        OE_TCM_Texture();
        ~OE_TCM_Texture();
        
        std::size_t textureID   {0};
        int mode                {0};
        int textureMulFactor    {1};
        std::size_t uvmap       {1};
    
        std::string to_str() const;
};

/* OE_TCM (TCM: Texture Combine Mode) is responsible for combining different types of textures 
 * into one single entity, for example combining multiple textures from different sources
 * to form a stencil. OE_TCM also dictates the usage of the textures, such as as diffuse/normal/specular maps etc.
 */

class OE_TCM : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase{
    
//    friend class CSL_Interpreter;
    
    public:
        static std::atomic<std::size_t> current_id;
        std::size_t id;
        
        OE_TCM();
        
        std::string to_str() const;
        
//    protected:
        std::vector<OE_TCM_Texture>    textures;
        
        int         combine_mode;
        bool        texture_array;
        float       r, g, b, a;
    
};


#endif
