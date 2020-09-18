#ifndef FE_LIGHT_H
#define FE_LIGHT_H

#include "FE_Object.h"

using namespace std;
class FE_Light : public FE_Object
{
    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        const string SUN = "sun";
        const string POINT = "point";
        const string SPOT = "spot";
        FE_Light();
        ~FE_Light();

        bool init();
        bool destroy();

        FE_Light* setType(string);
        string getType();

        FE_Light* setValue(float);
        float getValue();

        FE_Light* setFOV(float);
        float getFOV();

        FE_Light* setRange(float);
        float getRange();

        FE_Light* setColor(FE_Vec4);
        FE_Vec4 getColor();

        // boiler plate functions inherited from FE_Object
        FE_Light* setData(void*);
        FE_Light* setPos(FE_Vec4);
        FE_Light* setRotEuler(FE_Vec4);
        FE_Light* setRot(FE_Vec4);
        FE_Light* setScale(FE_Vec4);

        FE_Light* setX(float);
        FE_Light* setY(float);
        FE_Light* setZ(float);
        FE_Light* setRotX(float);
        FE_Light* setRotY(float);
        FE_Light* setRotZ(float);
        FE_Light* setScaleX(float);
        FE_Light* setScaleY(float);
        FE_Light* setScaleZ(float);
    protected:
        string light_type;
        float value;
        float fov;
        float range;
        FE_Vec4 color;
    private:
};

#endif // FE_LIGHT_H
