#ifndef OE_EVENT_H
#define OE_EVENT_H

#include <OE_Task.h>
#include <Events/OE_EventParser.h>
#include <functional>
#include <atomic>
#include <memory>
#include <set>

/** Temporary documentation: (OUTDATED)
  *
  * OE_Event::call(OE_Task*):
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
enum OE_EVENT_TYPE{
	OE_CUSTOM_EVENT=0,
	OE_KEYBOARD_EVENT=1,
	OE_MOUSE_EVENT=2,
	OE_GAMEPAD_EVENT=3,
	OE_NETWORK_EVENT=4,
	OE_COLLISION_EVENT=5,
	OE_ERROR_EVENT=6,
	OE_EVENT_COMBO=7
};

enum OE_ERROR_IMPORTANCE{
	OE_WARNING=0,
	OE_ERROR=1,
	OE_FATAL=2
};

typedef std::function<int(OE_Task, std::string)> OE_EVENTFUNC;

int template_event_func(OE_Task, std::string);

/* general event type */
class OE_Event: public OE_THREAD_SAFETY_OBJECT{
    friend class OE_EventHandler;
	friend class OE_InputEventHandler;
    public:
        static bool finished;
        OE_Event();
        virtual ~OE_Event();
        virtual int call()=0;
    
    protected:
    	
        // internal_call() is implemented in OE_Error.cpp
    	int internal_call();
        
        void setFunc(const OE_EVENTFUNC);

        bool active_{false};
        std::string name_;
        
        OE_EVENT_TYPE type_;
        OE_EVENTFUNC func_;
        
        OE_Task task_;
        
        bool has_init_{false};
        std::set<std::string> sub_events_;
};

/*button event used in keyboard/mouse/gamepad*/


enum OE_BUTTON{
	RELEASE= 0,
	PRESS= 2,
	JUST_PRESS= 1,
	JUST_RELEASE= 3 
};

/// class intended to store keyboard events (3 for each )
class OE_KeyboardEvent : public OE_Event{
	friend class OE_EventHandler;
	friend class OE_InputEventHandler;
	public:
    
	    OE_KeyboardEvent();
	    ~OE_KeyboardEvent();
	    int call();

	protected:
    
	    uint8_t keystate;
	    std::string key;
    
};

/// class intended to store mouse events (3 for each mouse buttons 1-5 and mouse position + mouse wheel)
class OE_MouseEvent : public OE_Event{
	friend class OE_EventHandler;
	friend class OE_InputEventHandler;
	public:
    
	    OE_MouseEvent();
	    ~OE_MouseEvent();
	    int call();
		
		static int x, y, delta_x, delta_y, mouse_wheel;
		static bool mousemoved;
		
	protected:
    	
    	void signal();
    	void wait();
	    
	    uint8_t keystate;
	    
	    std::string key;
	    
};

/// class intended to store gamepad events (3 for each mouse buttons 1-5 and mouse position)
class OE_GamepadEvent : public OE_Event{
	friend class OE_EventHandler;
	friend class OE_InputEventHandler;
	public:
    
	    OE_GamepadEvent();
	    ~OE_GamepadEvent();
	    int call();

	protected:
    
	    uint8_t keystate;
	    std::string key;

	    int axis;
	    bool axismoved;
	    int x, y, delta_x, delta_y;
};

/// class intended for user events
class OE_CustomEvent : public OE_Event{
	friend class OE_EventHandler;
	friend class OE_InputEventHandler;
	public:
    
    OE_CustomEvent();
    ~OE_CustomEvent();
    int call();

	protected:
};

/// class intended for error events
class OE_ErrorEvent : public OE_Event{
	friend class OE_EventHandler;
	friend class OE_InputEventHandler;
	public:
    
    	OE_ErrorEvent();
    	~OE_ErrorEvent();
    	int call();

	protected:
		OE_ERROR_IMPORTANCE importance;
		std::string error_output;
		std::string error_name;
};


/// class intended to store multiple events
/// TODO
class OE_EventCombo : public OE_Event{
	friend class OE_EventHandler;
	friend class OE_InputEventHandler;
	public:
    
	    OE_EventCombo();
	    ~OE_EventCombo();
	    int call();

	protected:
		std::vector<std::string> event_list;
		OE_EventPair indices;
};

#endif //OE_EVENT_H
