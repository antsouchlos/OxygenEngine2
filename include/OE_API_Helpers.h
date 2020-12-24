#ifndef OE_API_HELPER_INCLUDED
#define OE_API_HELPER_INCLUDED

#include <OE_TaskManager.h>

namespace OE_API_Helpers{
    
    int load_world(void*, OE_Task);
    
    int manage_mouse(void*, OE_Task*, std::string);
};

#endif