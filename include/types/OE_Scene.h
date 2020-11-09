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
        
        static std::atomic<std::size_t> current_id;
        std::size_t id;
        
        OE_Scene();
        OE_Scene(const std::string&);
        ~OE_Scene();
        
        std::string to_str();
        
    //protected:
        
        //std::unordered_map<std::size_t, std::shared_ptr<OE_Object>>       objects;
        
        //std::unordered_map<std::size_t, std::shared_ptr<OE_Texture>>      textures;
        //std::unordered_map<std::size_t, std::shared_ptr<OE_Material>>     materials;
        //std::unordered_map<std::size_t, std::shared_ptr<OE_TCM>>          texture_CMs;    
        std::set<std::size_t> objects;
        std::set<std::size_t> materials;
        std::set<std::size_t> textures;
        std::set<std::size_t> texture_CMs;
};



#endif
