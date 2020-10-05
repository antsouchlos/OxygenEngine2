#ifndef OE_TEXTURE_H
#define OE_TEXTURE_H

#include <types/OE_TypesBase.h>

/* OE_Texture represents a simple texture.
 * It can be assigned a camera or an image or video as the source
 * Or even arbitrary data
 */
class OE_Texture: public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase{

    friend class CSL_Interpreter;

    public:
        static std::map<std::size_t, std::string> id2name;
        static std::size_t current_id;
        static OE_Name2ID name2id;
        std::size_t id;
        
        OE_Texture();
        OE_Texture(const std::string&);
        ~OE_Texture();
        
        std::string to_str();

    protected:
        int         source;
        std::string path;
        std::size_t camera;
        void*       custom_data;
        
    private:
};

#endif 
