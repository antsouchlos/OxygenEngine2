#include "FE_EventHandler.h" //yes not InputEventHandler for a reason


FE_InputEventHandler::FE_InputEventHandler(){



}
FE_InputEventHandler::~FE_InputEventHandler(){}

vector<FE_Event*> FE_InputEventHandler::createEvents(){
	vector<FE_Event*> output;
	
	///generate keyboard events
	for(auto x: this->keyList){
		
		//event for just pressed (sent once per tap)
		FE_KeyboardEvent* event = new FE_KeyboardEvent();
		event->name = "keyboard-" + x.second + "+";
		event->key = x.second;
		event->setFunc(&template_event_func, nullptr);
		output.push_back(event);
		
		//event for just release (sent once per release)
		FE_KeyboardEvent* event1 = new FE_KeyboardEvent();
		event1->name = "keyboard-" + x.second + "-";
		event1->key = x.second;
		event1->setFunc(&template_event_func, nullptr);
		output.push_back(event1);
		
		//event for hold (sent every frame the key is being held)
		FE_KeyboardEvent* event2 = new FE_KeyboardEvent();
		event2->name = "keyboard-" + x.second + "";
		event2->key = x.second;
		event2->setFunc(&template_event_func, nullptr);
		output.push_back(event2);
	}
	
	///generate mouse events
	for(auto x: this->mouseList){
		//event for just pressed (sent once per tap)
		FE_MouseEvent* event = new FE_MouseEvent();
		event->name = "mouse-" + x.second + "+";
		event->key = x.second;
		event->setFunc(&template_event_func, nullptr);
		output.push_back(event);
		
		//event for just release (sent once per release)
		FE_MouseEvent* event1 = new FE_MouseEvent();
		event1->name = "mouse-" + x.second + "-";
		event1->key = x.second;
		event1->setFunc(&template_event_func, nullptr);
		output.push_back(event1);
		
		//event for hold (sent every frame the key is being held)
		FE_MouseEvent* event2 = new FE_MouseEvent();
		event2->name = "mouse-" + x.second + "";
		event2->key = x.second;
		event2->setFunc(&template_event_func, nullptr);
		output.push_back(event2);
	
	}
	///generate mouse motion event
	FE_MouseEvent* event3 = new FE_MouseEvent();
	event3->name = "mouse-motion";
	event3->key = "";
	event3->setFunc(&template_event_func, nullptr);
	output.push_back(event3);
	
	///generate mouse wheel event
	FE_MouseEvent* event4 = new FE_MouseEvent();
	event4->name = "mouse-wheel";
	event4->key = "";
	event4->setFunc(&template_event_func, nullptr);
	output.push_back(event4);
	return output;
}
bool FE_EventHandler::update(){
	
	this->updateInput();
	
	while(SDL_PollEvent(&this->event)){
		
		// exit before handling SDL events
		if(event.type == SDL_QUIT){ 
			done = true;
			return true;
		}
		
		updateInputEvents();
	
	
	}

	return false;
}

