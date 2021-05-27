#ifndef OE_MUTEXCONDITION_H
#define OE_MUTEXCONDITION_H

#include <types/OE_TypesBase.h>

#ifdef __EMSCRIPTEN__
    struct OE_EmscriptenBoolConditionWrapper: public OE_THREAD_SAFETY_OBJECT{
        bool value_{false};
        
        void operator = (bool a_val);
        bool operator ! ();
    };
#else
extern "C" {
    struct SDL_cond;
}
#endif
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
#ifdef __EMSCRIPTEN__
     std::vector<OE_EmscriptenBoolConditionWrapper> conditions;
#else
    std::vector<SDL_cond*> conditions;
#endif
};

#endif
