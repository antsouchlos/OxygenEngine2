#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#include "FE_EventHandler.h"


using namespace std;

struct FMKey{
    bool delta;
    bool pressed;
};


class FMouse
{
    public:
        map<char, FMKey> keys;
        int x; int y;
        int dx; int dy;
        SDL_mutex* mutex;
        int wheel_state;

        FMouse(SDL_mutex*);
        bool IsPressed(char);
        bool IsJustPressed(char);
        bool IsJustReleased(char);

        void Lock();
        void Unlock();

        int GetDeltaX();
        int GetDeltaY();

        int GetX();
        int GetY();

        int GetWheelStatus();
        bool IsWheelScrolled();

        void Hide();
        void Show();
        FMouse(){}
        ~FMouse();
    protected:

    private:
};
