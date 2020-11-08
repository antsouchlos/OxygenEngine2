#ifndef OE_VIEWPORTCONFIG_H
#define OE_VIEWPORTCONFIG_H

#include <types/OE_TypesBase.h>

/* OE_ViewportConfig determines how different cameras and scenes
 * are projected into the screen
 */

class OE_ViewportConfig : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase{
    
    friend class CSL_Interpreter;
    
    public:
        static std::size_t current_id;
        std::size_t id;
        
        
        OE_ViewportConfig();
        OE_ViewportConfig(const std::string&);
        
        ~OE_ViewportConfig();
        
        std::string to_str() const;
        
    protected:
        // for each camera
        std::vector<int>            layers;
        std::vector<std::size_t>    cameras;
        std::vector<int>            camera_modes;
        
        // for each layer
        std::vector<int>            layer_combine_modes;
        std::vector<float>          split_screen_positions;
        
};

#endif
