#include <OE_TaskManager.h>


using namespace std;

OE_TaskManager* OE_ThreadData::taskMgr = nullptr;

/*int test_unsync_thread(void* data, OE_Task task){
    
    SDL_Delay(6000);
    cout << "THIS THREAD IS FINISHED" << endl;
    
    return 0;
}*/


extern "C" int oxygen_engine_update_thread(void* data){

    // fetch the task manager and the thread from the pointer
    
    // execute all functions which are bind to that thread
    OE_ThreadData* actual_data = static_cast<OE_ThreadData*>(data);
    
    // update physics task
    OE_ThreadData::taskMgr->threads[actual_data->name].physics_task = OE_Task("OE_Physics", 0, 0, OE_ThreadData::taskMgr->getTicks());
    
    OE_ThreadData::taskMgr->updateThread(actual_data->name);
    //cout << actual_data->name;
    delete actual_data;
    return 0;
}

extern "C" int oxygen_engine_update_unsync_thread(void* data){
    
    // execute detached threads
    OE_UnsyncThreadData* actual_data = static_cast<OE_UnsyncThreadData*>(data);
    int output = actual_data->func(actual_data->data, OE_Task());
    actual_data->taskMgr->lockMutex();
    actual_data->taskMgr->finished_unsync_threadIDs.insert(actual_data->name);
    actual_data->taskMgr->unlockMutex();
    delete actual_data;
    return output;
}

void OE_ThreadStruct::updateTaskList(){
    
    for (size_t x=0; x < this->to_be_removed.size(); x++){
        for (size_t i=0; i < this->tasks.size(); i++){
            if (this->tasks[i].GetName() == this->to_be_removed.front()){
                this->tasks.erase(this->tasks.begin() + i);
                this->functions.erase(this->functions.begin() + i);
                this->task_data.erase(this->task_data.begin() + i);
            }
        }
        this->to_be_removed.pop();
    }
    
    for (auto x: std::exchange(this->pending_tasks, {}))
        this->tasks.push_back(x);
    for (auto x: std::exchange(this->pending_functions, {}))
        this->functions.push_back(x);
    for (auto x: std::exchange(this->pending_task_data, {}))
        this->task_data.push_back(x);
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
    
    this->renderer = new NRE_Renderer();
    this->renderer->screen = this->window;
    this->physics = new OE_PhysicsEngineBase();
    
    this->window->init(x, y, titlea, fullscreen, nullptr);
    this->renderer->init();
    
    this->createCondition();
    this->createCondition();
    this->createCondition();
    this->createCondition();
    
    this->events_task = OE_Task("OE_Physics", 0, 0, this->getTicks());
    
    this->event_handler.init();
    this->SetFrameRate(200);
    this->done = false;
    
    this->CreateNewThread("default");
    this->CreateNewThread("something else");
    //this->CreateUnsyncThread("independent", &test_unsync_thread, nullptr);
    return 0;
}

void OE_TaskManager::CreateUnsyncThread(string thread_name, const OE_METHOD func, void* params){
    
    OE_UnsyncThreadData* threaddata = new OE_UnsyncThreadData();
    threaddata->func = func;
    threaddata->data = params;
    threaddata->name = thread_name;
    threaddata->taskMgr = this;
    lockMutex();
    this->unsync_threadIDs[thread_name] = SDL_CreateThread(oxygen_engine_update_unsync_thread, thread_name.c_str(), (void*)threaddata);
    unlockMutex();
}

