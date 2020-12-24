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
        }
        // if button has just been released stop emitting events
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
        }
        // if button has just been released stop emitting events
        else if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE){
            just_pressed->keystate=OE_BUTTON::RELEASE;
            just_released->keystate=OE_BUTTON::RELEASE;
            held->keystate=OE_BUTTON::RELEASE;
        }
        else{}
    }
    //this->mouse_moved = false;
}

void OE_EventHandler::internalBroadcastKeyDownEvent(const std::string &name){
    
    if (name.length() <= 8)
        assert (name.substr(0, 6) == "mouse-");
    else if (name.length() > 8)
        assert (name.substr(0, 6) == "mouse-" || name.substr(0, 9) == "keyboard-");
    else{
        cout << "Error invalid name in internalBroadcastKeyDwonEvent: " << name << endl;
        assert(false);
    }
    
    // update mouse event if it exists
    if (name.substr(0, 6) == "mouse-"){
        
        OE_MouseEvent* just_pressed = static_cast<OE_MouseEvent*>(getIEvent(name + "+").get());
        OE_MouseEvent* just_released = static_cast<OE_MouseEvent*>(getIEvent(name + "-").get());
        OE_MouseEvent* held = static_cast<OE_MouseEvent*>(getIEvent(name + "").get());
        
        if(just_pressed->keystate < OE_BUTTON::PRESS){
            just_pressed->keystate+=1;
            just_released->keystate+=1;
            held->keystate+=1;
        }
        if(just_pressed->keystate == OE_BUTTON::JUST_PRESS) this->broadcastIEvent(just_pressed->name, nullptr);
    }
    // update keyboard event if it exists
    else if (name.substr(0, 9) == "keyboard-"){
        
        OE_KeyboardEvent* just_pressed = static_cast<OE_KeyboardEvent*>(getIEvent(name + "+").get());
        OE_KeyboardEvent* just_released = static_cast<OE_KeyboardEvent*>(getIEvent(name + "-").get());
        OE_KeyboardEvent* held = static_cast<OE_KeyboardEvent*>(getIEvent(name + "").get());

        if(just_pressed->keystate < OE_BUTTON::JUST_PRESS){
            just_pressed->keystate+=1;
            just_released->keystate+=1;
            held->keystate+=1;
        }
        if(just_pressed->keystate == OE_BUTTON::JUST_PRESS) this->broadcastIEvent(just_pressed->name, nullptr);
    }
    else {
        
    }
    
}

void OE_EventHandler::internalBroadcastKeyUpEvent(const std::string& name){
     if (name.length() <= 8)
        assert (name.substr(0, 6) == "mouse-");
    else if (name.length() > 8)
        assert (name.substr(0, 6) == "mouse-" || name.substr(0, 9) == "keyboard-");
    else{
        cout << "Error invalid name in internalBroadcastKeyDwonEvent: " << name << endl;
        assert(false);
    }
    
    // update mouse event if it exists
    if (name.substr(0, 6) == "mouse-"){
        
        OE_MouseEvent* just_pressed = static_cast<OE_MouseEvent*>(getIEvent(name + "+").get());
        OE_MouseEvent* just_released = static_cast<OE_MouseEvent*>(getIEvent(name + "-").get());
        OE_MouseEvent* held = static_cast<OE_MouseEvent*>(getIEvent(name + "").get());

        if(just_pressed->keystate < OE_BUTTON::JUST_RELEASE){
            just_pressed->keystate+=1;
            just_released->keystate+=1;
            held->keystate+=1;
        }
        if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE) this->broadcastIEvent(just_released->name, nullptr);
        else OE_WriteToLog("dafuq?"); ///IMPOSSIBLE
        
    }
    // update keyboard event if it exists
    else if (name.substr(0, 9) == "keyboard-"){
        
        OE_KeyboardEvent* just_pressed = static_cast<OE_KeyboardEvent*>(getIEvent(name + "+").get());
        OE_KeyboardEvent* just_released = static_cast<OE_KeyboardEvent*>(getIEvent(name + "-").get());
        OE_KeyboardEvent* held = static_cast<OE_KeyboardEvent*>(getIEvent(name + "").get());
    
        while(just_pressed->keystate < OE_BUTTON::JUST_RELEASE){
            just_pressed->keystate+=1;
            just_released->keystate+=1;
            held->keystate+=1;
        }
        if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE) this->broadcastIEvent(just_released->name, nullptr);
        else OE_WriteToLog("dafuq?"); ///IMPOSSIBLE
        
    }
    else {
        
    }
    
}

