#include <OE_TaskManager.h>


using namespace std;

OE_TaskManager* OE_ThreadData::taskMgr = nullptr;

OE_ThreadStruct::OE_ThreadStruct(){
    
}

OE_ThreadStruct::~OE_ThreadStruct(){
    
}

extern "C" int oxygen_engine_update_thread(void* data){

    // fetch the task manager and the thread from the pointer
    
    // execute all functions which are bind to that thread
    OE_ThreadData* actual_data = static_cast<OE_ThreadData*>(data);
    
    // update physics task
    OE_ThreadData::taskMgr->threads[actual_data->name].physics_task = OE_Task(actual_data->name + "-physics", 0, 0, OE_ThreadData::taskMgr->getTicks());
    
    OE_ThreadData::taskMgr->updateThread(actual_data->name);
    delete actual_data;
    return 0;
}

extern "C" int oxygen_engine_update_unsync_thread(void* data){
    
    // execute detached threads
    OE_UnsyncThreadData* actual_data = static_cast<OE_UnsyncThreadData*>(data);
    int output = 0;
    OE_Task unsync_task = OE_Task(actual_data->name, 0, 0, actual_data->taskMgr->getTicks());
    try{
        output = actual_data->func(unsync_task);
    }
    catch(csl::parser_error& e){
        std::string error_str = "OE: " + e.name_ + " thrown in unsync thread: '" + unsync_task.GetName() + "'" +"\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch(csl::interpreter_error& e){
        std::string error_str = "OE: " + e.name_ + " thrown in unsync thread: '" + unsync_task.GetName() + "'" +"\n";
        error_str += "\t" + e.what() + "\n";
        cout << error_str;
        OE_WriteToLog(error_str);
    }
    catch (...){
        std::string error_str = "OE: Exception thrown in unsync thread: '" + unsync_task.GetName() + "'";
        cout << error_str << endl;
        OE_WriteToLog(error_str + "\n");
    }
    actual_data->taskMgr->lockMutex();
    actual_data->taskMgr->finished_unsync_threadIDs.insert(actual_data->name);
    actual_data->taskMgr->unlockMutex();
    delete actual_data;
    return output;
}

void OE_ThreadStruct::updateTaskList(){
    
    // erase all unneeded tasks in the beginning
    for (size_t x=0; x < this->to_be_removed.size(); x++){
        for (size_t i=0; i < this->tasks.size(); i++){
            if (this->tasks[i].GetName() == this->to_be_removed.front()){
                this->tasks.erase(this->tasks.begin() + i);
                this->functions.erase(this->tasks[i].name);
            }
        }
        this->to_be_removed.pop();
    }
    
    // add all tasks of the previous frame
    for (auto x: std::exchange(this->pending_tasks, {})){
        if (this->task_names.count(x.name) == 1){
            cout << "OE: Task Manager WARNING: Already existing task: " << x.name << endl;
            OE_WriteToLog("OE: Task Manager WARNING: Already existing task: " + x.name);
            continue;
        }
        this->tasks.push_back(x);
        this->task_names.insert(x.name);
        this->functions[x.name] = this->pending_functions[x.name];
    }
    this->pending_functions.clear();
}



OE_TaskManager::OE_TaskManager()
{
    completed_threads = 0;
    done = false;
    started_threads = 0;
    countar = 0;
    world = nullptr;
}

OE_TaskManager::~OE_TaskManager()
{

}

/************************
 *  INIT
 * ***********************/

int OE_TaskManager::Init(std::string titlea, int x, int y, bool fullscreen){
    
    
    this->window = new OE_SDL_WindowSystem();
    
    // the lock and unlockMutexes are for making sure the mutexes are initialized
    this->window_mutex.lockMutex();
    this->window_mutex.unlockMutex();
    
    this->renderer_mutex.lockMutex();
    this->renderer = new NRE_Renderer();
    this->renderer->screen = this->window;
    this->renderer_mutex.unlockMutex();
    
    this->physics_mutex.lockMutex();
    this->physics = new OE_PhysicsEngineBase();
    this->physics_mutex.unlockMutex();
    
    this->window->init(x, y, titlea, fullscreen, nullptr);
    this->renderer->init();
    
    this->createCondition();
    this->createCondition();
    this->createCondition();
    this->createCondition();
    
    this->SetFrameRate(200);
    this->done = false;
    
    this->CreateNewThread("default");
    this->CreateNewThread("something else");
    //this->CreateUnsyncThread("independent", &test_unsync_thread, nullptr);
    return 0;
}

