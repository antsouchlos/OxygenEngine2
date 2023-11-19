#include <OE/Events/event_handler.h> //yes not InputEventHandler for a reason
#include <OE/api_oe.h>
#include <OE/types/libs_oe.h>

using namespace std;

oe::input_event_handler_t::input_event_handler_t() {
    mouseList_ = {
        {SDL_BUTTON_LEFT, "1"}, {SDL_BUTTON_RIGHT, "2"}, {SDL_BUTTON_MIDDLE, "3"}, {SDL_BUTTON_X1, "4"}, {SDL_BUTTON_X2, "5"}};
    keyList_ = {
        {SDLK_0, "0"},         {SDLK_1, "1"},           {SDLK_2, "2"},           {SDLK_3, "3"},         {SDLK_4, "4"},
        {SDLK_5, "5"},         {SDLK_6, "6"},           {SDLK_7, "7"},           {SDLK_8, "8"},         {SDLK_9, "9"},
        {SDLK_a, "a"},         {SDLK_b, "b"},           {SDLK_c, "c"},           {SDLK_d, "d"},         {SDLK_e, "e"},
        {SDLK_f, "f"},         {SDLK_g, "g"},           {SDLK_h, "h"},           {SDLK_i, "i"},         {SDLK_j, "j"},
        {SDLK_k, "k"},         {SDLK_l, "l"},           {SDLK_m, "m"},           {SDLK_n, "n"},         {SDLK_o, "o"},
        {SDLK_p, "p"},         {SDLK_q, "q"},           {SDLK_r, "r"},           {SDLK_s, "s"},         {SDLK_t, "t"},
        {SDLK_u, "u"},         {SDLK_v, "v"},           {SDLK_w, "w"},           {SDLK_x, "x"},         {SDLK_y, "y"},
        {SDLK_z, "z"},         {SDLK_ESCAPE, "esc"},    {SDLK_RIGHT, "right"},   {SDLK_LEFT, "left"},   {SDLK_UP, "up"},
        {SDLK_DOWN, "down"},   {SDLK_LSHIFT, "lshift"}, {SDLK_RSHIFT, "rshift"}, {SDLK_LALT, "lalt"},   {SDLK_RALT, "ralt"},
        {SDLK_SPACE, "space"}, {SDLK_LCTRL, "lctrl"},   {SDLK_RCTRL, "rctrl"},   {SDLK_F1, "f1"},       {SDLK_F2, "f2"},
        {SDLK_F3, "f3"},       {SDLK_F4, "f4"},         {SDLK_F5, "f5"},         {SDLK_F6, "f6"},       {SDLK_F7, "f7"},
        {SDLK_F8, "f8"},       {SDLK_F9, "f9"},         {SDLK_F10, "f10"},       {SDLK_F11, "f11"},     {SDLK_F12, "f12"},
        {SDLK_SEMICOLON, ";"}, {SDLK_PERIOD, "."},      {SDLK_COMMA, ","},       {SDLK_DELETE, "del"},  {SDLK_EQUALS, "="},
        {SDLK_SLASH, "/"},     {SDLK_TAB, "tab"},       {SDLK_MINUS, "-"},       {SDLK_BACKSLASH, "\\"}};
}
oe::input_event_handler_t::~input_event_handler_t() {
}

