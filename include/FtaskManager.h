#include "FTask.h"
#include "FWorldManager.h"

using namespace std;

class FtaskManager;

/* this is a shortcut type definition
 * to make code clearer.
 * It stores a pointer of a method of FTaskManager-derived classes
 */
typedef int(*FMETHOD)(void*, FTask);


struct FThreadstruct {
    /* this struct is used to store a thread. More specifically it stores:
     * - the threadID as an SDL_Thread pointer
     * - the tasks to execute which are represented as an FTask
     * - the pointer methods to execute which represent an FTask
     * - a boolean to make the thread asynchronous
     */

    vector<FTask>        tasks;
    vector<FMETHOD>      functions;
    vector<unsigned int> task_queue;
    vector<void*>        task_data;

    string               name;
    bool                 synchronize, changed;
    bool                 flushed = false;
    bool                 rendering_thread = false;

    int countar = 0;

};

extern "C" int fluoric_update_thread(void*data);



class FtaskManager: public FWorldManager
{
    public:

        FtaskManager();
        ~FtaskManager();

        // stores threads by their name
        map<string, struct FThreadstruct> threads = {};
        map<string,    SDL_Thread*>        threadIDs = {};

        // stores threads which should not be synchronized
        map<string, struct FThreadstruct>  unsync_threads = {};
        map<string,    SDL_Thread*>        unsync_threadIDs = {};
        ///SDL_mutex*                         mutex;
        SDL_cond*                          synchronization_cond;
        map<string, string>                active_tasks = {};
        SDL_sem*                           global_semaphore;
        bool done;
        FE_EventHandler                    event_handler;
        /*
        int                                thread_count;
        int                                threads_run;
        bool                               threads_sync;
        bool                               start_threads;
        */
        unsigned int GetFrameRate();
        void SetFrameRate(unsigned int);
        void AddTask(string, const FMETHOD, void*);
        void AddTask(string, const FMETHOD, int, void*);
        void AddTask(string, const FMETHOD, int, string, void*);
        void AddTask(string, const FMETHOD, string, void*);
        // similar to do-task, but only executes the function once after certain time has passed
        void DoOnce( string, const FMETHOD, int, void*);
        FTask GetTaskInfo(string, string);


        // removes a task from the task list
        void RemoveTask(string);

        void updateThread(string);


    protected:
        bool areThreadsReady();
        unsigned int framerate;
        unsigned int current_framerate;
        unsigned int completed_threads;
    private:
};
