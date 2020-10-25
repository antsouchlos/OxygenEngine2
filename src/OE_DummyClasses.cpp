#include <OE_DummyClasses.h>

// Dummy definitions of the class methods
// so that the classes can stand on their own and be used
// for testing purposes

using namespace std;

OE_WindowSystemBase::OE_WindowSystemBase(){
    
}

OE_WindowSystemBase::~OE_WindowSystemBase(){

}

bool OE_WindowSystemBase::init(int x, int y, string titlea, bool isFullscreen, void* data){
    return true;
}

bool OE_WindowSystemBase::update(){
    return true;
}

bool OE_WindowSystemBase::updateEvents(){
    return true;
}

void OE_WindowSystemBase::destroy(){
    
}


OE_RendererBase::OE_RendererBase(){
    
}

OE_RendererBase::~OE_RendererBase(){
    
}
    
bool OE_RendererBase::init(){
    return true;
}

bool OE_RendererBase::updateSingleThread(){
    return true;
}

bool OE_RendererBase::updateData(){
    return true;
}
    
bool OE_RendererBase::updateMultiThread(OE_Task*, int){
    return true;
}

void OE_RendererBase::destroy(){
}
    
    
OE_PhysicsEngineBase::OE_PhysicsEngineBase(){
    
}

OE_PhysicsEngineBase::~OE_PhysicsEngineBase(){
    
}
    
bool OE_PhysicsEngineBase::init(){
    return true;
}

bool OE_PhysicsEngineBase::updateMultiThread(OE_Task* task, int thread_num){
    //cout << task->GetElapsedTime();-
    return true;
}

void OE_PhysicsEngineBase::destroy(){
    return;
}
