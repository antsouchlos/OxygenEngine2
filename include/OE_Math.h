#ifndef OE_MATH_H_INCLUDED
#define OE_MATH_H_INCLUDED

#include <types/OE_TypesBase.h>

//#include <glm/glm.hpp>
#define GLM_FORCE_SSE3
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <glm/ext/vector_trigonometric.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/ext/matrix_float4x4.hpp>             
#include <glm/ext/matrix_transform.hpp> 
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <glm/gtc/noise.hpp>

#define FE_EPSILON 0.000001f
#define OE_EPSILON 0.000001f

// This is done, so that in the event we want to change the math library
// it will be easy
typedef glm::vec4 OE_Vec4;
typedef glm::vec3 OE_Vec3;
typedef glm::mat4 OE_Mat4x4;
typedef glm::quat OE_Quat;

#define OE_Pi2         glm::half_pi

#define OE_Abs         glm::abs
#define OE_Pow         glm::pow
#define OE_Sqrt        glm::sqrt

//#define OE_Perspective  glm::perspective
#define OE_Rotate       glm::rotate
#define OE_Normalize    glm::normalize
#define OE_Cross        glm::cross
#define OE_Dot          glm::dot
#define OE_Translate    glm::translate
#define OE_Identity     glm::identity
#define OE_Det          glm::determinant
#define OE_Transpose    glm::transpose
#define OE_Quat2Mat4x4  glm::toMat4

#define OE_2Euler       glm::eulerAngles
#define OE_Slerp        glm::slerp
#define OE_Lerp         glm::lerp

std::vector<float> OE_Mat4x4ToSTDVector(OE_Mat4x4);
OE_Mat4x4 OE_Perspective(float, float, float, float);


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
