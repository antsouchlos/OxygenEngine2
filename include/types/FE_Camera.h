#ifndef FE_CAMERA_H
#define FE_CAMERA_H

#include "FE_Object.h"


class FE_Camera : public FE_Object
{
    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_Camera();
        ~FE_Camera();
        bool init();
        bool destroy();

        float getAspectRatio();
        FE_Camera* setAspectRatio(float);

        float getFOV();
        FE_Camera* setFOV(float);

        int getNear();
        int getFar();
        FE_Camera* setRange(int, int);

        // boiler plate functions inherited from FE_Object
        FE_Camera* setData(void*);
        FE_Camera* setPos(FE_Vec4);
        FE_Camera* setRotEuler(FE_Vec4);
        FE_Camera* setRot(FE_Vec4);
        FE_Camera* setScale(FE_Vec4);

        FE_Camera* setX(float);
        FE_Camera* setY(float);
        FE_Camera* setZ(float);
        FE_Camera* setRotX(float);
        FE_Camera* setRotY(float);
        FE_Camera* setRotZ(float);
        FE_Camera* setScaleX(float);
        FE_Camera* setScaleY(float);
        FE_Camera* setScaleZ(float);
        FE_Mat4 genProjectionMatrix();
    protected:
        float aspect_ratio;
        float field_of_view;

        int near;
        int far;
    private:
};

#endif // FE_CAMERA_H
