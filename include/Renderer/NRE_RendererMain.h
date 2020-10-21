#ifndef NRE_RENDERERMAIN_H
#define NRE_RENDERERMAIN_H

#include <Renderer/NRE_RenderData.h>
#include <OE_DummyClasses.h>

class NRE_Renderer : public OE_RendererBase{
public:
    NRE_Renderer();
    ~NRE_Renderer();
    
    bool init();
    bool updateSingleThread();
    bool updateData();
    
    bool updateMultiThread(OE_Task*, int);
    void destroy();    
};



#endif