std::map<std::string, std::shared_ptr<oe::event_t>> oe::input_event_handler_t::create_events() {

    std::map<std::string, std::shared_ptr<oe::event_t>> event_list;
    std::string                                         cur_name;
    /// generate keyboard events
    for (auto x : this->keyList_) {

        // event for just pressed (sent once per tap)
        shared_ptr<oe::keyboard_event_t> event = std::make_shared<oe::keyboard_event_t>();
        cur_name                               = "keyboard-" + x.second + "+";
        event->key_                            = x.second;
        event_list[cur_name]                   = event;

        // event for just release (sent once per release)
        shared_ptr<oe::keyboard_event_t> event1 = std::make_shared<oe::keyboard_event_t>();
        cur_name                                = "keyboard-" + x.second + "-";
        event1->key_                            = x.second;
        event_list[cur_name]                    = event1;

        // event for hold (sent every frame the key is being held)
        shared_ptr<oe::keyboard_event_t> event2 = std::make_shared<oe::keyboard_event_t>();
        cur_name                                = "keyboard-" + x.second + "";
        event2->key_                            = x.second;
        event2->is_main_event_                  = true;
        event_list[cur_name]                    = event2;
    }

    /// generate mouse events
    for (auto x : this->mouseList_) {
        // event for just pressed (sent once per tap)
        shared_ptr<oe::mouse_event_t> event = std::make_shared<oe::mouse_event_t>();
        cur_name                            = "mouse-" + x.second + "+";
        event->key_                         = x.second;
        event_list[cur_name]                = event;

        // event for just release (sent once per release)
        shared_ptr<oe::mouse_event_t> event1 = std::make_shared<oe::mouse_event_t>();
        cur_name                             = "mouse-" + x.second + "-";
        event1->key_                         = x.second;
        event_list[cur_name]                 = event1;

        // event for hold (sent every frame the key is being held)
        shared_ptr<oe::mouse_event_t> event2 = std::make_shared<oe::mouse_event_t>();
        cur_name                             = "mouse-" + x.second + "";
        event2->key_                         = x.second;
        event2->is_main_event_               = true;
        event_list[cur_name]                 = event2;
    }
    /// generate mouse motion event
    shared_ptr<oe::mouse_move_event_t> event3 = std::make_shared<oe::mouse_move_event_t>();
    cur_name                                  = "mouse-motion";
    event_list[cur_name]                      = event3;

    /// generate mouse wheel event
    shared_ptr<oe::mouse_move_event_t> event4 = std::make_shared<oe::mouse_move_event_t>();
    cur_name                                  = "mouse-wheel";
    event_list[cur_name]                      = event4;

    /// generate mouse lock event
    shared_ptr<oe::mouse_move_event_t> event5 = std::make_shared<oe::mouse_move_event_t>();
    cur_name                                  = "mouse-lock";
    event5->set_func(&OE_API_Helpers::manage_mouse);
    event_list[cur_name] = event5;

    /// generate mouse unlock event
    shared_ptr<oe::mouse_move_event_t> event6 = std::make_shared<oe::mouse_move_event_t>();
    cur_name                                  = "mouse-unlock";
    event6->set_func(&OE_API_Helpers::manage_mouse);
    event_list[cur_name] = event6;

    return event_list;
}

void oe::event_handler_t::update_input() {
    lockMutex();
    for (auto key_event_elem : this->events_list_) {
        if (key_event_elem->type_ == KEYBOARD_EVENT) {

            auto key_event = static_cast<oe::keyboard_event_t*>(key_event_elem.get_pointer().get());
            if (key_event->keystate_ == oe::BUTTON_JUST_PRESS) {
                key_event->keystate_ += 1;
            }
            else if (key_event->keystate_ == oe::BUTTON_PRESS) {
                if (key_event->is_main_event_) {
                    this->broadcast_event(key_event_elem.get_id());
                }
            }
            else if (key_event->keystate_ == oe::BUTTON_JUST_RELEASE) {
                key_event->keystate_ = oe::BUTTON_RELEASE;
            }
            else {
                continue;
            }
        }
        else if (key_event_elem->type_ == MOUSE_EVENT) {
            auto key_event = static_cast<oe::mouse_event_t*>(key_event_elem.get_pointer().get());
            if (key_event->keystate_ == oe::BUTTON_JUST_PRESS) {
                key_event->keystate_ += 1;
            }
            else if (key_event->keystate_ == oe::BUTTON_PRESS) {
                if (key_event->is_main_event_) {
                    this->broadcast_event(key_event_elem.get_id());
                }
            }
            else if (key_event->keystate_ == oe::BUTTON_JUST_RELEASE) {
                key_event->keystate_ = oe::BUTTON_RELEASE;
            }
            else {
                continue;
            }
        }
        else {
            continue;
        }
    }
    unlockMutex();
}

