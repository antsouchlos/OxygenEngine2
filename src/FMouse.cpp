#include "FMouse.h"

FMouse::FMouse(SDL_mutex* keymutex){

    for (auto i : {'l', 'm', 'r'}){

        //this->keys[i] = new FMKey();
        this->keys[i].delta = false;
        this->keys[i].pressed = false;
    }
    this->x = 0; this->dx = 0;
    this->y = 0; this->dy = 0;
    this->wheel_state = 0;
    this->mutex = keymutex;
}

FMouse::~FMouse()
{
    //this->keys.clear();
}

bool FMouse::IsPressed(char key){
        // returns true if the key is being held down
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        SDL_LockMutex(this->mutex);
        bool keypress = this->keys[key].pressed;
        SDL_UnlockMutex(this->mutex);

        return keypress;
}

bool FMouse::IsJustPressed(char key){

        //returns true if the key is just being pressed (wasn't pressed the previous frame)
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        SDL_LockMutex(this->mutex);
        bool keypress = this->keys[key].pressed; bool keydelta = this->keys[key].delta;
        SDL_UnlockMutex(this->mutex);

        return keypress && !keydelta;
}

bool FMouse::IsJustReleased(char key){

        // returns true if the key is just being released
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        SDL_LockMutex(this->mutex);
        bool keypress = this->keys[key].pressed; bool keydelta = this->keys[key].delta;
        SDL_UnlockMutex(this->mutex);
        return keydelta && !keypress;
}

int FMouse::GetX(){
    return this->x;
}

int FMouse::GetY(){
    return this->y;
}

int FMouse::GetDeltaX(){
    return this->dx;
}

int FMouse::GetDeltaY(){
    return this->dy;
}

int FMouse::GetWheelStatus(){
    // returns wheel up or wheel down (1, -1)
    SDL_LockMutex(this->mutex);
    int state = this->wheel_state;
    SDL_UnlockMutex(this->mutex);
    return state;
}

bool FMouse::IsWheelScrolled(){
    // returns true if the wheel is being scrolled
    SDL_LockMutex(this->mutex);
    int state = this->wheel_state;
    SDL_UnlockMutex(this->mutex);
    return !state == 0;
}

void FMouse::Hide(){
    // lock the mouse into the screen and hide it
    SDL_LockMutex(this->mutex);
    SDL_ShowCursor(SDL_DISABLE);
    SDL_UnlockMutex(this->mutex);
}

void FMouse::Lock(){
    // lock the mouse into the screen and hide it
    SDL_LockMutex(this->mutex);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_UnlockMutex(this->mutex);
}

void FMouse::Show(){
    SDL_LockMutex(this->mutex);
    SDL_ShowCursor(SDL_ENABLE);
    SDL_UnlockMutex(this->mutex);
}

void FMouse::Unlock(){

    SDL_LockMutex(this->mutex);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    SDL_UnlockMutex(this->mutex);
}
