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
	std::unordered_map<uint8_t, std::string> mouseList;;
										 
	std::unordered_map<int, std::string> keyList;
};

#endif //OE_INPUTEVENTHANDLER_H