void OE_TaskManager::CreateUnsyncThread(string thread_name, const OE_METHOD func){
    
    OE_UnsyncThreadData* threaddata = new OE_UnsyncThreadData();
    threaddata->func = func;
    threaddata->name = thread_name;
    threaddata->taskMgr = this;
    lockMutex();
    this->unsync_threadIDs[thread_name] = SDL_CreateThread(oxygen_engine_update_unsync_thread, thread_name.c_str(), (void*)threaddata);
    unlockMutex();
}

void OE_TaskManager::CreateNewThread(string thread_name){
    
    bool synchro = true;
    
    lockMutex();
    
    this->threads[thread_name] = OE_ThreadStruct();
    this->threads[thread_name].synchronize = true;
    this->threads[thread_name].changed = true;
    
    OE_ThreadData::taskMgr = this;
    OE_ThreadData* data = new OE_ThreadData();
    data->name = thread_name;
    
    if(synchro)
        this->threadIDs[thread_name] = SDL_CreateThread(oxygen_engine_update_thread, thread_name.c_str(), (void*)data);
    else //USELESS
        this->unsync_threadIDs[thread_name] = SDL_CreateThread(oxygen_engine_update_thread, thread_name.c_str(), (void*)data);
    
    unlockMutex();
}

// This is for removing finished unsynchronized threads
// Otherwise they will pile up and waste memory on longer gaming sessions
void OE_TaskManager::removeFinishedUnsyncThreads(){
    lockMutex();
    for (const auto &x: this->finished_unsync_threadIDs){
        SDL_DetachThread(this->unsync_threadIDs[x]);
        this->unsync_threadIDs.erase(x);
    }
    this->finished_unsync_threadIDs.clear();
    unlockMutex();
}

/************************
 *  STEP
 * ***********************/

void OE_TaskManager::Step(){
    //synchronize at start
    lockMutex();
    started_threads++;
    
    if (started_threads > getReadyThreads()){
        started_threads = 0;
        condBroadcast(1);
    }
    else{
        condWait(1);
    }
    unlockMutex();

    this->renderer->updateSingleThread();
    done = this->window->update();
    // count how many times the step function has been called
    countar++;
    
    // THIS is where obsolete unsync threads are cleaned up
    this->removeFinishedUnsyncThreads();
    
    lockMutex();
    completed_threads++;


	// --------------------------------------------------------------
	// 	Is this really the most elegant and efficient way to solve 
	// 	the problem of a zombie thread?		- Anderas, est. 1999
	// --------------------------------------------------------------
    // ------------------ANSWER--------------------------------------
    // No, this is not the place to solve zombie threads. There cannot be
    // any "zombie" threads anyways. Unsynchronized threads are only handled in destroy()
    // This is just for synchronizing all main engine threads each frame.
    // Instead of using ONE mutex condition. The new OE Task Manager
    // uses at minimum 2 and a 3rd one for the physics engine, which would allow you
    // to do all sorts of things with the objects without having to worry about
    // other people accessing them (the synchronization with the renderer happens
    // exactly after this condition on THIS MAIN THREAD in this->renderer->updateData())
    // - Filippos, est. 2056
    // --------------------------------------------------------------
    
    if(completed_threads > getReadyThreads()){
        /// if this is the last motheyacking thread
        /// that slows down the game, signal all other threads
        /// to continue and reset the counter
        completed_threads = 0;
        condBroadcast(2);
    }
    else{
        /// if this is NOT dat last sh**
        /// wait indefinitely for a signal from the last thread
        condWait(2);
    }
    unlockMutex();
    
    //this->updateWorld();
    //this->events_task.update();
    //this->window->event_handler.handleAllEvents(&events_task);
    
    if (this->world != nullptr){
        
        this->physics->world = this->world;
        this->renderer->world = this->world;
        //auto t=clock();
        this->renderer->updateData();
        //cout << "NRE UPDATE DATA " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
    }
    
    this->updateWorld();
    this->window->event_handler.handleAllEvents();
}

void OE_TaskManager::Start(){
    done = false;
    // starts and maintains the game engine
    while (!(done)){
        this->Step();
    }
    // if an error occurs or the program ends cleanup
    this->Destroy();
}


/************************
 *  DESTROY
 * ***********************/

void OE_TaskManager::Destroy(){
    lockMutex();
    completed_threads++;
    unlockMutex();
    int thread_output = 0;
    for (auto thread : this->threadIDs){
        SDL_WaitThread(thread.second, &thread_output);
        cout << this->threads[thread.first].countar << endl;

    }
    
    for (auto thread : this->unsync_threadIDs){

        SDL_WaitThread(thread.second, &thread_output);

    }
    cout << this->countar << endl;
    
    this->renderer->destroy();
    this->physics->destroy();
    
    this->window->event_handler.cleanup();
    this->window->destroy();
    
    delete this->renderer;
    delete this->physics;
    delete this->window;
    
    if (this->world != nullptr)
        this->world = nullptr;
    if (this->pending_world != nullptr)
        this->pending_world = nullptr;
    
    this->destroy(); // from OE_MutexCondition
}

