#include "FGamepad.h"

FGamepad::FGamepad(int id, SDL_mutex* event_mutex, bool invalid_arg){


        // open the joystick
        this->joystick = NULL;
        this->joystick    = SDL_JoystickOpen(id);

        this->invalid = invalid_arg;
        // check if it is opened
        if(this->joystick == NULL){
            cout << "Cannot open Joystick: " << SDL_JoystickNameForIndex(id) << endl;
        }
        unsigned int numAxes = SDL_JoystickNumAxes(this->joystick);
        unsigned int numButtons = SDL_JoystickNumButtons(this->joystick);
        // initialize axes
        for(unsigned int i=0; i< numAxes; i++){

            // since no string mappings are made yet
            // add a string version of the axis index
            // as a string mapping
            this->axis[i] = 0;
            this->axis_map[to_string(i)] = i;
        }

        // initialize axes
        for(unsigned int i=0; i< numButtons; i++){

            // since no string mappings are made yet
            // add a string version of the axis index
            // as a string mapping
            //this->buttons[i] = new FButton();
            this->buttons[i].delta = false;
            this->buttons[i].pressed = false;
            this->button_map[to_string(i)] = i;
        }

        // store joystick id
        // and the event mutex to make all functions safe-threaded
        this->joystick_id = id;
        this->mutex       = event_mutex;

}

double FGamepad::GetAxis(int id){
    return ((double)this->axis[id])/32768.0;
}

double FGamepad::GetAxis(string name){
    return this->GetAxis(this->axis_map[name]);
}

bool FGamepad::IsInvalid(){
    return this->invalid;
}

bool FGamepad::IsJustPressed(int key){

        //returns true if the key is just being pressed (wasn't pressed the previous frame)
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        bool keypress, keydelta;
        try{
            if(this==nullptr) throw JoystickButtonNotFoundWarning("\n while calling method IsJustPressed() - button index "+ to_string(key));

            SDL_LockMutex(this->mutex);
            keypress = this->buttons[key].pressed;  keydelta = this->buttons[key].delta;
            SDL_UnlockMutex(this->mutex);

        } catch(JoystickButtonNotFoundWarning warning){

            cout << warning.what() << endl;
            return false;
        }

        return keypress && !keydelta;


}

bool FGamepad::IsPressed(int key){

        // returns true if the key is being held down
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        bool keypress;
        try{
            if(this==nullptr) throw JoystickButtonNotFoundWarning("\n while calling method IsPressed() - button index "+ to_string(key));

            SDL_LockMutex(this->mutex);
            keypress = this->buttons[key].pressed;
            SDL_UnlockMutex(this->mutex);

        } catch(JoystickButtonNotFoundWarning warning){

            cout << warning.what() << endl;
            return false;
        }

        return keypress;

}

bool FGamepad::IsJustReleased(int key){

        // returns true if the key is just being released
        // lock mutex to prevent key-events data to be accessed by other threads
        // while it is updated
        bool keypress, keydelta;
        try{
            if(this==nullptr) throw JoystickButtonNotFoundWarning("\n while calling method IsJustReleased() - button index "+ to_string(key));

            SDL_LockMutex(this->mutex);
            keypress = this->buttons[key].pressed; keydelta = this->buttons[key].delta;
            SDL_UnlockMutex(this->mutex);

        } catch(JoystickButtonNotFoundWarning warning){

            cout << warning.what() << endl;
            return false;
        }
        return keydelta && !keypress;

}

void FGamepad::MapAxis(string amap, int axis_id){

    try{
        if(this==nullptr) throw JoystickNotFoundWarning("\n while calling method MapAxis()");
        this->axis_map[amap] = axis_id;

    } catch(JoystickNotFoundWarning warning){
        cout << warning.what() << endl;

    } catch(JoystickAxisNotFoundWarning warning){
        cout << warning.what() << endl;
    }

}

void FGamepad::MapButton(string amap, int axis_id){
    try{
        if(this==nullptr) throw JoystickNotFoundWarning("\n while calling method MapButton()");
        this->button_map[amap] = axis_id;

    } catch(JoystickNotFoundWarning warning){
        cout << warning.what() << endl;

    } catch(JoystickButtonNotFoundWarning warning){
        cout << warning.what() << endl;
    }
}

bool FGamepad::IsJustPressed(string key){
    return this->IsJustPressed(this->button_map[key]);
}

bool FGamepad::IsPressed(string key){
    return this->IsPressed(this->button_map[key]);
}

bool FGamepad::IsJustReleased(string key){
    return this->IsJustReleased(this->button_map[key]);
}
FGamepad::FGamepad()
{}
FGamepad::~FGamepad()
{
    this->axis.clear();
    this->buttons.clear();
    this->axis_map.clear();
    this->button_map.clear();
}
