#ifndef OE_INPUTEVENTHANDLER_H
#define OE_INPUTEVENTHANDLER_H

#include <Events/OE_Event.h>

/** This class is intended to store any user inputs
  *
  */
class OE_InputEventHandler{
public:
	friend class OE_EventHandler;
    friend class OE_TaskManager;
    
	OE_InputEventHandler();
	~OE_InputEventHandler();
	
	void createEvents(std::map<std::string, std::shared_ptr<OE_Event>>*);
	
//protected:
	std::vector<std::string> active_events;
	const std::unordered_map<Uint8, std::string> mouseList = {{SDL_BUTTON_LEFT, "1"}, {SDL_BUTTON_RIGHT, "2"}, {SDL_BUTTON_MIDDLE, "3"},
										  {SDL_BUTTON_X1, "4"}, {SDL_BUTTON_X2, "5"}};
										 
	const std::unordered_map<int, std::string> keyList = {{SDLK_0, "0"}, {SDLK_1, "1"}, {SDLK_2, "2"}, {SDLK_3, "3"}, {SDLK_4, "4"}, {SDLK_5, "0"},
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
};

#endif //OE_INPUTEVENTHANDLER_H
