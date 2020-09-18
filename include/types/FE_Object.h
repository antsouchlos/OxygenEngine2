#ifndef FE_OBJECT_H
#define FE_OBJECT_H
#include "FMath.h"

class FE_Object:  public FE_THREAD_SAFETY_OBJECT
{

    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_Object();
        virtual ~FE_Object()=0;
        virtual bool init()=0;
        virtual bool destroy()=0;

        FE_Vec4 getPos();
        FE_Vec4 getRot();
        FE_Vec4 getRotEuler();
        FE_Vec4 getScale();

        FE_Vec4 getDeltaPos();
        FE_Vec4 getDeltaRot();
        FE_Vec4 getDeltaRotEuler();
        FE_Vec4 getDeltaScale();

        FE_Mat4 getModelMatrix();
        /*
        float getRotX();
        float getRotY();
        float getRotZ();

        float getX();
        float getY();
        float getZ();

        float getScaleX();
        float getScaleY();
        float getScaleZ();
        */
        void* getData();

        virtual FE_Object* setData(void*)=0;
        virtual FE_Object* setPos(FE_Vec4)=0;
        virtual FE_Object* setRotEuler(FE_Vec4)=0;
        virtual FE_Object* setRot(FE_Vec4)=0;
        virtual FE_Object* setScale(FE_Vec4)=0;

        virtual FE_Object* setX(float)=0;
        virtual FE_Object* setY(float)=0;
        virtual FE_Object* setZ(float)=0;
        virtual FE_Object* setRotX(float)=0;
        virtual FE_Object* setRotY(float)=0;
        virtual FE_Object* setRotZ(float)=0;
        virtual FE_Object* setScaleX(float)=0;
        virtual FE_Object* setScaleY(float)=0;
        virtual FE_Object* setScaleZ(float)=0;
        std::string name;

    protected:

        void updateDeltaAttributes();

        void internalSetPos(FE_Vec4);
        void internalSetRot(FE_Vec4);
        void internalSetRot(FE_Quat);
        void internalSetScale(FE_Vec4);

        void internalSetX(float);
        void internalSetY(float);
        void internalSetZ(float);

        void internalSetRotX(float);
        void internalSetRotY(float);
        void internalSetRotZ(float);

        void internalSetScaleX(float);
        void internalSetScaleY(float);
        void internalSetScaleZ(float);

        FE_Vec4 pos;
        FE_Vec4 rot;
        FE_Vec4 sca;
        FE_Quat quat_rot;

        FE_Vec4 delta_pos;
        FE_Vec4 delta_rot;
        FE_Vec4 delta_sca;
        FE_Quat delta_quat;

        bool rotation_type;
        void* data;


    private:
};

#endif // FE_OBJECT_H