/************************
 *  THREAD UPDATE
 * ***********************/

void OE_TaskManager::updateThread(const string name){
    // obtain the queue in which the tasks are executed if the task queue if changed
    while(!done){
        //synchronize at start
        lockMutex();
        started_threads++;
        
        if (started_threads > getReadyThreads()){
            started_threads = 0;
            condBroadcast(1);
        }
        else{
            
            condWait(1);
        }
        unlockMutex();
        
        auto ticks = this->getTicks();
        if(this->threads[name].changed){
            this->sortThreadTasks(name);
        }
        this->runThreadTasks(name);
        
        if(this->threads[name].synchronize == true){
            
            // for physics engine
            int comp_threads_copy = 0;
            
            /// synchronize threads to N fps initially
            this->lockMutex();
            unsigned int local_framerate = framerate;
            unsigned int current_ticks = this->getTicks()-ticks;
            this->unlockMutex();

            if( (local_framerate) > current_ticks ){
                //this->pause(local_framerate - (current_ticks));
                this->current_framerate = 1000.0/(float)local_framerate;
            }
            else{
                this->current_framerate = 1000.0/(float)(current_ticks);
            }
            
            // count how many times the thread has been called
            this->threads[name].countar++;
            
            // SYNCHRONIZE PHYSICS
            lockMutex();
            physics_threads++;
            comp_threads_copy = physics_threads;
            if(physics_threads > (getReadyThreads()-1)){
                physics_threads=0;
                condBroadcast(3);
            }
            else{
                
                condWait(3);
            }
            unlockMutex();
            
            /**************************/
            // This is where physics are run
            this->threads[name].physics_task.update();
            try{
                this->physics->updateMultiThread(&this->threads[name].physics_task, comp_threads_copy);
            }
            catch(oe::physics_error& e){
                std::string error_str = "OE: " + e.name_ + " thrown in thread: '" + name + "', thread_num: " + std::to_string(comp_threads_copy);
                error_str += ", invocation: " + std::to_string(this->threads[name].physics_task.counter) + "\n";
                error_str += "\t" + e.what() + "\n";
                cout << error_str;
                OE_WriteToLog(error_str);
            }
            catch(...){
                /// universal error handling. will catch any exception
                /// feel free to add specific handling for specific errors
                auto task = this->threads[name].physics_task;
                string outputa = string("OE: Physics exception thrown in thread: '" + name  + "', thread_num: " + std::to_string(comp_threads_copy));
                outputa += ", invocation: " + std::to_string(task.counter);
                cout << outputa << endl;
                OE_WriteToLog(outputa + "\n");
            }
            /**************************/
            
            /// add this thread to already
            /// finished-thread counter
            lockMutex();
            completed_threads++;            
            if(completed_threads > getReadyThreads()){
                /// if this is the last motheyacking thread
                /// that slows down the game, signal all other threads
                /// to continue and reset the counter
                completed_threads = 0;
                condBroadcast(2);
            }
            else{
                /// if this is NOT dat last sh**
                /// wait indefinitely for a signal from the last thread
                condWait(2);
            }
            unlockMutex();
        }
    }
}

int OE_TaskManager::getReadyThreads(){
    /// count ALL (i repeat) AAALLLLL threads that are set to be synchronized together
    unsigned int number_of_threads=0;
    // WARNING: For some reason this causes a segmentation fault sometimes and i have no idea why
    /*for(auto thread: threads){
        if(thread.second.synchronize){
            number_of_threads+=1;
        }
    }*/
    number_of_threads = this->threads.size();
    return number_of_threads;
}

/************************
 *  OTHER FUNCS
 * ***********************/

void OE_TaskManager::updateWorld(){
    lockMutex();
    if (this->pending_world != nullptr){
        if (this->world != nullptr)
            this->world = nullptr;
        this->world = this->pending_world;
    }
    this->pending_world = nullptr;
    unlockMutex();
}

unsigned int OE_TaskManager::GetFrameRate(){
    // return active framerate
    return this->current_framerate;
}

void OE_TaskManager::SetFrameRate(unsigned int frametarget){
    // set desired framerate
    lockMutex();
    this->framerate = 1000u/frametarget;
    unlockMutex();
}

void OE_TaskManager::AddTask(string name, const OE_METHOD func){

    OE_Task task = OE_Task(name, 0, 0, this->getTicks());
    lockMutex();
    this->threads["default"].pending_tasks.push_back(task);
    this->threads["default"].pending_functions[name] = func;
    this->threads["default"].changed = true;
    unlockMutex();

}

