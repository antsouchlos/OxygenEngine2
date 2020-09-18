#include "FE_EventHandler.h"

using namespace std;

class FTask
{
    public:
        friend class FtaskManager;
        FTask(string, int, int, int);
        FTask(string, int, int, int, int);
        ~FTask();

        int CONTINUE();
        int FINISHED();
        int GetCounter();
        int GetPriority();
        int GetTime();
        int GetElapsedTime();
        string GetName();
        void update();

    protected:
        int counter;
        int priority;
        int delay;
        long long delta_ticks;
        string name;
        int init_ticks;
        int ticks;
};
