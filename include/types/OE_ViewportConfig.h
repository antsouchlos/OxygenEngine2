#ifndef OE_VIEWPORTCONFIG_H
#define OE_VIEWPORTCONFIG_H

#include <types/OE_TypesBase.h>

/* OE_ViewportConfig determines how different cameras and scenes
 * are projected into the screen
 */

class OE_ViewportConfig : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase{
    
    friend class CSL_Interpreter;
    
    public:
        static std::atomic<std::size_t> current_id;
        std::size_t id;
        
        
        OE_ViewportConfig();
        OE_ViewportConfig(const std::string&);
        
        ~OE_ViewportConfig();
        
        void addLayer();
        void addCamera(std::size_t, std::size_t);
        void removeCamera(std::size_t, std::size_t);
        void removeCamera(std::size_t);
        void removeLayer(std::size_t);
        
        void setCameraMode(std::size_t, int);
        void setCameraLayerMode(std::size_t, std::size_t, int);
        
        void setLayerCombineMode(std::size_t, int);
        void setLayerSplitScreenPosHorizontal(std::size_t, float);
        void setLayerSplitScreenPosVertical(std::size_t, float);
        
        bool existsLayer(std::size_t);
        bool existsCamera(std::size_t);
        bool existsCameraLayer(std::size_t, std::size_t);
        
        
        std::string to_str() const;
        
//    protected:
        
        /** for each camera ************************/
        std::vector<std::size_t>    layers;
        std::vector<std::size_t>    cameras;
        /*
         * 0       : ignore
         * 1       : whole screen
         * 2/3     : left/right split screen
         * 4/5/6/7 : upper-left/upper-right/down-left/down-right split screen
         */
        std::vector<int>            camera_modes; 
        
        /** for each layer  ************************/
        // 0 : ignore, 1 : add (for now)
        std::vector<int>            layer_combine_modes;
        // two floats for each layer, but only useful when there is a split screen in eye coordinates
        std::vector<float>          split_screen_positions;
        
};

#endif
