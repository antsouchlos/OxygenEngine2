#ifndef OE_MUTEXCONDITION_H
#define OE_MUTEXCONDITION_H

#include <types/OE_TypesBase.h>

extern "C" {
    struct SDL_cond;
}

/* This class abstracts away the SDL part of locking mutexes
 * and conditions
 */ 
class OE_MutexCondition : public OE_THREAD_SAFETY_OBJECT{
public:
    OE_MutexCondition();
    ~OE_MutexCondition();
    void createCondition();
    void destroy();
    
    void condBroadcast(std::size_t);
    void condWait(std::size_t);
    void pause(int);
    unsigned int getTicks();
    // list so that it does not get invalidated by adding new conditions
    std::vector<SDL_cond*> conditions;
};

#endif
