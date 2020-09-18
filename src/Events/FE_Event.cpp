#include "FE_Event.h"
 
bool FE_Event::finished = false;

FE_Event::FE_Event(){
    active=false;
    name = "";
    condition = SDL_CreateCond();
    occupied = 0;

}
FE_Event::~FE_Event(){}

void FE_Event::setFunc(const FE_EVENTFUNC a_func, void* data){
    func = a_func;
    user_data = data;
}

void FE_Event::setFuncData(void* data){
	if(data != nullptr)
		user_data = data;
}
int FE_Event::internal_call(FTask* task, void* data){
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
FE_KeyboardEvent::FE_KeyboardEvent(){
    type = FE_KEYBOARD_EVENT;
    keystate = FE_BUTTON::RELEASE;
}
FE_KeyboardEvent::~FE_KeyboardEvent(){}

int FE_KeyboardEvent::call(FTask* task, void* data){
	
	return internal_call(task, data);
}

//mouse
int FE_MouseEvent::x = 0;
int FE_MouseEvent::y = 0;
int FE_MouseEvent::delta_x = 0;
int FE_MouseEvent::delta_y = 0;
int FE_MouseEvent::mouse_wheel = 0;
bool FE_MouseEvent::mousemoved = false;

FE_MouseEvent::FE_MouseEvent(){
    type = FE_MOUSE_EVENT;
    mousemoved = false;
    keystate = FE_BUTTON::RELEASE;
}
FE_MouseEvent::~FE_MouseEvent(){}

int FE_MouseEvent::call(FTask* task, void* data){
	
	return internal_call(task, data);
}

//gamepad
FE_GamepadEvent::FE_GamepadEvent(){
    type = FE_GAMEPAD_EVENT;
    axis=0; axismoved = false;
}
FE_GamepadEvent::~FE_GamepadEvent(){}

int FE_GamepadEvent::call(FTask* task, void* data){
    
    return internal_call(task, data);
}

//custom
FE_CustomEvent::FE_CustomEvent(){
    type = FE_CUSTOM_EVENT;
}
FE_CustomEvent::~FE_CustomEvent(){}

int FE_CustomEvent::call(FTask* task, void* data){

	return internal_call(task, data);
}

//error
FE_ErrorEvent::FE_ErrorEvent(){
    type = FE_ERROR_EVENT;
}
FE_ErrorEvent::~FE_ErrorEvent(){}

int FE_ErrorEvent::call(FTask* task, void* data){
	
	/***************************/
	///non-generic handling
	if( this->importance == FE_FATAL) finished = true;
	
    if( data == nullptr)
    	func(user_data, task, name);
    else{
        user_data = data;
        func(user_data, task, name);
    }
    
    return 0;
}

//multiple events
FE_EventCombo::FE_EventCombo(){
    type = FE_EVENT_COMBO;
}
FE_EventCombo::~FE_EventCombo(){}

int FE_EventCombo::call(FTask* task, void* data){
	
	return internal_call(task, data);
}
