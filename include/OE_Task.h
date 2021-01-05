#ifndef OE_TASK_H
#define OE_TASK_H

#include <types/OE_Libs.h>
#include <Events/OE_EventHandler.h>
#include <OE_Error.h>

class OE_Task
{
    
    friend class OE_TaskManager;
    friend struct OE_ThreadStruct;
        
    public:
       
        OE_Task();
        OE_Task(std::string, int, int, int);
        OE_Task(std::string, int, int, int, int);

        bool operator > (const OE_Task&) const;

        int CONTINUE();
        int FINISHED();
        int GetCounter();
        int GetPriority();
        int GetTime();
        int GetElapsedTime();
        std::string GetName();
        void update();

    protected:
        int counter{0};
        int priority{0};
        int delay{0};
        long long delta_ticks{0};
        std::string name;
        int init_ticks{0};
        int ticks{0};
};

#endif