void OE_TaskManager::AddTask(string name, const OE_METHOD func, int priority){

    OE_Task task = OE_Task(name, priority, 0, this->getTicks());
    lockMutex();
    this->threads["default"].pending_tasks.push_back(task);
    this->threads["default"].pending_functions[name] = func;
    this->threads["default"].changed = true;
    unlockMutex();
}

void OE_TaskManager::AddTask(string name, const OE_METHOD func, int priority, string threadname){

    OE_Task task = OE_Task(name, priority, 0, this->getTicks());
    lockMutex();
    this->threads[threadname].pending_tasks.push_back(task);
    this->threads[threadname].pending_functions[name] = func;
    this->threads[threadname].changed = true;
    unlockMutex();
}

void OE_TaskManager::AddTask(string name, const OE_METHOD func, string threadname){

    OE_Task task = OE_Task(name, 0, 0, this->getTicks());
    lockMutex();
    this->threads[threadname].pending_tasks.push_back(task);
    this->threads[threadname].pending_functions[name] = func;
    this->threads[threadname].changed = true;
    unlockMutex();
}

void OE_TaskManager::DoOnce(string name, const OE_METHOD func , int delay){

    OE_Task task = OE_Task(name, 0, delay, this->getTicks());
    lockMutex();
    this->threads["default"].pending_tasks.push_back(task);
    this->threads["default"].pending_functions[name] = func;
    this->threads["default"].changed = true;
    unlockMutex();
}

void OE_TaskManager::RemoveTask(std::string name){
    lockMutex();
    this->threads["default"].to_be_removed.push(name);
    unlockMutex();
}

void OE_TaskManager::RemoveTask(std::string name, std::string threadname){
    lockMutex();
    this->threads[threadname].to_be_removed.push(name);
    unlockMutex();
}

OE_Task OE_TaskManager::GetTaskInfo(string threadname, string name){

    for(auto task : this->threads[threadname].tasks){
        if(task.GetName() == name)
            return OE_Task(task.name, task.counter, task.priority, task.init_ticks, task.ticks);
    }
    return OE_Task("NONE", 0, 0, 0);
}

void OE_TaskManager::runThreadTasks(const std::string& name){
    unsigned int tasks_size = this->threads[name].functions.size();
    
    // after updating the task queue start executing the functions
    if( tasks_size != 0){
        
        vector<unsigned int> obsolete_tasks;
        
        unsigned int task_index = 0;
        for(auto &&task: this->threads[name].tasks){
            
            // store active task
            this->active_tasks[name] = task.GetName();
            task.update();
            // call the task
            int output = 0;
            try{
                if (this->threads[name].functions[task.name] != nullptr)
                    output =  this->threads[name].functions[task.name](task);
            } 
            catch(csl::parser_error& e){
                std::string error_str = "OE: " + e.name_ + " thrown in task: '" + task.name + "', thread: '" + name;
                error_str += "', invocation: " + std::to_string(task.counter) + "\n";
                error_str += "\t" + e.what() + "\n";
                cout << error_str;
                OE_WriteToLog(error_str);
                output = 1;
            }
            catch(csl::interpreter_error& e){
                std::string error_str = "OE: " + e.name_ + " thrown in task: '" + task.name + "', thread: '" + name;
                error_str += "', invocation: " + std::to_string(task.counter) + "\n";
                error_str += "\t" + e.what() + "\n";
                cout << error_str;
                OE_WriteToLog(error_str);
                output = 1;
            }
            catch(...){
                /// universal error handling. will catch any exception
                /// feel free to add specific handling for specific errors
                string outputa = string("OE: Exception thrown in task: '" + task.name + "', thread: '" + name);
                outputa += "', invocation: " + std::to_string(task.counter);
                cout << outputa << endl;
                OE_WriteToLog(outputa + "\n");
                output = 1;
            }
            switch(output){
                case 1: obsolete_tasks.push_back(task_index); break;
            }
            
            task_index++;
        }
        
        
        
        for(auto task : obsolete_tasks){
            this->threads[name].functions.erase(this->threads[name].tasks[task].name);
            this->threads[name].tasks.erase(this->threads[name].tasks.begin()+task);
        }
        obsolete_tasks.clear();
    }
}

void OE_TaskManager::sortThreadTasks(const std::string& name){
    
    lockMutex();
    this->threads[name].updateTaskList();
    unlockMutex();
    
    std::sort(this->threads[name].tasks.begin(), this->threads[name].tasks.end(), std::greater<OE_Task>()); 
}
