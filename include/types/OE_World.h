#ifndef OE_WORLD_H
#define OE_WORLD_H

#include <types/OE_Scene.h>
#include <types/OE_ViewportConfig.h>

class OE_World : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase {
    
    friend class CSL_Interpreter;
    friend class NRE_Renderer;
    
    public:    
        OE_World();
        ~OE_World();
        
        std::string to_str() const;
        
    //protected:
        
        std::size_t                                         loaded_scene;
        std::map<std::size_t, OE_Scene*>          scenes;
        
        std::size_t                                         loaded_viewport;
        std::map<std::size_t, OE_ViewportConfig*> viewports;                
};

#endif
