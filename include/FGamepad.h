#include "FE_EventHandler.h"


using namespace std;
struct FButton{
    bool pressed;
    bool delta;
};

class FGamepad
{
    public:
        int                    joystick_id;
        SDL_Joystick*          joystick;
        SDL_mutex*             mutex;

        map<int, FButton>  buttons;
        map<int, int>          axis;

        map<string, int> button_map;
        map<string, int>   axis_map;
        bool invalid;

        FGamepad(int, SDL_mutex*, bool);
        bool IsInvalid();

        double GetAxis(string);
        double GetAxis(int);

        void   MapAxis(string, int);
        void   MapButton(string, int);

        bool   IsPressed(string);
        bool   IsJustPressed(string);
        bool   IsJustReleased(string);

        bool   IsPressed(int);
        bool   IsJustPressed(int);
        bool   IsJustReleased(int);
        FGamepad();
        ~FGamepad();
    protected:
    private:
};
