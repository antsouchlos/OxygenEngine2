#ifndef OE_API_HELPER_INCLUDED
#define OE_API_HELPER_INCLUDED

#include <task_manager.h>

namespace OE_API_Helpers {

    int load_world(OE_Task, std::string);

    int manage_mouse(OE_Task, std::string);

    void checkIfInit();
}; // namespace OE_API_Helpers

#endif
