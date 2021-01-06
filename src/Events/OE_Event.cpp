#include <Events/OE_Event.h>
#include <Carbon/CSL_Interpreter.h>

using namespace std;
 
bool OE_Event::finished = false;

OE_Event::OE_Event(){
    active_=false;
    name_ = "";
}

OE_Event::~OE_Event(){}

void OE_Event::setFunc(const OE_EVENTFUNC a_func){
    lockMutex();
    func_ = a_func;
    unlockMutex();
}

int OE_Event::internal_call(){
	/***************************/
	///generic handling
	
	if (!this->has_init_){
        
        this->task_ = OE_Task(this->name_, 0, 0, SDL_GetTicks());
        this->has_init_ = true;
    }
	
	task_.update();
	try {
        func_(task_, name_);
    }
    catch(oe::api_error& e){
        std::string error_str = "OE: " + e.name_ + " thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter()) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(csl::parser_error& e){
        std::string error_str = "OE: " + e.name_ + " thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter()) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(csl::interpreter_error& e){
        std::string error_str = "OE: " + e.name_ + " thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter()) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(...){
        std::string error_str = "OE: Exception thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter());
        cout << error_str << endl;
        OE_WriteToLog(error_str  + "\n");
    }
    return 0;
    /**************************/
}
//keyboard
OE_KeyboardEvent::OE_KeyboardEvent(){
    type_ = OE_KEYBOARD_EVENT;
    keystate = OE_BUTTON::RELEASE;
}
OE_KeyboardEvent::~OE_KeyboardEvent(){}

int OE_KeyboardEvent::call(){
	
	return internal_call();
}

//mouse
int OE_MouseEvent::x = 0;
int OE_MouseEvent::y = 0;
int OE_MouseEvent::delta_x = 0;
int OE_MouseEvent::delta_y = 0;
int OE_MouseEvent::mouse_wheel = 0;
bool OE_MouseEvent::mousemoved = false;

OE_MouseEvent::OE_MouseEvent(){
    type_ = OE_MOUSE_EVENT;
    mousemoved = false;
    keystate = OE_BUTTON::RELEASE;
}
OE_MouseEvent::~OE_MouseEvent(){}

int OE_MouseEvent::call(){
	
	return internal_call();
}

//gamepad
OE_GamepadEvent::OE_GamepadEvent(){
    type_ = OE_GAMEPAD_EVENT;
    axis=0; axismoved = false;
}
OE_GamepadEvent::~OE_GamepadEvent(){}

int OE_GamepadEvent::call(){
    
    return internal_call();
}

//custom
OE_CustomEvent::OE_CustomEvent(){
    type_ = OE_CUSTOM_EVENT;
}
OE_CustomEvent::~OE_CustomEvent(){}

int OE_CustomEvent::call(){

	return internal_call();
}

//error
OE_ErrorEvent::OE_ErrorEvent(){
    type_ = OE_ERROR_EVENT;
}
OE_ErrorEvent::~OE_ErrorEvent(){}

int OE_ErrorEvent::call(){
	
	/***************************/
	///non-generic handling
	if( this->importance == OE_FATAL) finished = true;
	
    this->internal_call();
    
    return 0;
}

//multiple events
OE_EventCombo::OE_EventCombo(){
    type_ = OE_EVENT_COMBO;
}
OE_EventCombo::~OE_EventCombo(){}

int OE_EventCombo::call(){
	
	return internal_call();
}
