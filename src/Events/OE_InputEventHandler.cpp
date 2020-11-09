#include <Events/OE_EventHandler.h> //yes not InputEventHandler for a reason

using namespace std;

OE_InputEventHandler::OE_InputEventHandler(){



}
OE_InputEventHandler::~OE_InputEventHandler(){}

void OE_InputEventHandler::createEvents(std::map<std::string, std::shared_ptr<OE_Event>>* event_list){


    ///generate keyboard events
    for(auto x: this->keyList){
        
        //event for just pressed (sent once per tap)
        shared_ptr<OE_KeyboardEvent> event = std::make_shared<OE_KeyboardEvent>();
        event->name = "keyboard-" + x.second + "+";
        event->key = x.second;
        event->setFunc(&template_event_func, nullptr);
        event_list[0][event->name] =event;
        
        //event for just release (sent once per release)
        shared_ptr<OE_KeyboardEvent> event1 = std::make_shared<OE_KeyboardEvent>();
        event1->name = "keyboard-" + x.second + "-";
        event1->key = x.second;
        event1->setFunc(&template_event_func, nullptr);
        event_list[0][event1->name] =event1;
        
        //event for hold (sent every frame the key is being held)
        shared_ptr<OE_KeyboardEvent> event2 = std::make_shared<OE_KeyboardEvent>();
        event2->name = "keyboard-" + x.second + "";
        event2->key = x.second;
        event2->setFunc(&template_event_func, nullptr);
        event_list[0][event2->name] =event2;
    }
    
    ///generate mouse events
    for(auto x: this->mouseList){
        //event for just pressed (sent once per tap)
        shared_ptr<OE_MouseEvent> event = std::make_shared<OE_MouseEvent>();
        event->name = "mouse-" + x.second + "+";
        event->key = x.second;
        event->setFunc(&template_event_func, nullptr);
        event_list[0][event->name] =event;
        
        //event for just release (sent once per release)
        shared_ptr<OE_MouseEvent> event1 = std::make_shared<OE_MouseEvent>();
        event1->name = "mouse-" + x.second + "-";
        event1->key = x.second;
        event1->setFunc(&template_event_func, nullptr);
        event_list[0][event1->name] =event1;
        
        //event for hold (sent every frame the key is being held)
        shared_ptr<OE_MouseEvent> event2 = std::make_shared<OE_MouseEvent>();
        event2->name = "mouse-" + x.second + "";
        event2->key = x.second;
        event2->setFunc(&template_event_func, nullptr);
        event_list[0][event2->name] =event2;
    
    }
    ///generate mouse motion event
    shared_ptr<OE_MouseEvent> event3 = std::make_shared<OE_MouseEvent>();
    event3->name = "mouse-motion";
    event3->key = "";
    event3->setFunc(&template_event_func, nullptr);
    event_list[0][event3->name] =event3;

    ///generate mouse wheel event
    shared_ptr<OE_MouseEvent> event4 = std::make_shared<OE_MouseEvent>();
    event4->name = "mouse-wheel";
    event4->key = "";
    event4->setFunc(&template_event_func, nullptr);
    event_list[0][event4->name] =event4;
}

bool OE_EventHandler::update(){
    
    this->updateInput();
    
    while(SDL_PollEvent(&this->event)){
        
        // exit before handling SDL events
        if(event.type == SDL_QUIT){ 
            done = true;
            return done;
        }
        updateInputEvents();
    }
    // This is needed to support things like OE_Finish()
    return done;
}

void OE_EventHandler::updateInput(){

    for(auto key: this->input_handler.keyList){
    
        OE_KeyboardEvent* just_pressed = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "+").get());
        OE_KeyboardEvent* just_released = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "-").get());
        OE_KeyboardEvent* held = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "").get());
        
        // if button is already pressed once, make it register continuous events
        if(just_pressed->keystate == OE_BUTTON::JUST_PRESS){
            
            just_pressed->keystate+=1;
            just_released->keystate+=1;
            held->keystate+=1;
        }
        else if(just_pressed->keystate == OE_BUTTON::PRESS){
        
            this->broadcastIEvent(held->name, nullptr);
        }// if button has just been released stop emitting events
        else if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE){
            
            just_pressed->keystate=OE_BUTTON::RELEASE;
            just_released->keystate=OE_BUTTON::RELEASE;
            held->keystate=OE_BUTTON::RELEASE;
        
        }
        else{}
    }
    
    for(auto key: this->input_handler.mouseList){
    
        OE_MouseEvent* just_pressed = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "+").get());
        OE_MouseEvent* just_released = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "-").get());
        OE_MouseEvent* held = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "").get());
        
        // if button is already pressed once, make it register continuous events
        if(just_pressed->keystate == OE_BUTTON::JUST_PRESS){
            
            just_pressed->keystate+=1;
            just_released->keystate+=1;
            held->keystate+=1;
        }
        else if(just_pressed->keystate == OE_BUTTON::PRESS){
        
            this->broadcastIEvent(held->name, nullptr);
        }// if button has just been released stop emitting events
        else if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE){
            
            just_pressed->keystate=OE_BUTTON::RELEASE;
            just_released->keystate=OE_BUTTON::RELEASE;
            held->keystate=OE_BUTTON::RELEASE;
        
        }
        else{}
    }
}

