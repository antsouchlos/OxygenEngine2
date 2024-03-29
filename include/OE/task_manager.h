#ifndef OE_TASKMANAGER_H
#define OE_TASKMANAGER_H

#include <OE/Events/event_handler.h>
#include <OE/Events/mutex_condition.h>
#include <OE/dummy_classes.h>
#include <OE/global_variables.h>
#include <queue>


#ifdef __EMSCRIPTEN__
// these variables control when the other threads start in a web environment
// because webassembly/javascript do not allow threads to be created on the fly
extern std::atomic<bool> oe_threads_ready_to_start;
#endif



extern "C" int oxygen_engine_update_thread(void* data);
extern "C" int oxygen_engine_update_unsync_thread(void* data);

extern "C" {
struct SDL_Thread;
}


namespace oe {
    class task_manager_t;

    struct thread_data {
        /* This struct is passed to the update_thread function
         * It passes the task manager pointer and the thread name it runs on
         * so it can run methods from classes inherited from OE_TaskManager class
         * without any fuss in another thread
         */
        static task_manager_t* taskMgr;
        std::size_t            thread_id{0};
    };

    struct unsync_thread_data {
        oe::method_type func;
        task_manager_t* taskMgr;
        std::string     name;
    };

    struct thread_struct {
        /* this struct is used to store a thread. More specifically it stores
         * the tasks to execute which are represented as an oe::task_t
         */
        static std::atomic<std::size_t> current_id;
        const std::size_t               id;
        thread_struct();

        oe::shared_index_map_t<oe::task_t, oe::index_map_sort_type::greater_than> tasks_;
        int                                                                       countar = 0;
        oe::task_info_t                                                           physics_task;
    };

    class task_manager_t : public mutex_condition_t {

        friend int ::oxygen_engine_update_unsync_thread(void*);
        friend int ::oxygen_engine_update_thread(void*);

    public:
        /**********************************/
        // public member variables

        OE_THREAD_SAFETY_OBJECT  renderer_mutex_;
        oe::renderer_base_t*     renderer_{nullptr};
        oe::renderer_init_info   renderer_init_info_;
        oe::renderer_update_info renderer_info_;

        OE_THREAD_SAFETY_OBJECT physics_mutex_;
        oe::physics_base_t*     physics_{nullptr};
        oe::physics_update_info physics_info_;
        oe::physics_init_info   physics_init_info_;

        OE_THREAD_SAFETY_OBJECT window_mutex_;
        oe::winsys_base_t*      window_{nullptr};
        oe::winsys_update_info  window_info_;
        oe::winsys_init_info    window_init_info_;
        oe::winsys_output       window_output_;

        oe::networking_base_t*   network_{nullptr};
        oe::networking_init_info network_init_info_;

        /**********************************/
        // public methods
        task_manager_t();

        unsigned int get_frame_rate();
        void         set_frame_rate(unsigned int);
        /**
         * adds a function to the to-do list of the task manager
         * the first 2 arguments are mandatory and they are:
         * - the name given to the task to be accessed with
         * - the function to be added which should conform to the oe::method_type format specified in task.h
         * the rest 2 are optional and they are
         * - priority (int) higher number => lower priority
         * - the name of the thread to be run, if omitted uses the default thread
         */
        void add_task(std::string, const oe::method_type);
        void add_task(std::string, const oe::method_type, int);
        void add_task(std::string, const oe::method_type, int, std::string);
        void add_task(std::string, const oe::method_type, std::string);
        // similar to do-task, but only executes the function once after certain time has passed
        void            do_once(std::string, const oe::method_type, int);
        oe::task_info_t get_task_info(std::string, std::string);


        // Main functions
        int  init(std::string, int, int, bool, oe::renderer_init_info, oe::winsys_init_info, oe::physics_init_info,
                  oe::networking_init_info);
        void create_new_thread(std::string);
        void create_unsync_thread(std::string, const oe::method_type);
        void step();
        void start();
        void destroy();


        // removes a task from the task list of a thread
        void remove_task(std::string);
        void remove_task(std::string, std::string);

        bool is_done();

        void                      set_pending_world(std::shared_ptr<OE_World>);
        std::shared_ptr<OE_World> get_world();

        void force_restart_renderer();

    private:
        /**********************************/
        // private variables
        unsigned int framerate_;
        unsigned int current_framerate_;

        bool              renderer_init_errors_{false};
        std::atomic<bool> restart_renderer_{false};
        bool              physics_init_errors_{false};
        bool              winsys_init_errors_{false};
        bool              network_init_errors_{false};

        // synchronized threads
        oe::shared_index_map_t<thread_struct>        threads_;
        std::unordered_map<std::size_t, SDL_Thread*> threadIDs_ = {};

        // stores threads which should not be synchronized.
        // Overkill to make it shared_index_map, since the elements are never accessed directly
        std::unordered_map<std::string, SDL_Thread*> unsync_threadIDs_          = {};
        std::unordered_set<std::string>              finished_unsync_threadIDs_ = {};

        // very important variable
        std::atomic<bool> done_;
        bool              errors_on_init_{false};

        int completed_threads_{0};
        int started_threads_{0};
        int physics_threads_{0};

        int countar_{0};

        /**********************************/
        // private methods

        void update_thread(std::size_t);
        void update_task_list(std::size_t);


        void remove_finished_unsync_threads();

        int get_ready_threads();

        void sync_begin_frame();
        void sync_end_frame();

        void update_world();
        void run_thread_tasks(std::size_t);

        // error handling functions
        // those are implemented in OE_Error.cpp
        // in order to have all non-core-engine error handling at one place
        int             try_run_unsync_thread(unsync_thread_data*);
        oe::task_action try_run_task(std::size_t, std::shared_ptr<oe::task_t>);

        // ALL these return true if error is found so it terminates the engine
        bool try_run_physics_update_multi_thread(std::size_t, const int&);
        bool try_run_renderer_update_single_thread();
        bool try_run_renderer_update_data();
        bool try_run_winsys_update();

        bool try_run_winsys_init(int, int, std::string, bool, oe::winsys_init_info);
        bool try_run_physics_init(oe::physics_init_info);
        bool try_run_renderer_init(oe::renderer_init_info);
        bool try_run_network_init(oe::networking_init_info);
    };
}; // namespace oe
#endif
