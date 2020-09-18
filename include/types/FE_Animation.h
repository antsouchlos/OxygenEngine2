#ifndef FE_ANIMATION_H
#define FE_ANIMATION_H

#include "FE_Libs.h"
#include "FMath.h"

using namespace std;
class FE_Transformation{

public:
    string bonename;

    FE_Vec4 pos;

    FE_Vec4 rot;

    FE_Vec4 sca;

};

class FE_Animation : public FE_THREAD_SAFETY_OBJECT
{
    friend class FE_Mesh;

    friend class CSL_Interpreter;
    friend class CSL_Writer;

    public:
        FE_Animation();
        ~FE_Animation();
        bool init();
        bool destroy();
    protected:
        string name;
        int frameNum;
	//string name;----------------WTF!?-----------------
	//ANSWER: OOPS didnt see it, a kai mpes steam
        bool inverse_kinematics;

        vector<FE_Transformation*> transforms;
    private:
};

#endif // FE_ANIMATION_H
