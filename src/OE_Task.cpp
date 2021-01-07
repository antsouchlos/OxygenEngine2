#include <OE_Task.h>

using namespace std;

OE_Task::OE_Task(){}

OE_Task::OE_Task(string name, int priority, int delay, int ticksa){
    this->counter = 0;
    this->priority = priority;
    this->delay = delay;

    this->name = name;
    this->delta_ticks = ticksa;
    this->init_ticks = ticksa;
    this->ticks = 0;
}

OE_Task::OE_Task(string name, int countera, int priority, int init_ticks, int ticks){
    this->counter = countera;
    this->priority = priority;
    this->delay = 0;

    this->name = name;
    this->ticks = ticks;
    this->init_ticks = init_ticks;
}

bool OE_Task::operator > (const OE_Task& other) const {
    if (this->priority > other.priority){
        return true;
    }
    else if (this->priority < other.priority){
        return false;
    }
    else {
        return this->name > other.name;
    }
}

bool OE_Task::operator == (const OE_Task& other) const {
    bool output = true;
    output = output && this->name == other.name;
    output = output && this->counter == other.counter;
    return output;
}


void OE_Task::update(){
    this->counter +=1;
    this->ticks = SDL_GetTicks()-this->delta_ticks;
    this->delta_ticks = SDL_GetTicks();
}

int OE_Task::CONTINUE(){
    return 0;
}
int OE_Task::FINISHED(){
    return 1;
}
int OE_Task::GetCounter(){
    return this->counter;
}
int OE_Task::GetPriority(){
    return this->priority;
}

int OE_Task::GetTime(){
    return SDL_GetTicks()-this->init_ticks;
}

int OE_Task::GetElapsedTime(){
    return this->ticks;
}

string OE_Task::GetName(){
    return this->name;
}
