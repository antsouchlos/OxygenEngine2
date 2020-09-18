#ifndef FE_EVENT_HANDLER_H
#define FE_EVENT_HANDLER_H
#include "FE_InputEventHandler.h"

class FE_EventHandler: FE_THREAD_SAFETY_OBJECT{
public:

    FE_EventHandler();
    ~FE_EventHandler();
    
    // internal event functions
    void init();
    FE_Event* getIEvent(string);
    FE_Event* getIEventUNSAFE(string);
    
    void createUserEvent(string);
    void setIEventFunc(string, const FE_EVENTFUNC, void*);
    void broadcastIEvent(string, void*);
    void broadcastIEventWait(string, int);
    void mapIEvent(string, string);
    void unmapIEvent(string, string);
    int callIEvent(string, FTask*, void*);
    void destroyIEvent(string);
    
    void updateInput();
    bool update();
    void cleanup();
    void updateInputEvents();
    int updateEventThreads(FTask*);
    
protected:
	
	bool havePendingEvents();
    ///variables
    FE_InputEventHandler input_handler;
    vector<FE_Event*> internal_events;
    //vector<FE_EventCombo*> event_combos;
    //vector<FE_CustomEvent*> user_events;
    vector<string> obsolete_events;
    
    queue<string> pending_events; //there is a reason i use this instead of vector (first in first out)
    uint8_t index = -1;
    bool done;
    
    //SDL specific
    SDL_Event event;
    
};

#endif //FE_EVENT_HANDLER_H
