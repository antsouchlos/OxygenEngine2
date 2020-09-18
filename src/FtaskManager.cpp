#include "FtaskManager.h"






FtaskManager::FtaskManager()
{
    // this initializer is obsolete
    // it is rewritten in FEngine.cpp
    // dont uncomment dat sh** it will break the program
    /*
     FThreaddata* defaultThreadData = new FThreaddata();
    defaultThreadData->taskMgr = this;
    defaultThreadData->name = "default";
    cout << "12123456789" << endl;

    FThreadstruct defaultThread;
    //defaultThread.threadID = SDL_CreateThread(fluoric_update_thread, "default", &defaultThreadData);
    defaultThread.tasks = {};
    defaultThread.functions = {};
    this->threads["default"] = &defaultThread;
    this->threads["default"]->changed = false;
    */
    //cout << "deleting" << endl;
    completed_threads = 0;
}

FtaskManager::~FtaskManager()
{
    /*for(auto thread: threads){
        thread.second.functions.clear();
        thread.second.tasks.clear();
    }*/
    //this->threads.clear();
    //this->threadIDs.clear();
}

unsigned int FtaskManager::GetFrameRate(){
    // return active framerate
    return this->current_framerate;
}

void FtaskManager::SetFrameRate(unsigned int frametarget){
    // set desired framerate
     this->framerate = 1000u/frametarget;
}
/*
 * adds a function to the to-do list of the task manager
 * the first 2 arguments are mandatory and they are:
 * - the name given to the task to be accessed with
 * - the function to be added which should return an integer and take an FTask as an argument
 * the rest 2 are optional and they are
 * - priority (int) higher number => lower priority
 * - the name of the thread to be run, if omitted uses the default thread
 */
void FtaskManager::AddTask(string name, const FMETHOD func, void* data){

    FTask task = FTask(name, 0, 0, SDL_GetTicks());
    this->threads["default"].tasks.push_back(task);
    this->threads["default"].functions.push_back(func);
    this->threads["default"].task_data.push_back(data);
    this->threads["default"].changed = true;

}

void FtaskManager::AddTask(string name, const FMETHOD func, int priority, void* data){

    FTask task = FTask(name, priority, 0, SDL_GetTicks());
    this->threads["default"].tasks.push_back(task);
    this->threads["default"].functions.push_back(func);
    this->threads["default"].task_data.push_back(data);
    this->threads["default"].changed = true;
}

 void FtaskManager::AddTask(string name, const FMETHOD func, int priority, string threadname, void* data){

    FTask task = FTask(name, priority, 0, SDL_GetTicks());
    this->threads[threadname].tasks.push_back(task);
    this->threads[threadname].functions.push_back(func);
    this->threads[threadname].task_data.push_back(data);
    this->threads[threadname].changed = true;
}

 void FtaskManager::AddTask(string name, const FMETHOD func, string threadname, void* data){

    FTask task = FTask(name, 0, 0, SDL_GetTicks());
    this->threads[threadname].tasks.push_back(task);
    this->threads[threadname].functions.push_back(func);
    this->threads[threadname].task_data.push_back(data);
    this->threads[threadname].changed = true;
}

void FtaskManager::DoOnce(string name, const FMETHOD func , int delay, void* data){

    FTask task = FTask(name, 0, delay, SDL_GetTicks());
    this->threads["default"].tasks.push_back(task);
    this->threads["default"].functions.push_back(func);
    this->threads["default"].task_data.push_back(data);
    this->threads["default"].changed = true;
}

FTask FtaskManager::GetTaskInfo(string threadname, string name){

    for(auto task : this->threads[threadname].tasks){
        if(task.GetName() == name)
            return FTask(task.name, task.counter, task.priority, task.init_ticks, task.ticks);
    }
    return FTask("NONE", 0, 0, 0);
}

void FtaskManager::updateThread(string name){



    // obtain the queue in which the tasks are executed if the task queue if changed
    while(!done){
    Uint32 ticks = SDL_GetTicks();


    if(this->threads[name].changed){

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

    unsigned int tasks_size = this->threads[name].functions.size();
	this->event_handler.updateEventThreads(nullptr);
    // after updating the task queue start executing the functions
    if( tasks_size != 0){

        vector<unsigned int> obsolete_tasks;

        for(unsigned int task= 0; task< this->threads[name].task_queue.size(); task++){


            FThreadstruct temp = this->threads[name];

            // store active task
            this->active_tasks[name] = this->threads[name].tasks[task].GetName();
            this->threads[name].tasks[task].update();

            // call the task
            int output = 0;
            #ifdef FE_DEBUG

            try{
                output =  temp.functions[task](temp.task_data[task], temp.tasks[task]);

            } catch(runtime_error& exc){

                /// universal error handling. will catch any error inherited from std::runtime_error
                string outputa = string(exc.what()) + "\n Task: " + this->threads[name].tasks[task].GetName() + "\n Thread: " + name;
                cout << outputa << endl;
                writeToLog(outputa);
                output = 1;
            }
            #else
            output = temp.functions[task](temp.task_data[task], temp.tasks[task]);
            #endif
            //cout << name << endl;
            switch(output){

                case 1: obsolete_tasks.push_back(task); break;

            }
            this->event_handler.updateEventThreads(nullptr);
        }

        for(auto task : obsolete_tasks){

                this->threads[name].functions.erase(this->threads[name].functions.begin()+task);
                this->threads[name].tasks.erase(this->threads[name].tasks.begin()+task);
                this->threads[name].task_data.erase(this->threads[name].task_data.begin()+task);

        }
        obsolete_tasks.clear();
    }

    if(this->threads[name].synchronize){

        /// synchronize threads to N fps initially
        this->lockMutex();
        unsigned int local_framerate = framerate;
        unsigned int current_ticks = SDL_GetTicks()-ticks;
        this->unlockMutex();

        if( (local_framerate) > current_ticks ){
            SDL_Delay( local_framerate - (current_ticks) );
            this->current_framerate = 1000.0/(float)local_framerate ;
        }
        else{
            this->current_framerate = 1000.0/(float)(current_ticks);

        }

        /// add this thread to already
        /// finished-thread counter
        lockMutex();
        completed_threads++;
        //cout << name << endl;
        unlockMutex();

        if(areThreadsReady()){

            /// if this is the last motheyacking thread
            /// that slows down the game, signal all other threads
            /// to continue and reset the counter
            lockMutex();
            //cout << "" << endl;
            completed_threads = 0;
            SDL_CondBroadcast(synchronization_cond);
            unlockMutex();

        }
        else{

            /// if this is NOT dat last sh**
            /// wait indefinitely for a signal from the last thread
            lockMutex();
            SDL_CondWait(synchronization_cond, this->wmutex);
            unlockMutex();

        }

    }
    }
}

bool FtaskManager::areThreadsReady(){

    lockMutex();

    /// count ALL (i repeat) AAALLLLL threads that are set to be synchronized together
    unsigned int number_of_threads=0;
    for(auto thread: threads)
    if(thread.second.synchronize){
        number_of_threads+=1;
    }

    bool output = completed_threads > number_of_threads;
    unlockMutex();

    return output;
}

