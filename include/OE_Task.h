#ifndef OE_TASK_H
#define OE_TASK_H

#include <types/OE_Libs.h>
#include <Events/OE_EventHandler.h>

class OE_Task
{
    public:
        friend class OE_TaskManager;
        OE_Task();
        OE_Task(std::string, int, int, int);
        OE_Task(std::string, int, int, int, int);
        ~OE_Task();

        int CONTINUE();
        int FINISHED();
        int GetCounter();
        int GetPriority();
        int GetTime();
        int GetElapsedTime();
        std::string GetName();
        void update();

    protected:
        int counter;
        int priority;
        int delay;
        long long delta_ticks;
        std::string name;
        int init_ticks;
        int ticks;
};

#endif
