#include <Renderer/NRE_RendererMain.h>

NRE_Renderer::NRE_Renderer(){
    
}

NRE_Renderer::~NRE_Renderer(){
    
}
    
bool NRE_Renderer::init(){
    return true;
}

bool NRE_Renderer::updateSingleThread(){
    
    return true;
}

bool NRE_Renderer::updateData(){
    
    return true;
}
    
bool NRE_Renderer::updateMultiThread(OE_Task*, int){
    
    return false;
}

void NRE_Renderer::destroy(){
    
}    
