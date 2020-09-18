#ifndef FE_EVENT_H
#define FE_EVENT_H
#include "FE_EventParser.h"

using namespace std;


/** Temporary documentation:
  *
  * FE_Event::call(Ftask*, void*):
  * Return values:
  *  0: successfully run
  *  1: event is executed in another thread
  *  2++ or -1--: papares gia tin ora (add your own if necessary)
  * ------------------------------------
  * Insert custom internal event type here
  * This is needed to be able to do the correct casts afterwards
  * Andreas' TODO: 
  * -Collision Events
  * -Network Events 
  * -Anything else his subsystems need
  * Philsegeler's TODO:
  * -Anything else his subsystems need
  */
enum FE_EVENT_TYPE{
	FE_CUSTOM_EVENT=0,
	FE_KEYBOARD_EVENT=1,
	FE_MOUSE_EVENT=2,
	FE_GAMEPAD_EVENT=3,
	FE_NETWORK_EVENT=4,
	FE_COLLISION_EVENT=5,
	FE_ERROR_EVENT=6,
	FE_EVENT_COMBO=7
};

enum FE_ERROR_IMPORTANCE{
	FE_WARNING=0,
	FE_ERROR=1,
	FE_FATAL=2
};

class FTask;
typedef int(*FE_EVENTFUNC)(void*, FTask*, string);
int template_event_func(void*, FTask*, string);

/* general event type */
class FE_Event: public FE_THREAD_SAFETY_OBJECT{
    friend class FE_EventHandler;
	friend class FE_InputEventHandler;
    public:
        static bool finished;
        FE_Event();
        virtual ~FE_Event();
        virtual int call(FTask*, void*)=0;
    
    protected:
    	
    	int internal_call(FTask*, void*);
        /// internal functions
        void setFunc(const FE_EVENTFUNC, void*);
        void setFuncData(void*);
        /// variables
        int occupied;
        bool active;
        string name;
        FE_EVENT_TYPE type;
        FE_EVENTFUNC func;
        void* user_data;
        unordered_set<string> sub_events;
        //SDL-specific
		SDL_cond* condition;
};

/*button event used in keyboard/mouse/gamepad*/


enum FE_BUTTON{
	RELEASE= 0,
	PRESS= 2,
	JUST_PRESS= 1,
	JUST_RELEASE= 3 
};

/// class intended to store keyboard events (3 for each )
class FE_KeyboardEvent : public FE_Event{
	friend class FE_EventHandler;
	friend class FE_InputEventHandler;
	public:
    
	    FE_KeyboardEvent();
	    ~FE_KeyboardEvent();
	    int call(FTask*, void*);

	protected:
    
	    uint8_t keystate;
	    string key;
    
};

/// class intended to store mouse events (3 for each mouse buttons 1-5 and mouse position + mouse wheel)
class FE_MouseEvent : public FE_Event{
	friend class FE_EventHandler;
	friend class FE_InputEventHandler;
	public:
    
	    FE_MouseEvent();
	    ~FE_MouseEvent();
	    int call(FTask*, void*);
		
		static int x, y, delta_x, delta_y, mouse_wheel;
		static bool mousemoved;
		
	protected:
    	
    	void signal();
    	void wait();
	    
	    uint8_t keystate;
	    
	    string key;
	    
};

/// class intended to store gamepad events (3 for each mouse buttons 1-5 and mouse position)
class FE_GamepadEvent : public FE_Event{
	friend class FE_EventHandler;
	friend class FE_InputEventHandler;
	public:
    
	    FE_GamepadEvent();
	    ~FE_GamepadEvent();
	    int call(FTask*, void*);

	protected:
    
	    uint8_t keystate;
	    string key;

	    int axis;
	    bool axismoved;
	    int x, y, delta_x, delta_y;
};

/// class intended for user events
class FE_CustomEvent : public FE_Event{
	friend class FE_EventHandler;
	friend class FE_InputEventHandler;
	public:
    
    FE_CustomEvent();
    ~FE_CustomEvent();
    int call(FTask*, void*);

	protected:
};

/// class intended for error events
class FE_ErrorEvent : public FE_Event{
	friend class FE_EventHandler;
	friend class FE_InputEventHandler;
	public:
    
    	FE_ErrorEvent();
    	~FE_ErrorEvent();
    	int call(FTask*, void*);

	protected:
		FE_ERROR_IMPORTANCE importance;
		string error_output;
		string error_name;
};


/// class intended to store multiple events
class FE_EventCombo : public FE_Event{
	friend class FE_EventHandler;
	friend class FE_InputEventHandler;
	public:
    
	    FE_EventCombo();
	    ~FE_EventCombo();
	    int call(FTask*, void*);

	protected:
		vector<string> event_list;
		FE_EventPair indices;
};

/// remaining ones: FE_EventCombo
#endif //FE_EVENT_H
