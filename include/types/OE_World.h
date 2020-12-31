#ifndef OE_WORLD_H
#define OE_WORLD_H

#include <types/OE_Scene.h>
#include <types/OE_ViewportConfig.h>

class OE_World : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase {
    
    friend class CSL_Interpreter;
    friend class NRE_Renderer;
    
    public:    
        
        static OE_SharedIndexMap<OE_Scene>          scenesList;
        static OE_SharedIndexMap<OE_Object>         objectsList;
        static OE_SharedIndexMap<OE_Material>       materialsList;
        static OE_SharedIndexMap<OE_Texture>        texturesList;
        static OE_SharedIndexMap<OE_TCM>            tcmsList;
        static OE_SharedIndexMap<OE_ViewportConfig> viewportsList;
        
        
        
        OE_World();
        ~OE_World();
        
        std::string to_str() const;
        
    //protected:
        
        std::size_t                                         loaded_scene;
        
        std::set<std::size_t> scenes;
        std::set<std::size_t> viewports;
        std::size_t                                         loaded_viewport;
                      
};

#endif