void oe::event_handler_t::internal_register_keydown_event(const std::string& name) {

    // update mouse event if it exists
    if (name.starts_with("mouse-")) {

        oe::mouse_event_t* just_pressed  = static_cast<oe::mouse_event_t*>(get_event(name + "+").get());
        oe::mouse_event_t* just_released = static_cast<oe::mouse_event_t*>(get_event(name + "-").get());
        oe::mouse_event_t* held          = static_cast<oe::mouse_event_t*>(get_event(name + "").get());

        if (just_pressed->keystate_ < oe::BUTTON_PRESS) {
            just_pressed->keystate_ += 1;
            just_released->keystate_ += 1;
            held->keystate_ += 1;
        }
        if (just_pressed->keystate_ == oe::BUTTON_JUST_PRESS) this->broadcast_event(just_pressed->id);
    }
    // update keyboard event if it exists
    else if (name.starts_with("keyboard-")) {

        oe::keyboard_event_t* just_pressed  = static_cast<oe::keyboard_event_t*>(get_event(name + "+").get());
        oe::keyboard_event_t* just_released = static_cast<oe::keyboard_event_t*>(get_event(name + "-").get());
        oe::keyboard_event_t* held          = static_cast<oe::keyboard_event_t*>(get_event(name + "").get());

        if (just_pressed->keystate_ < oe::BUTTON_JUST_PRESS) {
            just_pressed->keystate_ += 1;
            just_released->keystate_ += 1;
            held->keystate_ += 1;
        }
        if (just_pressed->keystate_ == oe::BUTTON_JUST_PRESS) this->broadcast_event(just_pressed->id);
    }
    else {
        // TODO: Warnings
    }
}

void oe::event_handler_t::internal_register_keyup_event(const std::string& name) {
    // update mouse event if it exists
    if (name.starts_with("mouse-")) {

        oe::mouse_event_t* just_pressed  = static_cast<oe::mouse_event_t*>(get_event(name + "+").get());
        oe::mouse_event_t* just_released = static_cast<oe::mouse_event_t*>(get_event(name + "-").get());
        oe::mouse_event_t* held          = static_cast<oe::mouse_event_t*>(get_event(name + "").get());

        while (just_pressed->keystate_ < oe::BUTTON_JUST_RELEASE) {
            just_pressed->keystate_ += 1;
            just_released->keystate_ += 1;
            held->keystate_ += 1;
        }
        if (just_pressed->keystate_ == oe::BUTTON_JUST_RELEASE)
            this->broadcast_event(just_released->id);
        else
            OE_WriteToLog("dafuq?"); /// IMPOSSIBLE
    }
    // update keyboard event if it exists
    else if (name.starts_with("keyboard-")) {

        oe::keyboard_event_t* just_pressed  = static_cast<oe::keyboard_event_t*>(get_event(name + "+").get());
        oe::keyboard_event_t* just_released = static_cast<oe::keyboard_event_t*>(get_event(name + "-").get());
        oe::keyboard_event_t* held          = static_cast<oe::keyboard_event_t*>(get_event(name + "").get());

        while (just_pressed->keystate_ < oe::BUTTON_JUST_RELEASE) {
            just_pressed->keystate_ += 1;
            just_released->keystate_ += 1;
            held->keystate_ += 1;
        }
        if (just_pressed->keystate_ == oe::BUTTON_JUST_RELEASE)
            this->broadcast_event(just_released->id);
        else
            OE_WriteToLog("dafuq?"); /// IMPOSSIBLE
    }
    else {
        // TODO: Warnings
    }
}

void oe::event_handler_t::internal_update_mouse_status(int x, int y, int delta_x, int delta_y) {
    mouse_x_       = x;
    mouse_y_       = y;
    mouse_delta_x_ = delta_x;
    mouse_delta_y_ = delta_y;
}
