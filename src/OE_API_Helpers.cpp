#include <OE_API_Helpers.h>
#include <OE_API.h>

using namespace OE;
using namespace OE_API_Helpers;
using namespace std;

int OE_API_Helpers::load_world(void* file, OE_Task task){
    
    string filename = *static_cast<string*>(file);
    delete static_cast<string*>(file);
    
    CSL_Interpreter interpreter;
    OE_World* loaded_world = interpreter.interpretFile(filename);
    
    OE_Main->lockMutex();
    OE_Main->pending_world = loaded_world;
    OE_Main->unlockMutex();
    OE_BroadcastEvent("loaded-" + filename, nullptr);
    return 0;
}
