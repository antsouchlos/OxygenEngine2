#include <Events/OE_Event.h>
 
using namespace std;
 
bool OE_Event::finished = false;

OE_Event::OE_Event(){
    active=false;
    name = "";
    //condition = SDL_CreateCond();
    times_invoked = 0;

}
OE_Event::~OE_Event(){}

void OE_Event::setFunc(const OE_EVENTFUNC a_func, void* data){
    lockMutex();
    func = a_func;
    user_data = data;
    unlockMutex();
}

void OE_Event::setFuncData(void* data){
    lockMutex();
	if(data != nullptr)
		user_data = data;
    unlockMutex();
}
int OE_Event::internal_call(OE_Task* task, void* data){
	/***************************/
	///generic handling
	
	if( data == nullptr)
    	func(user_data, task, name);
    else{
    	user_data = data;
        func(user_data, task, name);
        
    }
    return 0;
    /**************************/
}
//keyboard
OE_KeyboardEvent::OE_KeyboardEvent(){
    type = OE_KEYBOARD_EVENT;
    keystate = OE_BUTTON::RELEASE;
}
OE_KeyboardEvent::~OE_KeyboardEvent(){}

int OE_KeyboardEvent::call(OE_Task* task, void* data){
	
	return internal_call(task, data);
}

//mouse
int OE_MouseEvent::x = 0;
int OE_MouseEvent::y = 0;
int OE_MouseEvent::delta_x = 0;
int OE_MouseEvent::delta_y = 0;
int OE_MouseEvent::mouse_wheel = 0;
bool OE_MouseEvent::mousemoved = false;

OE_MouseEvent::OE_MouseEvent(){
    type = OE_MOUSE_EVENT;
    mousemoved = false;
    keystate = OE_BUTTON::RELEASE;
}
OE_MouseEvent::~OE_MouseEvent(){}

int OE_MouseEvent::call(OE_Task* task, void* data){
	
	return internal_call(task, data);
}

//gamepad
OE_GamepadEvent::OE_GamepadEvent(){
    type = OE_GAMEPAD_EVENT;
    axis=0; axismoved = false;
}
OE_GamepadEvent::~OE_GamepadEvent(){}

int OE_GamepadEvent::call(OE_Task* task, void* data){
    
    return internal_call(task, data);
}

//custom
OE_CustomEvent::OE_CustomEvent(){
    type = OE_CUSTOM_EVENT;
}
OE_CustomEvent::~OE_CustomEvent(){}

int OE_CustomEvent::call(OE_Task* task, void* data){

	return internal_call(task, data);
}

//error
OE_ErrorEvent::OE_ErrorEvent(){
    type = OE_ERROR_EVENT;
}
OE_ErrorEvent::~OE_ErrorEvent(){}

int OE_ErrorEvent::call(OE_Task* task, void* data){
	
	/***************************/
	///non-generic handling
	if( this->importance == OE_FATAL) finished = true;
	
    if( data == nullptr)
    	func(user_data, task, name);
    else{
        user_data = data;
        func(user_data, task, name);
    }
    
    return 0;
}

//multiple events
OE_EventCombo::OE_EventCombo(){
    type = OE_EVENT_COMBO;
}
OE_EventCombo::~OE_EventCombo(){}

int OE_EventCombo::call(OE_Task* task, void* data){
	
	return internal_call(task, data);
}
