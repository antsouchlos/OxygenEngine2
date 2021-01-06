#include <Events/OE_EventHandler.h>

using namespace std;

int template_event_func(OE_Task task, string event_name){ cout << event_name << endl; return 0;}

OE_EventHandler::OE_EventHandler(){
	done = false;
	
}
void OE_EventHandler::init(){
	this->input_handler.createEvents(&this->internal_events);
    for (auto x : this->internal_events){
        this->happened_events_counter[x.first] = 0;
    }
    this->done = false;
}

OE_EventHandler::~OE_EventHandler(){
    this->internal_events.clear();
    this->happened_events_counter.clear();
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
	event->name_ = a_name;
	event->setFunc(&template_event_func);
	
	lockMutex();
    if (internal_events.count(a_name) == 0){
        internal_events[a_name] = event;
        happened_events_counter[a_name] = 0;
    }
	unlockMutex();
}



void OE_EventHandler::destroyIEvent(string a_name){
   lockMutex();
       
   this->obsolete_events.push_back(a_name);

   unlockMutex();
   //return output;

}
void OE_EventHandler::setIEventFunc(string a_name, const OE_EVENTFUNC func){
    lockMutex();
    if (getIEventUNSAFE(a_name) != nullptr){
        getIEventUNSAFE(a_name)->setFunc(func);
    }
    else{
        
    }
        
    unlockMutex();
}

void OE_EventHandler::mapIEvent(string upper, string target){
	lockMutex();
    if (getIEventUNSAFE(target) != nullptr){
        getIEventUNSAFE(target)->sub_events_.insert(upper);
    }
    else{
        
    }
	
	unlockMutex();
}

void OE_EventHandler::unmapIEvent(string upper, string target){
	lockMutex();
    if (getIEventUNSAFE(target) != nullptr){
        getIEventUNSAFE(target)->sub_events_.erase(getIEventUNSAFE(target)->sub_events_.find(upper));
    }
    else{
        
    }
	
	unlockMutex();
}
/// so simple
void OE_EventHandler::broadcastIEvent(string a_name){
    
    set<string> tobecalled_events;
	lockMutex();
    
    if (getIEventUNSAFE(a_name) != nullptr){
        this->pending_events.push_back(a_name);
        tobecalled_events = this->getIEventUNSAFE(a_name)->sub_events_;
    }
    
    unlockMutex();
    
    /// broadcast any sub events (which in turn broadcast their sub events etc.)
    for(auto &x : tobecalled_events)
    	this->broadcastIEvent(x);
}

// TODO
void OE_EventHandler::broadcastIEventWait(string a_name, int milliseconds){}

/// so simple
int OE_EventHandler::callIEvent(string a_name){

    /// generic event management
    auto event = getIEvent(a_name);
    if (event != nullptr){
        
        event->lockMutex();
        event->call();
        event->unlockMutex();
    } 
    else{
        cout << "OE Event is nullptr: '" + a_name + "'" << endl;
        OE_WriteToLog("OE Event is nullptr: '" + a_name + "'" + "\n");
    }

    return 0;
}

std::size_t OE_EventHandler::getEventActivations(std::string a_name){
    size_t output = 0;
    lockMutex();
    if (this->happened_events_counter.count(a_name) == 1){
        output = this->happened_events_counter[a_name];
    }
    unlockMutex();
    return output;
}

std::size_t OE_EventHandler::getEventCounter(std::string a_name){
    size_t output = 0;
    auto event = getIEvent(a_name);
    if (event != nullptr){
        event->lockMutex();
        output = event->task_.GetCounter();
        event->unlockMutex();
    }
    return output;
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
int OE_EventHandler::handleAllEvents(){
    
    lockMutex();
    for (auto x : this->happened_events_counter){
        this->happened_events_counter[x.first] = 0;
    }
    unlockMutex();
    
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
        for (const auto& x : this->happened_events){
            this->happened_events_counter[x]++;
        }
        unlockMutex();
        
        for (auto a_event: happened_events)
            callIEvent(a_event);
        
    }
    return 0;
}

void OE_EventHandler::cleanup(){

    lockMutex();
    
    for(auto &a_name: obsolete_events){
        if (getIEventUNSAFE(a_name) != nullptr){
            internal_events.erase(a_name);
            happened_events_counter.erase(a_name);
        }
    }
    obsolete_events.clear();
    
    unlockMutex();
}

