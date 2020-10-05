#ifndef OE_SCENE_H
#define OE_SCENE_H

#include <types/OE_Camera.h>
#include <types/OE_Light.h>
#include <types/OE_Mesh.h>
#include <types/OE_Material.h>
#include <types/OE_Texture.h>
#include <types/OE_TCM.h>

/* OE_Scene is a collection of objects (meshes, lights, cameras, etc.)
 * textures, materials, TCMs etc.
 */

class OE_Scene : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase {
    
    friend class CSL_Interpreter;
    
    public:
        
        static std::map<std::size_t, std::string> id2name;
        static std::size_t current_id;
        static OE_Name2ID name2id;
        std::size_t id;
        
        OE_Scene();
        OE_Scene(const std::string&);
        ~OE_Scene();
        
        std::string to_str();
        
    protected:
        
        std::map<std::size_t, OE_Object*>       objects;
        std::map<std::size_t, OE_Texture*>      textures;
        std::map<std::size_t, OE_Material*>     materials;
        std::map<std::size_t, OE_TCM*>          texture_CMs;    
};



#endif