void OE_EventHandler::updateInputEvents(){
    
    switch(this->event.type){

        //check for key presses
        case SDL_KEYDOWN:

            for(auto key: this->input_handler.keyList){

                if (this->event.key.keysym.sym == key.first){

                    OE_KeyboardEvent* just_pressed = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "+").get());
                    OE_KeyboardEvent* just_released = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "-").get());
                    OE_KeyboardEvent* held = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "").get());


                    if(just_pressed->keystate < OE_BUTTON::JUST_PRESS){
                        just_pressed->keystate+=1;
                        just_released->keystate+=1;
                        held->keystate+=1;
                    }

                    if(just_pressed->keystate == OE_BUTTON::JUST_PRESS) this->broadcastIEvent(just_pressed->name, nullptr);

                }
            }
            break;

        // check for releases
        case SDL_KEYUP:

			for(auto key: this->input_handler.keyList){

				if (this->event.key.keysym.sym == key.first){
					
					OE_KeyboardEvent* just_pressed = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "+").get());
					OE_KeyboardEvent* just_released = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "-").get());
					OE_KeyboardEvent* held = static_cast<OE_KeyboardEvent*>(getIEvent("keyboard-" + key.second + "").get());
		
					
				    while(just_pressed->keystate < OE_BUTTON::JUST_RELEASE){
				    	just_pressed->keystate+=1;
				    	just_released->keystate+=1;
				    	held->keystate+=1;
				    }
					if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE) this->broadcastIEvent(just_released->name, nullptr);
					else OE_WriteToLog("dafuq?"); ///IMPOSSIBLE
					
				}			
			}
			break;

		// update mouse position
        case SDL_MOUSEMOTION:
        	
        	//fetch mouse position, since this IS needed
        	SDL_GetMouseState(&OE_MouseEvent::x, &OE_MouseEvent::y);

        	SDL_GetRelativeMouseState(&OE_MouseEvent::delta_x, &OE_MouseEvent::delta_y);
        	//cout << "MOUSE_MOTION EVENt" << endl;
        	this->broadcastIEvent("mouse-motion", nullptr);
			
			break;
			
		// update mouse down events
        case SDL_MOUSEBUTTONDOWN:
        
        	for(auto key: this->input_handler.mouseList){
        	
        		if (this->event.button.button == key.first){
        		
        			OE_MouseEvent* just_pressed = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "+").get());
					OE_MouseEvent* just_released = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "-").get());
					OE_MouseEvent* held = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "").get());


					if(just_pressed->keystate < OE_BUTTON::PRESS){
					  	just_pressed->keystate+=1;
					   	just_released->keystate+=1;
					   	held->keystate+=1;
					}

					//fetch mouse position, since this may be needed
					SDL_GetMouseState(&OE_MouseEvent::x, &OE_MouseEvent::y);
        			SDL_GetRelativeMouseState(&OE_MouseEvent::delta_x, &OE_MouseEvent::delta_y);

					if(just_pressed->keystate == OE_BUTTON::JUST_PRESS) this->broadcastIEvent(just_pressed->name, nullptr);

				}
        	}
        	break;
            
        // update mouse up events
        case SDL_MOUSEBUTTONUP:
        
            for(auto key: this->input_handler.mouseList){
            
                if (this->event.button.button == key.first){
                
                    OE_MouseEvent* just_pressed = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "+").get());
                    OE_MouseEvent* just_released = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "-").get());
                    OE_MouseEvent* held = static_cast<OE_MouseEvent*>(getIEvent("mouse-" + key.second + "").get());


                    if(just_pressed->keystate < OE_BUTTON::JUST_RELEASE){
                        just_pressed->keystate+=1;
                        just_released->keystate+=1;
                        held->keystate+=1;
                    }

                    //fetch mouse position, since this may be needed
                    SDL_GetMouseState(&OE_MouseEvent::x, &OE_MouseEvent::y);
                    SDL_GetRelativeMouseState(&OE_MouseEvent::delta_x, &OE_MouseEvent::delta_y);

                    if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE) this->broadcastIEvent(just_released->name, nullptr);
                    else OE_WriteToLog("dafuq?"); ///IMPOSSIBLE
                }
            }
            
            break;
        
        // update mouse wheel events
        case SDL_MOUSEWHEEL:
        
            OE_MouseEvent::mouse_wheel = event.wheel.y;
            
            //fetch mouse position, since this may be needed
            SDL_GetMouseState(&OE_MouseEvent::x, &OE_MouseEvent::y);
            SDL_GetRelativeMouseState(&OE_MouseEvent::delta_x, &OE_MouseEvent::delta_y);

            this->broadcastIEvent("mouse-wheel", nullptr);
            break;
	}
}
