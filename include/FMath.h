#include "FE_EventHandler.h"
#ifndef FMATH_H_INCLUDED
#define FMATH_H_INCLUDED

#define FE_EPSILON 0.00001f
 class FMath
{
    public:
        FMath();
        ~FMath();

    protected:
    private:
};
class FE_Vec4;
 class FE_Mat4;
class FE_Quat;

 namespace FE_Math{

    float  pow(float, float);
    int    pow(int, int);
    double pow(double, double);
    double  abs(double);
    float   abs(float);
    int     abs(int);
    float   radians(float);
    float   degrees(float);

    FE_Vec4    cross(FE_Vec4, FE_Vec4);
    FE_Mat4    view(FE_Vec4, FE_Vec4);
    FE_Mat4    perspective(float, float, float, float);
    FE_Vec4    normalize(FE_Vec4);
    FE_Quat    normalize(FE_Quat);
    float      dot(FE_Quat, FE_Quat);
    FE_Quat    slerp(FE_Quat, FE_Quat, float);
    //long double pi = 3.1415926535;
}

class FE_Vec4{
    public:
        float x;
        float y;
        float z;
        float w;
        bool axis_angle;
        FE_Vec4();
        FE_Vec4(float, float, float, float);
        FE_Vec4(float, float, float, float, bool);
        ~FE_Vec4();
        bool isAxisAngle();
        std::array<float, 4> toArray();
        FE_Vec4 operator +(FE_Vec4);
        FE_Vec4 operator -(FE_Vec4);
        FE_Vec4 operator +(float);
        FE_Vec4 operator -(float);
        FE_Vec4 operator *(float);
        bool operator ==(FE_Vec4);
        float operator *(FE_Vec4);
        std::string   print(bool);

};

class FE_Mat4{
public:
    FE_Vec4 x;
    FE_Vec4 y;
    FE_Vec4 z;
    FE_Vec4 w;

    FE_Mat4();
    FE_Mat4(float);
    FE_Mat4(FE_Vec4, FE_Vec4, FE_Vec4, FE_Vec4);
    ~FE_Mat4();
    std::array<float, 16> toArray();
    std::string           print(bool);
    FE_Mat4 operator +(FE_Mat4);
    FE_Mat4 operator -(FE_Mat4);
    FE_Mat4 operator *(float);
    FE_Vec4 operator *(FE_Vec4);
    FE_Mat4 operator *(FE_Mat4);
    bool    operator ==(FE_Mat4);
    FE_Mat4 transpose();
    void  setRot(FE_Vec4);
    void setRot(FE_Quat);
    void  setPos(FE_Vec4);
    void setScale(FE_Vec4);
};

class FE_Quat{
public:
    float rad, x, y, z;
    bool quatted;

    FE_Quat();
    FE_Quat(FE_Vec4);
    FE_Quat(float, float, float, float, bool);
    FE_Quat operator +(FE_Quat);
    FE_Quat operator *(float);
    FE_Quat operator *(FE_Quat);

    std::string  print();
    FE_Quat makeQuat();
    void setRad(float);

    void fromEuler(FE_Vec4);
    void fromAxisAngle(FE_Vec4);
    FE_Vec4 toAxisAngle();
    FE_Vec4 toEuler();
    FE_Mat4 toMatrix();
};
#endif // FMATH_H_INCLUDED
