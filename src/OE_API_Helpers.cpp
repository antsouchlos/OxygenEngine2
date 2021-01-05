#include <OE_API_Helpers.h>
#include <OE_API.h>

using namespace oe;
using namespace OE_API_Helpers;
using namespace std;

int OE_API_Helpers::load_world(void* file, OE_Task task){
    
    string filename = *static_cast<string*>(file);
    delete static_cast<string*>(file);
    

	csl::interpret_file(filename);

    oe::broadcast_event("loaded-" + filename);
    
	return 0;
}

int OE_API_Helpers::manage_mouse(OE_Task* task, std::string event_name){
    
    if (event_name == "mouse-lock"){
        OE_Main->window->lockMouse();
    }
    else if (event_name == "mouse-unlock"){
        OE_Main->window->unlockMouse();
    }
    return 0;
}
