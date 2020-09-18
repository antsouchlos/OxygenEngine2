#include "FE_EventHandler.h"


int template_event_func(void* data, FTask* task, string event_name){ cout << event_name << endl; return 0;}

FE_EventHandler::FE_EventHandler(){
	done = false;
	
}
void FE_EventHandler::init(){
	this->internal_events = this->input_handler.createEvents();

}

FE_EventHandler::~FE_EventHandler(){

	for(auto x: this->internal_events) delete x;
	//for(auto x: this->event_combos) delete x;
	//for(auto x: this->user_events) delete x;

}


FE_Event* FE_EventHandler::getIEvent(string a_name){
   /// wraps getIEventUNSAFE in a mutex
   lockMutex();

   FE_Event* output = getIEventUNSAFE(a_name);
   unlockMutex();
   return output;
}

FE_Event* FE_EventHandler::getIEventUNSAFE(string a_name){
	/// function to simplify fetching an event by name 
	/// WARNING: DO NOT USE THE FUNCTION OUTSIDE OF MUTEXES


   FE_Event* output = nullptr;
   for(size_t i = 0; i < internal_events.size(); i++)
   if(internal_events[i]->name == a_name){
       output = internal_events[i];
   }
   
   //for(size_t i = 0; i < user_events.size(); i++)
   //if(user_events[i]->name == a_name){
   //    output = user_events[i];
   //}
   
   //for(size_t i = 0; i < event_combos.size(); i++)
   //if(event_combos[i]->name == a_name){
   //    output = event_combos[i];
   //}

   return output;
}

void FE_EventHandler::createUserEvent(string a_name){

	FE_CustomEvent* event = new FE_CustomEvent();
	event->name = a_name;
	event->setFunc(&template_event_func, nullptr);
	
	lockMutex();
	internal_events.push_back(event);
	unlockMutex();
}



void FE_EventHandler::destroyIEvent(string a_name){
   lockMutex();

   for(size_t i = 0; i < internal_events.size(); i++)
   if(internal_events[i]->name == a_name){
       delete internal_events[i];
       internal_events.erase(internal_events.begin()+i);
   }

   unlockMutex();
   //return output;

}
void FE_EventHandler::setIEventFunc(string a_name, const FE_EVENTFUNC func, void* data){
	getIEvent(a_name)->setFunc(func, data);
}

void FE_EventHandler::mapIEvent(string upper, string target){
	lockMutex();
	getIEventUNSAFE(target)->sub_events.insert(upper);
	unlockMutex();
}

void FE_EventHandler::unmapIEvent(string upper, string target){
	lockMutex();
	getIEventUNSAFE(target)->sub_events.erase(getIEventUNSAFE(target)->sub_events.find(upper));
	unlockMutex();
}
/// so simple
void FE_EventHandler::broadcastIEvent(string a_name, void* data){

	lockMutex();
    this->pending_events.push(a_name);
    this->getIEventUNSAFE(a_name)->setFuncData(data);
    unordered_set<string> tobecalled_events = this->getIEventUNSAFE(a_name)->sub_events;
    unlockMutex();
    
    /// broadcast any sub events (which in turn broadcast their sub events etc.)
    for(auto &x : tobecalled_events)
    	this->broadcastIEvent(x, nullptr);
}
void FE_EventHandler::broadcastIEventWait(string a_name, int milliseconds){}

/// so simple
int FE_EventHandler::callIEvent(string a_name, FTask* task, void* data){
	
	FE_Event* temp_event = getIEvent(a_name);
	
	/// generic event management
	temp_event->lockMutex();
	temp_event->occupied++;
	
	///only call an event if it is free (nobody calls its function)
	if(temp_event->occupied > 1){ 
		
		SDL_CondWait(temp_event->condition, temp_event->wmutex);
	}
	temp_event->unlockMutex();
	
	
    getIEvent(a_name)->call(task, data);
    
    /// release the event, so that it can be called afterwards
    temp_event->lockMutex();
    getIEvent(a_name)->occupied = 0;
    SDL_CondBroadcast(temp_event->condition);
    temp_event->unlockMutex();
    
    return 0;
}


bool FE_EventHandler::havePendingEvents(){
//cout << "pending_events" << endl;
	lockMutex();
	bool output = !this->pending_events.empty();
	unlockMutex();
	return output;
}
/*IMPORTANT
 * this function is run in every thread
 */
int FE_EventHandler::updateEventThreads(FTask* task){
	
	while(havePendingEvents()){
		
		///fetch an event and delete it from the queue
		/// so that other threads target next events
		lockMutex();
		
		if(pending_events.empty()){
			unlockMutex();
			return 0;
		}
		string a_event = this->pending_events.front();
		this->pending_events.pop();
		
		unlockMutex();
		
		callIEvent(a_event, task, nullptr);
		
	}
	return 0;
}

void FE_EventHandler::cleanup(){

	lockMutex();
	
	for(auto &a_name: obsolete_events){
	
		for(size_t i = 0; i < internal_events.size(); i++){
		
			if(internal_events[i]->name == a_name){
			
				delete internal_events[i];
				internal_events.erase(internal_events.begin()+i);
			}
		}
	}
	obsolete_events.clear();
	
	unlockMutex();

}

