#include <Events/OE_EventHandler.h>

using namespace std;

int template_event_func(void* data, OE_Task* task, string event_name){ cout << event_name << endl; return 0;}

OE_EventHandler::OE_EventHandler(){
	done = false;
	
}
void OE_EventHandler::init(){
	this->input_handler.createEvents(&this->internal_events);
    this->done = false;
}

OE_EventHandler::~OE_EventHandler(){
    this->internal_events.clear();
}

// THIS IS VERY USEFUL
std::shared_ptr<OE_Event> OE_EventHandler::getIEvent(string a_name){
   /// wraps getIEventUNSAFE in a mutex, (now in 2020: like... seriously????)
   lockMutex();

   std::shared_ptr<OE_Event> output = getIEventUNSAFE(a_name);
   unlockMutex();
   return output;
}

std::shared_ptr<OE_Event> OE_EventHandler::getIEventUNSAFE(string a_name){
	/// function to simplify fetching an event by name 
	/// WARNING: DO NOT USE THE FUNCTION OUTSIDE OF MUTEXES
   
   if (internal_events.count(a_name) != 0)
       return internal_events[a_name];

   return nullptr;
}

void OE_EventHandler::createUserEvent(string a_name){

	std::shared_ptr<OE_CustomEvent> event = std::make_shared<OE_CustomEvent>();
	event->name = a_name;
	event->setFunc(&template_event_func, nullptr);
	
	lockMutex();
    if (internal_events.count(a_name) == 0){
        internal_events[a_name] = event;
    }
	unlockMutex();
}



void OE_EventHandler::destroyIEvent(string a_name){
   lockMutex();
       
   this->obsolete_events.push_back(a_name);

   unlockMutex();
   //return output;

}
void OE_EventHandler::setIEventFunc(string a_name, const OE_EVENTFUNC func, void* data){
    lockMutex();
    if (getIEventUNSAFE(a_name) != nullptr){
        getIEventUNSAFE(a_name)->setFunc(func, data);
    }
    else{
        
    }
        
    unlockMutex();
}

void OE_EventHandler::mapIEvent(string upper, string target){
	lockMutex();
    if (getIEventUNSAFE(target) != nullptr){
        getIEventUNSAFE(target)->sub_events.insert(upper);
    }
    else{
        
    }
	
	unlockMutex();
}

void OE_EventHandler::unmapIEvent(string upper, string target){
	lockMutex();
    if (getIEventUNSAFE(target) != nullptr){
        getIEventUNSAFE(target)->sub_events.erase(getIEventUNSAFE(target)->sub_events.find(upper));
    }
    else{
        
    }
	
	unlockMutex();
}
/// so simple
void OE_EventHandler::broadcastIEvent(string a_name, void* data){
    
    set<string> tobecalled_events;
	lockMutex();
    
    if (getIEventUNSAFE(a_name) != nullptr){
        this->pending_events.push_back(a_name);
        this->getIEventUNSAFE(a_name)->setFuncData(data);
        tobecalled_events = this->getIEventUNSAFE(a_name)->sub_events;
    }
    
    unlockMutex();
    
    /// broadcast any sub events (which in turn broadcast their sub events etc.)
    for(auto &x : tobecalled_events)
    	this->broadcastIEvent(x, nullptr);
}

// TODO
void OE_EventHandler::broadcastIEventWait(string a_name, int milliseconds){}

/// so simple
int OE_EventHandler::callIEvent(string a_name, OE_Task* task, void* data){

    /// generic event management
    auto event = getIEvent(a_name);
    if (event != nullptr){
        
        event->lockMutex();
        event->times_invoked++;
        event->call(task, data);
        event->unlockMutex();
    } 
    else{
        // TODO: handle error
    }

    return 0;
}

bool OE_EventHandler::havePendingEvents(){
    lockMutex();
    bool output = !this->pending_events.empty();
    unlockMutex();
    return output;
}

/*IMPORTANT
 * this function is run in the main thread only in the 2020 version
 */
int OE_EventHandler::handleAllEvents(OE_Task* task){
    
    while(havePendingEvents()){
        
        ///fetch an event and delete it from the queue
        /// so that other threads target next events
        lockMutex();
        
        this->happened_events.clear();
        
        if(pending_events.empty()){
            unlockMutex();
            return 0;
        }
        
        this->happened_events = std::exchange(this->pending_events, {});
        unlockMutex();
        
        for (auto a_event: happened_events)
            callIEvent(a_event, task, nullptr);
        
    }
    return 0;
}

void OE_EventHandler::cleanup(){

    lockMutex();
    
    for(auto &a_name: obsolete_events){
        if (getIEventUNSAFE(a_name) != nullptr){
            internal_events.erase(a_name);
        }
    }
    obsolete_events.clear();
    
    unlockMutex();
}

