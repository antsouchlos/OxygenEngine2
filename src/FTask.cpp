#include "FTask.h"

FTask::FTask(string name, int priority, int delay, int ticksa){
    this->counter = 0;
    this->priority = priority;
    this->delay = delay;

    this->name = name;
    this->delta_ticks = ticksa;
    this->init_ticks = ticksa;
    this->ticks = 0;
}

FTask::FTask(string name, int countera, int priority, int init_ticks, int ticks){
    this->counter = countera;
    this->priority = priority;
    this->delay = 0;

    this->name = name;
    this->ticks = ticks;
    this->init_ticks = init_ticks;
}

FTask::~FTask(){
    //dtor
}
void FTask::update(){
    this->counter +=1;
    this->ticks = SDL_GetTicks()-this->delta_ticks;
    this->delta_ticks = SDL_GetTicks();
    //cout << "success" << endl;
}

int FTask::CONTINUE(){
    return 0;
}
int FTask::FINISHED(){
    return 1;
}
int FTask::GetCounter(){
    return this->counter;
}
int FTask::GetPriority(){
    return this->priority;
}

int FTask::GetTime(){
    return SDL_GetTicks()-this->init_ticks;
}

int FTask::GetElapsedTime(){
    return this->ticks;
}

string FTask::GetName(){
    return this->name;
}
