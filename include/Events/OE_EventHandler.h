#ifndef OE_EVENT_HANDLER_H
#define OE_EVENT_HANDLER_H


#include <Events/OE_InputEventHandler.h>

class OE_EventHandler: OE_THREAD_SAFETY_OBJECT{
public:
    
    friend class OE_TaskManager;

    OE_EventHandler();
    ~OE_EventHandler();
    
    // internal event functions
    void init();
    OE_Event* getIEvent(std::string); // THIS IS TOTALLY USELESS! WTF WAS I EVEN THINKING??!
    OE_Event* getIEventUNSAFE(std::string);
    
    void createUserEvent(std::string);
    void setIEventFunc(std::string, const OE_EVENTFUNC, void*);
    void broadcastIEvent(std::string, void*);
    
    void broadcastIEventWait(std::string, int); // TOTALLY USELESS
    void mapIEvent(std::string, std::string);
    void unmapIEvent(std::string, std::string);
    int callIEvent(std::string, OE_Task*, void*);
    void destroyIEvent(std::string);
    
    void updateInput();
    bool update();
    void cleanup();
    void updateInputEvents();
    int handleAllEvents(OE_Task*);
    
    bool done;
    
protected:
	
	bool havePendingEvents();

    OE_InputEventHandler input_handler;
    
    std::map<std::string, OE_Event*> internal_events;
    std::vector<std::string> obsolete_events;
    std::vector<std::string> pending_events;
    
    std::vector<std::string> happened_events;
    uint8_t index = -1;
    
    
    //SDL specific
    SDL_Event event;
    
};

#endif //OE_EVENT_HANDLER_H