void FE_EventHandler::updateInput(){

	for(auto key: this->input_handler.keyList){
	
		FE_KeyboardEvent* just_pressed = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "+"));
		FE_KeyboardEvent* just_released = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "-"));
		FE_KeyboardEvent* held = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + ""));
		
		// if button is already pressed once, make it register continuous events
		if(just_pressed->keystate == FE_BUTTON::JUST_PRESS){
			
			just_pressed->keystate+=1;
			just_released->keystate+=1;
			held->keystate+=1;
		}
		else if(just_pressed->keystate == FE_BUTTON::PRESS){
		
			this->broadcastIEvent(held->name, nullptr);
		}// if button has just been released stop emitting events
		else if(just_pressed->keystate == FE_BUTTON::JUST_RELEASE){
			
			just_pressed->keystate=FE_BUTTON::RELEASE;
			just_released->keystate=FE_BUTTON::RELEASE;
			held->keystate=FE_BUTTON::RELEASE;
		
		}
		else{}
	}
	
	for(auto key: this->input_handler.mouseList){
	
		FE_MouseEvent* just_pressed = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + "+"));
		FE_MouseEvent* just_released = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + "-"));
		FE_MouseEvent* held = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + ""));
		
		// if button is already pressed once, make it register continuous events
		if(just_pressed->keystate == FE_BUTTON::JUST_PRESS){
			
			just_pressed->keystate+=1;
			just_released->keystate+=1;
			held->keystate+=1;
		}
		else if(just_pressed->keystate == FE_BUTTON::PRESS){
		
			this->broadcastIEvent(held->name, nullptr);
		}// if button has just been released stop emitting events
		else if(just_pressed->keystate == FE_BUTTON::JUST_RELEASE){
			
			just_pressed->keystate=FE_BUTTON::RELEASE;
			just_released->keystate=FE_BUTTON::RELEASE;
			held->keystate=FE_BUTTON::RELEASE;
		
		}
		else{}
	}
}
void FE_EventHandler::updateInputEvents(){
		
	switch(this->event.type){

		//check for key presses
		case SDL_KEYDOWN:

			for(auto key: this->input_handler.keyList){

				if (this->event.key.keysym.sym == key.first){

					FE_KeyboardEvent* just_pressed = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "+"));
					FE_KeyboardEvent* just_released = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "-"));
					FE_KeyboardEvent* held = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + ""));


				    if(just_pressed->keystate < FE_BUTTON::JUST_PRESS){
				    	just_pressed->keystate+=1;
				    	just_released->keystate+=1;
				    	held->keystate+=1;
				    }

					if(just_pressed->keystate == FE_BUTTON::JUST_PRESS) this->broadcastIEvent(just_pressed->name, nullptr);
					
				}			
			}
			break;

		// check for releases
        case SDL_KEYUP:

			for(auto key: this->input_handler.keyList){

				if (this->event.key.keysym.sym == key.first){
					
					FE_KeyboardEvent* just_pressed = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "+"));
					FE_KeyboardEvent* just_released = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "-"));
					FE_KeyboardEvent* held = static_cast<FE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + ""));
		
					
				    while(just_pressed->keystate < FE_BUTTON::JUST_RELEASE){
				    	just_pressed->keystate+=1;
				    	just_released->keystate+=1;
				    	held->keystate+=1;
				    }
					if(just_pressed->keystate == FE_BUTTON::JUST_RELEASE) this->broadcastIEvent(just_released->name, nullptr);
					else writeToLog("dafuq?"); ///IMPOSSIBLE
					
				}			
			}
			break;

		// update mouse position
        case SDL_MOUSEMOTION:
        	
        	//fetch mouse position, since this IS needed
        	SDL_GetMouseState(&FE_MouseEvent::x, &FE_MouseEvent::y);
        	SDL_GetRelativeMouseState(&FE_MouseEvent::delta_x, &FE_MouseEvent::delta_y);
        	//cout << "MOUSE_MOTION EVENt" << endl;
        	this->broadcastIEvent("mouse-motion", nullptr);
			
			break;
			
		// update mouse down events
        case SDL_MOUSEBUTTONDOWN:
        
        	for(auto key: this->input_handler.mouseList){
        	
        		if (this->event.button.button == key.first){
        		
        			FE_MouseEvent* just_pressed = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + "+"));
					FE_MouseEvent* just_released = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + "-"));
					FE_MouseEvent* held = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + ""));


					if(just_pressed->keystate < FE_BUTTON::PRESS){
					  	just_pressed->keystate+=1;
					   	just_released->keystate+=1;
					   	held->keystate+=1;
					}

					//fetch mouse position, since this may be needed
					SDL_GetMouseState(&FE_MouseEvent::x, &FE_MouseEvent::y);
        			SDL_GetRelativeMouseState(&FE_MouseEvent::delta_x, &FE_MouseEvent::delta_y);

					if(just_pressed->keystate == FE_BUTTON::JUST_PRESS) this->broadcastIEvent(just_pressed->name, nullptr);

				}
        	}
        	break;
        // update mouse up events
        case SDL_MOUSEBUTTONUP:
        
        	for(auto key: this->input_handler.mouseList){
        	
        		if (this->event.button.button == key.first){
        		
        			FE_MouseEvent* just_pressed = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + "+"));
					FE_MouseEvent* just_released = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + "-"));
					FE_MouseEvent* held = static_cast<FE_MouseEvent*>(getIEvent("mouse-" + key.second + ""));


					if(just_pressed->keystate < FE_BUTTON::JUST_RELEASE){
					  	just_pressed->keystate+=1;
					   	just_released->keystate+=1;
					   	held->keystate+=1;
					}

					//fetch mouse position, since this may be needed
					SDL_GetMouseState(&FE_MouseEvent::x, &FE_MouseEvent::y);
        			SDL_GetRelativeMouseState(&FE_MouseEvent::delta_x, &FE_MouseEvent::delta_y);

					if(just_pressed->keystate == FE_BUTTON::JUST_RELEASE) this->broadcastIEvent(just_released->name, nullptr);
					else writeToLog("dafuq?"); ///IMPOSSIBLE
				}
        	}
        	
        	break;
        
        // update mouse wheel events
        case SDL_MOUSEWHEEL:
        
        	FE_MouseEvent::mouse_wheel = event.wheel.y;
        	
        	//fetch mouse position, since this may be needed
			SDL_GetMouseState(&FE_MouseEvent::x, &FE_MouseEvent::y);
			SDL_GetRelativeMouseState(&FE_MouseEvent::delta_x, &FE_MouseEvent::delta_y);

        	this->broadcastIEvent("mouse-wheel", nullptr);
        	break;
	}
}
