#include <Events/OE_MutexCondition.h>

OE_MutexCondition::OE_MutexCondition(){
    
}

OE_MutexCondition::~OE_MutexCondition(){
    
}

void OE_MutexCondition::destroy(){
    for (auto x: this->conditions)
        SDL_DestroyCond(x);
}

void OE_MutexCondition::createCondition(){
    lockMutex();
    SDL_cond* condition = SDL_CreateCond();
    this->conditions.push_back(condition);
    unlockMutex();
}
    
void OE_MutexCondition::condWait(size_t id){
    if(this->conditions.size() > id){
        SDL_CondWait(this->conditions[id], this->wmutex);
    }
}

void OE_MutexCondition::condBroadcast(size_t id){
    if(this->conditions.size() > id){
        SDL_CondBroadcast(this->conditions[id]);
    }
}

void OE_MutexCondition::pause(int delay){
    SDL_Delay(delay);
}

unsigned int OE_MutexCondition::getTicks(){
    return SDL_GetTicks();
}
