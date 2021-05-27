#include <Events/OE_MutexCondition.h>
#include <types/OE_Libs.h>

#ifdef __EMSCRIPTEN__
void OE_EmscriptenBoolConditionWrapper::operator = (bool a_val){
    lockMutex();
    value_ = a_val;
    unlockMutex();
}

bool OE_EmscriptenBoolConditionWrapper::operator ! (){
    lockMutex();
    auto result = value_;
    unlockMutex();
    return !result;
}
#endif

OE_MutexCondition::OE_MutexCondition(){
    
}

OE_MutexCondition::~OE_MutexCondition(){
    
}

void OE_MutexCondition::destroy(){
    lockMutex();
#ifndef __EMSCRIPTEN__
    for (auto x: this->conditions)
        SDL_DestroyCond(x);
#else
    this->conditions.clear();
#endif
    unlockMutex();
}

void OE_MutexCondition::createCondition(){
    lockMutex();
#ifdef __EMSCRIPTEN__

    this->conditions.push_back(OE_EmscriptenBoolConditionWrapper());
#else
    SDL_cond* condition = SDL_CreateCond();
    this->conditions.push_back(condition);
#endif
    
    unlockMutex();
}
    
void OE_MutexCondition::condWait(size_t id){
    if(this->conditions.size() > id){
#ifdef __EMSCRIPTEN__
        this->conditions[id] = false;
        unlockMutex();
        
        while (!this->conditions[id])
            this->pause(1);
        
        lockMutex();
#else
        SDL_CondWait(this->conditions[id], this->wmutex);
#endif
    }
}

void OE_MutexCondition::condBroadcast(size_t id){
    if(this->conditions.size() > id){
#ifdef __EMSCRIPTEN__
        this->conditions[id] = true;
#else
        SDL_CondBroadcast(this->conditions[id]);
#endif
    }
}

void OE_MutexCondition::pause(int delay){
    SDL_Delay(delay);
}

unsigned int OE_MutexCondition::getTicks(){
    return SDL_GetTicks();
}
