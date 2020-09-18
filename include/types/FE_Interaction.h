#ifndef FE_INTERACTION_H
#define FE_INTERACTION_H

#include "FE_Libs.h"

using namespace std;
class FE_Physics{
public:
    string bounciness;

    bool vehicle, tire;
};

class FE_Interaction : public FE_THREAD_SAFETY_OBJECT
{
    friend class FE_Scene;

    friend class CSL_Writer;
    friend class CSL_Interpreter;
    public:
        FE_Interaction();
        ~FE_Interaction();
    protected:
        vector<string> objectNames;
        string name;

        string interaction;
    private:
};

#endif // FE_INTERACTION_H
