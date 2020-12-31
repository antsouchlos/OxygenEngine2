#ifndef OE_MATERIAL_H
#define OE_MATERIAL_H

#include <types/OE_TCM.h>

class OE_Material : public OE_THREAD_SAFETY_OBJECT, public CSL_WriterBase{
    
    friend class CSL_Interpreter;
    friend class NRE_Renderer;
    
    public:
        static std::atomic<std::size_t> current_id;
        std::size_t id;
        
        
        OE_Material();
        OE_Material(const std::string&);
        ~OE_Material();
        
        std::string to_str();
    
//    protected:
        float dif_r{0.0f}, dif_g{0.0f}, dif_b{0.0f}, dif_a{1.0f};
        float scol_r{0.0f}, scol_g{0.0f}, scol_b{0.0f};
        
        float alpha{1.0f}, translucency{0.0f}, illuminosity{0.0f};
        float specular_intensity{1.0f}, specular_hardness{1.0f};
        
        std::vector<std::size_t> textureCM_IDs;
        
        std::vector<float> GetRendererData();
};


#endif
