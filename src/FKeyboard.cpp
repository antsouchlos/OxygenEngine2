#include "FKeyboard.h"

FKeyboard::FKeyboard(){

    // initialize a new FKey struct for every key out there
    for(auto it : this->keyList){

            FKey keya = FKey();
            this->keys[it.second] = keya;
            this->keys[it.second].pressed=false; this->keys[it.second].delta = false;
    }

    // create a keyboard mutex to prevent
    // other threads from accessing keyboard data
    // while it is updated
    this->mutex = SDL_CreateMutex();

}

FKeyboard::~FKeyboard(){
    // cleanup to avoid memory leaks
    SDL_DestroyMutex(mutex);
    this->keyList.clear();
    this->keys.clear();
}

bool FKeyboard::IsJustPressed(string key){

        //returns true if the key is just being pressed (wasn't pressed the previous frame)
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        SDL_LockMutex(this->mutex);
        bool keypress = this->keys[key].pressed; bool keydelta = this->keys[key].delta;
        SDL_UnlockMutex(this->mutex);

        return keypress && !keydelta;


}

bool FKeyboard::IsPressed(string key){

        // returns true if the key is being held down
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        SDL_LockMutex(this->mutex);
        bool keypress = this->keys[key].pressed;
        SDL_UnlockMutex(this->mutex);

        return keypress;

}

bool FKeyboard::IsJustReleased(string key){

        // returns true if the key is just being released
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        SDL_LockMutex(this->mutex);
        bool keypress = this->keys[key].pressed; bool keydelta = this->keys[key].delta;
        SDL_UnlockMutex(this->mutex);
        return keydelta && !keypress;

}