void OE_TaskManager::CreateNewThread(string thread_name){
    
    bool synchro = true;
    OE_ThreadStruct defaultThread = OE_ThreadStruct();
    defaultThread.synchronize = synchro;
    defaultThread.changed = true;
    
    lockMutex();
    
    this->threads[thread_name] = defaultThread;

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
    done = this->event_handler.update();

    this->renderer->updateSingleThread();
    this->window->update();
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
    
    this->updateWorld();
    this->events_task.update();
    this->event_handler.handleAllEvents(&events_task);
    
    if (this->world != nullptr){
        
        this->physics->world = this->world;
        this->renderer->world = this->world;
        this->renderer->updateData();
    }
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
    
    this->event_handler.cleanup();
    
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
            /// synchronize threads to N fps initially
            this->lockMutex();
            unsigned int local_framerate = framerate;
            unsigned int current_ticks = this->getTicks()-ticks;
            int comp_threads_copy = completed_threads;
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
            if(physics_threads > (getReadyThreads()-1)){
                physics_threads=0;
                condBroadcast(3);
            }
            else{
                condWait(3);
            }
            unlockMutex();
            
            this->threads[name].physics_task.update();
            this->physics->updateMultiThread(&this->threads[name].physics_task, comp_threads_copy);
            
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
    for(auto thread: threads){
        if(thread.second.synchronize){
            number_of_threads+=1;
        }
    }
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

void OE_TaskManager::AddTask(string name, const OE_METHOD func, void* data){

    OE_Task task = OE_Task(name, 0, 0, this->getTicks());
    lockMutex();
    this->threads["default"].pending_tasks.push_back(task);
    this->threads["default"].pending_functions.push_back(func);
    this->threads["default"].pending_task_data.push_back(data);
    this->threads["default"].changed = true;
    unlockMutex();

}

void OE_TaskManager::AddTask(string name, const OE_METHOD func, int priority, void* data){

    OE_Task task = OE_Task(name, priority, 0, this->getTicks());
    lockMutex();
    this->threads["default"].pending_tasks.push_back(task);
    this->threads["default"].pending_functions.push_back(func);
    this->threads["default"].pending_task_data.push_back(data);
    this->threads["default"].changed = true;
    unlockMutex();
}

void OE_TaskManager::AddTask(string name, const OE_METHOD func, int priority, string threadname, void* data){

    OE_Task task = OE_Task(name, priority, 0, this->getTicks());
    lockMutex();
    this->threads[threadname].pending_tasks.push_back(task);
    this->threads[threadname].pending_functions.push_back(func);
    this->threads[threadname].pending_task_data.push_back(data);
    this->threads[threadname].changed = true;
    unlockMutex();
}

void OE_TaskManager::AddTask(string name, const OE_METHOD func, string threadname, void* data){

    OE_Task task = OE_Task(name, 0, 0, this->getTicks());
    lockMutex();
    this->threads[threadname].pending_tasks.push_back(task);
    this->threads[threadname].pending_functions.push_back(func);
    this->threads[threadname].pending_task_data.push_back(data);
    this->threads[threadname].changed = true;
    unlockMutex();
}

void OE_TaskManager::DoOnce(string name, const OE_METHOD func , int delay, void* data){

    OE_Task task = OE_Task(name, 0, delay, this->getTicks());
    lockMutex();
    this->threads["default"].pending_tasks.push_back(task);
    this->threads["default"].pending_functions.push_back(func);
    this->threads["default"].pending_task_data.push_back(data);
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
	//this->event_handler.updateEventThreads(nullptr);
    // after updating the task queue start executing the functions
    if( tasks_size != 0){
        
        vector<unsigned int> obsolete_tasks;
        for(unsigned int task= 0; task< this->threads[name].task_queue.size(); task++){

            OE_ThreadStruct temp = this->threads[name];
            // store active task
            this->active_tasks[name] = this->threads[name].tasks[task].GetName();
            this->threads[name].tasks[task].update();
            // call the task
            int output = 0;
            #ifdef FE_DEBUG
            try{
                output =  temp.functions[task](temp.task_data[task], temp.tasks[task]);
            } 
            catch(runtime_error& exc){
                /// universal error handling. will catch any error inherited from std::runtime_error
                string outputa = string(exc.what()) + "\n Task: " + this->threads[name].tasks[task].GetName() + "\n Thread: " + name;
                cout << outputa << endl;
                OE_WriteToLog(outputa);
                output = 1;
            }
            #else
            output = temp.functions[task](temp.task_data[task], temp.tasks[task]);
            #endif
            //cout << name << endl;
            switch(output){
                case 1: obsolete_tasks.push_back(task); break;
            }
            //this->event_handler.updateEventThreads(nullptr);
        }
        
        for(auto task : obsolete_tasks){
            this->threads[name].functions.erase(this->threads[name].functions.begin()+task);
            this->threads[name].tasks.erase(this->threads[name].tasks.begin()+task);
            this->threads[name].task_data.erase(this->threads[name].task_data.begin()+task);
        }
        obsolete_tasks.clear();
    }
}

void OE_TaskManager::sortThreadTasks(const std::string& name){
    
    lockMutex();
    this->threads[name].updateTaskList();
    unlockMutex();
    
    this->threads[name].task_queue.clear();

    /// make a new list to store the succession the tasks should be called
    list<int> task_priority_list = {};
        
    for (unsigned int i=0; i<this->threads[name].tasks.size(); i++)
        task_priority_list.push_back(this->threads[name].tasks[i].GetPriority());

    task_priority_list.sort(greater<int>());
    /// update the threads task queue so that the succession doesn't
    /// have to be calculated every frame
    /// for every task priority
    for(auto task_index : task_priority_list){
        for(unsigned int i=0; i<this->threads[name].tasks.size(); i++){
            // if the tasks corresponds to the priority
            if (this->threads[name].tasks[i].GetPriority() == task_index)
                this->threads[name].task_queue.push_back(i);
        }
    }
}
