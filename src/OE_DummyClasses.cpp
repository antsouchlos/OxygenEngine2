#include <OE_DummyClasses.h>
#include <OE_API.h>

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

bool OE_WindowSystemBase::getMouseLockedState(){return mouse_locked;}
void OE_WindowSystemBase::lockMouse(){}
void OE_WindowSystemBase::unlockMouse(){}

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
    //throw 5;
    return true;
}

void OE_PhysicsEngineBase::destroy(){
    return;
}

OE_NetworkingBase::OE_NetworkingBase(){}
OE_NetworkingBase::~OE_NetworkingBase(){}
    
void OE_NetworkingBase::init(){}
    
int OE_NetworkingBase::execute(OE_Task){
    
    //int a =0;
    
    while(!done){
        // manage networking stuff, use your own threads etc.
        // YOU control the loop. Upon a call on this->destroy() it should stop though
        // use oe::create_event() and oe::broadcastEvent() to communicate with the engine 
        //cout << "OE_Networking is running" << ++a << endl;
        oe::pause(10);
    }
    return 0;
    
}
void OE_NetworkingBase::destroy(){
    done = true;
}
