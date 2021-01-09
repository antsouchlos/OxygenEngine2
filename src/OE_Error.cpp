#include <OE_Error.h>
#include <OE_TaskManager.h>
#include <Events/OE_Event.h>

using namespace std;

// This is where events' error handling is handled
int OE_Event::internal_call(){
	/***************************/
	///generic handling
	
	if (!this->has_init_){
        
        this->task_ = OE_Task(this->name_, 0, 0, SDL_GetTicks());
        this->has_init_ = true;
    }
	
	task_.update();
	try {
        func_(task_, name_);
    }
    catch(oe::api_error& e){
        std::string error_str = "[OE Error] " + e.name_ + " thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter()) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(csl::parser_error& e){
        std::string error_str = "[CSL Error] " + e.name_ + " thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter()) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(csl::interpreter_error& e){
        std::string error_str = "[CSL Error] " + e.name_ + " thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter()) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(std::exception& e){
        std::string error_str = "[OE Error] std::exception variant thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter()) + "\n";
        error_str += "\t" + string(e.what()) + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(...){
        std::string error_str = "[OE Error] Exception thrown in event: '" + this->name_ + "', event invocation counter: " + std::to_string(this->task_.GetCounter());
        cout << error_str << endl;
        OE_WriteToLog(error_str  + "\n");
    }
    return 0;
    /**************************/
}

// error handling functions
int OE_TaskManager::tryRun_unsync_thread(OE_UnsyncThreadData* actual_data){
    
    int output = 1;
    
    OE_Task unsync_task = OE_Task(actual_data->name, 0, 0, actual_data->taskMgr->getTicks());
    try{
        output = actual_data->func(unsync_task);
    }
    catch(oe::api_error& e){
        std::string error_str = "[OE Error] " + e.name_ + " thrown in unsync thread: '" + unsync_task.GetName() + "'" +"\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(csl::parser_error& e){
        std::string error_str = "[CSL Error] " + e.name_ + " thrown in unsync thread: '" + unsync_task.GetName() + "'" +"\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(csl::interpreter_error& e){
        std::string error_str = "[CSL Error] " + e.name_ + " thrown in unsync thread: '" + unsync_task.GetName() + "'" +"\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(std::exception& e){
        std::string error_str = "[OE Error] std::exception variant thrown in unsync thread: '" + unsync_task.GetName() + "'" +"\n";
        error_str += "\t" + string(e.what()) + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch (...){
        std::string error_str = "[OE Error] Exception thrown in unsync thread: '" + unsync_task.GetName() + "'";
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    return output;
}

int OE_TaskManager::tryRun_task(const std::string& name, OE_Task& task){
    
    int output = 0;
    
    try{
        if (this->threads[name].functions[task.name] != nullptr)
            output =  this->threads[name].functions[task.name](task);
    } 
    catch(oe::api_error& e){
        std::string error_str = "[OE Error] " + e.name_ + " thrown in task: '" + task.name + "', thread: '" + name;
        error_str += "', invocation: " + std::to_string(task.counter) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
        output = 1;
    }
    catch(csl::parser_error& e){
        std::string error_str = "[CSL Error] " + e.name_ + " thrown in task: '" + task.name + "', thread: '" + name;
        error_str += "', invocation: " + std::to_string(task.counter) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
        output = 1;
    }
    catch(csl::interpreter_error& e){
        std::string error_str = "[CSL Error] " + e.name_ + " thrown in task: '" + task.name + "', thread: '" + name;
        error_str += "', invocation: " + std::to_string(task.counter) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
        output = 1;
    }
    catch(std::exception& e){
        std::string error_str = "[OE Error] " + string(typeid(e).name()) + " thrown in task: '" + task.name + "', thread: '" + name;
        error_str += "', invocation: " + std::to_string(task.counter) + "\n";
        error_str += "\t" + string(e.what()) + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
        output = 1;
    }
    catch(...){
        /// universal error handling. will catch any exception
        /// feel free to add specific handling for specific errors
        string outputa = string("[OE Error] Exception thrown in task: '" + task.name + "', thread: '" + name);
        outputa += "', invocation: " + std::to_string(task.counter);
        cout << outputa << endl;
        OE_WriteToLog(outputa + "\n");
        output = 1;
    }
    return output;
}
        
void OE_TaskManager::tryRun_physics_updateMultiThread(const std::string &name, const int& comp_threads_copy){
    
    try{
        this->physics->updateMultiThread(&this->threads[name].physics_task, comp_threads_copy);
    }
    catch(oe::api_error& e){
        std::string error_str = "[OE Error] " + e.name_ + " thrown in updateMultiThread in thread: '" + name + "', thread_num: " +                 std::to_string(comp_threads_copy);
        error_str += ", invocation: " + std::to_string(this->threads[name].physics_task.counter) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(oe::physics_error& e){
        std::string error_str = "[OE Error] " + e.name_ + " thrown in updateMultiThread in thread: '" + name + "', thread_num: " + std::to_string(comp_threads_copy);
        error_str += ", invocation: " + std::to_string(this->threads[name].physics_task.counter) + "\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(std::exception& e){
        std::string error_str = "[OE Error] " + string(typeid(e).name()) + " thrown in updateMultiThread in thread: '" + name + "', thread_num: " + std::to_string(comp_threads_copy);
        error_str += ", invocation: " + std::to_string(this->threads[name].physics_task.counter) + "\n";
        error_str += "\t" + string(e.what()) + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(...){
        /// universal error handling. will catch any exception
        /// feel free to add specific handling for specific errors
        auto task = this->threads[name].physics_task;
        string outputa = string("[OE Error] Physics exception thrown in updateMultiThread in thread: '" + name  + "', thread_num: " + std::to_string(comp_threads_copy));
        outputa += ", invocation: " + std::to_string(task.counter);
        cout << outputa << endl;
        OE_WriteToLog(outputa + "\n");
    }
}

bool OE_TaskManager::tryRun_renderer_updateSingleThread(){
    
    bool output = false;
    
    try{
        output = this->renderer->updateSingleThread();
    }
    catch(oe::api_error &e){
        std::string error_str = "[NRE Error] " + e.name_ + " thrown in updateSingleThread, invocation: " + std::to_string(this->countar);
        error_str += "\n\t" + e.what();
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(oe::renderer_error &e){
        std::string error_str = "[NRE Error] " + e.name_ + " thrown in updateSingleThread, invocation: " + std::to_string(this->countar);
        error_str += "\n\t" + e.what();
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(std::exception &e){
        std::string error_str = "[NRE Error] " + string(typeid(e).name()) + " thrown in updateSingleThread, invocation: " + std::to_string(this->countar);
        error_str += "\n\t" + string(e.what());
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(...){
        std::string error_str = "[NRE Error] Renderer exception thrown in updateSingleThread, invocation: " + std::to_string(this->countar);
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    return output;
}

void OE_TaskManager::tryRun_renderer_updateData(){
    
    try{
        this->renderer->updateData();
    }
    catch(oe::api_error &e){
        std::string error_str =  "[NRE Error] " + e.name_ + " thrown in updateData, invocation: " + std::to_string(this->countar);
        error_str += "\n\t" + e.what();
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(oe::renderer_error &e){
        std::string error_str =  "[NRE Error] " + e.name_ + " thrown in updateData, invocation: " + std::to_string(this->countar);
        error_str += "\n\t" + e.what();
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(std::exception &e){
        std::string error_str =  "[NRE Error] " + string(typeid(e).name()) + " thrown in updateData, invocation: " + std::to_string(this->countar);
        error_str += "\n\t" + string(e.what());
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(...){
        std::string error_str = "[NRE Error] Renderer exception thrown in updateData, invocation: " + std::to_string(this->countar);
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
}

bool OE_TaskManager::tryRun_winsys_update(){
    
    bool output = true;
    try{
        output = this->window->update();
    }
    catch(oe::winsys_error &e){
        std::string error_str =  "[OE WINSYS Error] " + e.name_ + " thrown in winsys update, invocation: " + std::to_string(this->countar);
        error_str += "\n\t" + e.what();
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(std::exception &e){
        std::string error_str =  "[OE WINSYS Error] " + string(typeid(e).name()) + " thrown in winsys update, invocation: " + std::to_string(this->countar);
        error_str += "\n\t" + string(e.what());
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(...){
        std::string error_str = "[OE WINSYS Error] Exception thrown in winsys update, invocation: " + std::to_string(this->countar);
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    
    return output;
}
        
void OE_TaskManager::tryRun_winsys_init(int x, int y, std::string titlea, bool fullscreen, void* params){
    
    try{
        this->window->init(x, y, titlea, fullscreen, params);
    }
    catch(oe::winsys_error &e){
        std::string error_str =  "[OE WINSYS Error] " + e.name_ + " thrown in window system initialization";
        error_str += "\n\t" + e.what();
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(std::exception &e){
        std::string error_str =  "[OE WINSYS Error] " + string(typeid(e).name()) + " thrown in window system initialization";
        error_str += "\n\t" + string(e.what());
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(...){
        std::string error_str = "[OE WINSYS Error] Could not initialize window system due to thrown exception in window->init()";
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
}

void OE_TaskManager::tryRun_physics_init(){
    
    try{
        this->physics->init();
    }
    catch(oe::physics_error &e){
        std::string error_str =  "[OE Error] " + e.name_ + " thrown in physics engine initialization";
        error_str += "\n\t" + e.what();
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(std::exception &e){
        std::string error_str =  "[OE Error] " + string(typeid(e).name()) + " thrown in physics engine initialization";
        error_str += "\n\t" + string(e.what());
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(...){
        std::string error_str = "[OE Error] Could not initialize physics engine due to thrown exception in physics->init()";
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
}

void OE_TaskManager::tryRun_renderer_init(){
    
    try{
        this->renderer->init();
    }
    catch(oe::renderer_error &e){
        std::string error_str =  "[NRE Error] " + e.name_ + " thrown in renderer initialization";
        error_str += "\n\t" + e.what();
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(std::exception &e){
        std::string error_str =  "[NRE Error] " + string(typeid(e).name()) + " thrown in renderer initialization";
        error_str += "\n\t" + string(e.what());
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    catch(...){
        std::string error_str = "[NRE Error] Could not initialize renderer due to thrown exception in renderer->init()";
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
}
