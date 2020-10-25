#ifndef OE_TASKMANAGER_H
#define OE_TASKMANAGER_H

#include <OE_Task.h>
#include <types/OE_World.h>
#include <OE_SDL_WindowSystem.h>
#include <Events/OE_MutexCondition.h>
#include <Renderer/NRE_RendererMain.h>


class OE_TaskManager;

/* this is a shortcut type definition
 * to make code clearer.
 * It stores a pointer of a method of OE_TaskManager-derived classes
 */
typedef int(*OE_METHOD)(void*, OE_Task);

struct OE_ThreadData{
    /* This struct is passed to the update_thread function
     * It passes the task manager pointer and the thread name it runs on
     * so it can run methods from classes inherited from OE_TaskManager class
     * without any fuss in another thread
     */
    static OE_TaskManager* taskMgr;
    std::string name;
};

struct OE_UnsyncThreadData{
    OE_METHOD func;
    void* data;
};

struct OE_ThreadStruct {
    /* this struct is used to store a thread. More specifically it stores:
     * - the threadID as an SDL_Thread pointer
     * - the tasks to execute which are represented as an FTask
     * - the pointer methods to execute which represent an FTask
     * - a boolean to make the thread asynchronous
     */

    std::vector<OE_Task>        tasks;
    std::vector<OE_METHOD>      functions;
    std::vector<unsigned int>   task_queue;
    std::vector<void*>          task_data;
    
    // for new tasks to be run after the next frame
    std::vector<OE_Task>        pending_tasks;
    std::vector<OE_METHOD>      pending_functions;
    std::vector<void*>          pending_task_data;
    
    // for tasks to be removed
    std::queue<std::string>    to_be_removed;
    
    void updateTaskList();
    
    std::string          name;
    bool                 synchronize, changed;
    bool                 flushed = false;
    bool                 rendering_thread = false;

    int countar = 0;
    
    OE_Task physics_task;

};



extern "C" int oxygen_engine_update_thread(void*data);
extern "C" int oxygen_engine_update_unsync_thread(void*data);


class OE_TaskManager: public OE_MutexCondition
{
    public:

        OE_TaskManager();
        ~OE_TaskManager();

        // stores threads by their name
        std::map<std::string, struct OE_ThreadStruct>     threads = {};
        std::map<std::string,    SDL_Thread*>           threadIDs = {};

        // stores threads which should not be synchronized
        //std::map<std::string, struct OE_ThreadStruct>  unsync_threads = {};
        std::map<std::string,    SDL_Thread*>        unsync_threadIDs = {};
        
        std::map<std::string, std::string> active_tasks = {};
        
        //SDL_sem*                           global_semaphore;
        //very important variable
        bool                                done;
        
        
        OE_EventHandler                     event_handler;
        OE_RendererBase*                    renderer{nullptr};
        OE_PhysicsEngineBase*               physics{nullptr};
        OE_WindowSystemBase*                window{nullptr};
        
        OE_World*                           world{nullptr};

        OE_Task                             events_task;
        
        
        unsigned int GetFrameRate();
        void SetFrameRate(unsigned int);
        /**
        * adds a function to the to-do list of the task manager
        * the first 2 arguments are mandatory and they are:
        * - the name given to the task to be accessed with
        * - the function to be added which should return an integer and take an OE_Task as an argument
        * the rest 2 are optional and they are
        * - priority (int) higher number => lower priority
        * - the name of the thread to be run, if omitted uses the default thread
        */
        void AddTask(std::string, const OE_METHOD, void*);
        void AddTask(std::string, const OE_METHOD, int, void*);
        void AddTask(std::string, const OE_METHOD, int, std::string, void*);
        void AddTask(std::string, const OE_METHOD, std::string, void*);
        // similar to do-task, but only executes the function once after certain time has passed
        void DoOnce( std::string, const OE_METHOD, int, void*);
        OE_Task GetTaskInfo(std::string, std::string);
        
        
        // Main functions
        int Init();
        void CreateNewThread(std::string);
        void CreateUnsyncThread(std::string, const OE_METHOD, void*);
        void Step();
        void Start();
        void Destroy();
        
        
        // removes a task from the task list of a thread
        void RemoveTask(std::string);
        void RemoveTask(std::string, std::string);

        void updateThread(const std::string);
        

    protected:
        int getReadyThreads();
        unsigned int framerate;
        unsigned int current_framerate;
        
        int completed_threads;
        int started_threads{0};
        int physics_threads{0};
        
        int countar{0};
        
    private:
        
        
        void runThreadTasks(const std::string&);
        void sortThreadTasks(const std::string&);
};

#endif
