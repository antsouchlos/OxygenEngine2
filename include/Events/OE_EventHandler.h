#ifndef OE_EVENT_HANDLER_H
#define OE_EVENT_HANDLER_H

#include <Events/OE_InputEventHandler.h>

class OE_EventHandler: public OE_THREAD_SAFETY_OBJECT{
public:
    
    friend class OE_TaskManager;

    OE_EventHandler();
    ~OE_EventHandler();
    
    // internal event functions
    void init();
    std::shared_ptr<OE_Event> getIEvent(std::string);
    std::shared_ptr<OE_Event> getIEventUNSAFE(std::string);
    
    void createUserEvent(std::string);
    void setIEventFunc(std::string, const OE_EVENTFUNC);
    void broadcastIEvent(std::string);
    
    void broadcastIEventWait(std::string, int); // TODO
    void mapIEvent(std::string, std::string);
    void unmapIEvent(std::string, std::string);
    int callIEvent(std::string);
    void destroyIEvent(std::string);
    
    std::size_t getEventActivations(std::string);
    std::size_t getEventCounter(std::string);
    
    void updateInput();
    void cleanup();
    int handleAllEvents();
    
    
    void updatePostInputLoop();
    
    // The methods starting with internal* are only supposed to be usede
    // in subclasses of OE_WindowSystemBase
    // the key strings must exist
    
    void internalBroadcastKeyDownEvent(const std::string&);
    void internalBroadcastKeyUpEvent(const std::string&);
    
    std::atomic<bool> done;
    
    OE_InputEventHandler input_handler;
    
protected:
	
	bool havePendingEvents();

    
    
    std::map<std::string, std::shared_ptr<OE_Event>> internal_events;
    std::vector<std::string> obsolete_events;
    std::vector<std::string> pending_events;
    
    std::vector<std::string> happened_events;
    std::unordered_map<std::string, std::size_t> happened_events_counter;
    
    uint8_t index = -1;    
};

#endif //OE_EVENT_HANDLER_H
