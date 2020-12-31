#include <OE_API_Helpers.h>
#include <OE_API.h>

using namespace oe;
using namespace OE_API_Helpers;
using namespace std;

int OE_API_Helpers::load_world(void* file, OE_Task task){
    
    string filename = *static_cast<string*>(file);
    delete static_cast<string*>(file);
    
	csl::Interpreter interpreter;
    shared_ptr<OE_World> loaded_world = interpreter.interpret_file(filename);
    
    OE_Main->lockMutex();
    OE_World::objectsList.extend(interpreter.object_list_, true);
    OE_World::materialsList.extend(interpreter.material_list_, true);
    OE_World::texturesList.extend(interpreter.texture_list_, true);
    OE_World::tcmsList.extend(interpreter.tcm_list_, true);
    OE_World::viewportsList.extend(interpreter.viewport_list_, true);
    OE_World::scenesList.extend(interpreter.scene_list_, true);
    OE_Main->pending_world = loaded_world;
    OE_Main->unlockMutex();
    oe::broadcast_event("loaded-" + filename, nullptr);
    
	return 0;
}

int OE_API_Helpers::manage_mouse(void* data, OE_Task* task, std::string event_name){
    
    if (event_name == "mouse-lock"){
        OE_Main->window->lockMouse();
    }
    else if (event_name == "mouse-unlock"){
        OE_Main->window->unlockMouse();
    }
    return 0;
}
