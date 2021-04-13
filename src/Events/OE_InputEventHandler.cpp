#include <Events/OE_EventHandler.h> //yes not InputEventHandler for a reason
#include <types/OE_Libs.h>

using namespace std;

OE_InputEventHandler::OE_InputEventHandler(){
    mouseList = {{SDL_BUTTON_LEFT, "1"}, {SDL_BUTTON_RIGHT, "2"}, {SDL_BUTTON_MIDDLE, "3"},
										  {SDL_BUTTON_X1, "4"}, {SDL_BUTTON_X2, "5"}};
    keyList = {{SDLK_0, "0"}, {SDLK_1, "1"}, {SDLK_2, "2"}, {SDLK_3, "3"}, {SDLK_4, "4"}, {SDLK_5, "0"},
                                {SDLK_6, "6"}, {SDLK_7, "7"}, {SDLK_8, "8"}, {SDLK_9, "9"}, {SDLK_a, "a"}, {SDLK_b, "b"},
                                {SDLK_c, "c"}, {SDLK_d, "d"}, {SDLK_e, "e"}, {SDLK_f, "f"}, {SDLK_g, "g"},
                                {SDLK_h, "h"}, {SDLK_i, "i"}, {SDLK_j, "j"}, {SDLK_k, "k"}, {SDLK_l, "l"},
                                {SDLK_m, "m"}, {SDLK_n, "n"}, {SDLK_o, "o"}, {SDLK_p, "p"}, {SDLK_q, "q"},
                                {SDLK_r, "r"}, {SDLK_s, "s"}, {SDLK_t, "t"}, {SDLK_u, "u"}, {SDLK_v, "v"},
                                {SDLK_w, "w"}, {SDLK_x, "x"}, {SDLK_y, "y"}, {SDLK_z, "z"}, {SDLK_ESCAPE, "esc"},
                                {SDLK_RIGHT, "right"}, {SDLK_LEFT, "left"}, {SDLK_UP, "up"}, {SDLK_DOWN, "down"},
                                {SDLK_LSHIFT, "lshift"}, {SDLK_RSHIFT, "rshift"}, {SDLK_LALT, "lalt"}, {SDLK_RALT, "ralt"},
                                {SDLK_SPACE, "space"}, {SDLK_LCTRL, "lctrl"}, {SDLK_RCTRL, "rctrl"}, {SDLK_F1, "f1"},
                                {SDLK_F2, "f2"}, {SDLK_F3, "f3"}, {SDLK_F4, "f4"}, {SDLK_F5, "f5"}, {SDLK_F6, "f6"},
                                {SDLK_F7, "f7"}, {SDLK_F8, "f8"}, {SDLK_F9, "f9"}, {SDLK_F10, "f10"}, {SDLK_F11, "f11"},
                                {SDLK_F12, "f12"}, {SDLK_SEMICOLON, ";"}, {SDLK_PERIOD, "."}, {SDLK_COMMA, ","},
                                {SDLK_DELETE, "del"}, {SDLK_EQUALS, "="}, {SDLK_SLASH, "/"}, {SDLK_TAB, "tab"},
                                {SDLK_MINUS, "-"}, {SDLK_BACKSLASH, "\\" }
                                         };

}
OE_InputEventHandler::~OE_InputEventHandler(){}

void OE_InputEventHandler::createEvents(std::map<std::string, std::shared_ptr<OE_Event>>* event_list){

    ///generate keyboard events
    for(auto x: this->keyList){
        
        //event for just pressed (sent once per tap)
        shared_ptr<OE_KeyboardEvent> event = std::make_shared<OE_KeyboardEvent>();
        event->name_ = "keyboard-" + x.second + "+";
        event->key = x.second;
        event->setFunc(&template_event_func);
        event_list[0][event->name_] =event;
        
        //event for just release (sent once per release)
        shared_ptr<OE_KeyboardEvent> event1 = std::make_shared<OE_KeyboardEvent>();
        event1->name_ = "keyboard-" + x.second + "-";
        event1->key = x.second;
        event1->setFunc(&template_event_func);
        event_list[0][event1->name_] =event1;
        
        //event for hold (sent every frame the key is being held)
        shared_ptr<OE_KeyboardEvent> event2 = std::make_shared<OE_KeyboardEvent>();
        event2->name_ = "keyboard-" + x.second + "";
        event2->key = x.second;
        event2->setFunc(&template_event_func);
        event_list[0][event2->name_] =event2;
    }
    
    ///generate mouse events
    for(auto x: this->mouseList){
        //event for just pressed (sent once per tap)
        shared_ptr<OE_MouseEvent> event = std::make_shared<OE_MouseEvent>();
        event->name_ = "mouse-" + x.second + "+";
        event->key = x.second;
        event->setFunc(&template_event_func);
        event_list[0][event->name_] =event;
        
        //event for just release (sent once per release)
        shared_ptr<OE_MouseEvent> event1 = std::make_shared<OE_MouseEvent>();
        event1->name_ = "mouse-" + x.second + "-";
        event1->key = x.second;
        event1->setFunc(&template_event_func);
        event_list[0][event1->name_] =event1;
        
        //event for hold (sent every frame the key is being held)
        shared_ptr<OE_MouseEvent> event2 = std::make_shared<OE_MouseEvent>();
        event2->name_ = "mouse-" + x.second + "";
        event2->key = x.second;
        event2->setFunc(&template_event_func);
        event_list[0][event2->name_] =event2;
    
    }
    ///generate mouse motion event
    shared_ptr<OE_MouseEvent> event3 = std::make_shared<OE_MouseEvent>();
    event3->name_ = "mouse-motion";
    event3->key = "";
    event3->setFunc(&template_event_func);
    event_list[0][event3->name_] =event3;

    ///generate mouse wheel event
    shared_ptr<OE_MouseEvent> event4 = std::make_shared<OE_MouseEvent>();
    event4->name_ = "mouse-wheel";
    event4->key = "";
    event4->setFunc(&template_event_func);
    event_list[0][event4->name_] =event4;
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
            this->broadcastIEvent(held->name_);
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
            this->broadcastIEvent(held->name_);
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
        if(just_pressed->keystate == OE_BUTTON::JUST_PRESS) this->broadcastIEvent(just_pressed->name_);
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
        if(just_pressed->keystate == OE_BUTTON::JUST_PRESS) this->broadcastIEvent(just_pressed->name_);
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
        if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE) this->broadcastIEvent(just_released->name_);
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
        if(just_pressed->keystate == OE_BUTTON::JUST_RELEASE) this->broadcastIEvent(just_released->name_);
        else OE_WriteToLog("dafuq?"); ///IMPOSSIBLE
        
    }
    else {
        
    }
    
}

