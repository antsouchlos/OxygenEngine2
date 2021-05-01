#ifndef OE_MATH_H_INCLUDED
#define OE_MATH_H_INCLUDED

#include <types/OE_TypesBase.h>

//#include <glm/glm.hpp>
#define GLM_FORCE_SSE3
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
//#include <glm/gtc/constants.hpp>
//#include <glm/gtc/type_ptr.hpp>

//#include <glm/ext/vector_trigonometric.hpp>
//#include <glm/gtc/matrix_inverse.hpp>
//#include <glm/gtc/matrix_access.hpp>
//#include <glm/ext/matrix_clip_space.hpp>
//#include <glm/ext/matrix_projection.hpp>

#include <glm/gtc/quaternion.hpp>
//#include <glm/ext/quaternion_float.hpp>
//#include <glm/ext/quaternion_common.hpp>
//#include <glm/ext/quaternion_geometric.hpp>
//#include <glm/ext/quaternion_transform.hpp>

//#include <glm/gtc/noise.hpp>

#define FE_EPSILON 0.000001f
#define OE_EPSILON 0.000001f

namespace oe{
    const auto PI = glm::pi<float>();
};

// This is done, so that in the event we want to change the math library
// it will be easy

class OE_Vec4 : public glm::vec4{
public:
    
    using glm::vec4::vec4;
    OE_Vec4 operator + (const OE_Vec4&);
    OE_Vec4 operator - (const OE_Vec4&);
    
};

class OE_Vec3 : public glm::vec3{
public:
    
    using glm::vec3::vec3;
    
};

class OE_Mat4x4 : public glm::mat4{
public:
    using glm::mat4::mat4;
    
    OE_Mat4x4 operator * (const OE_Mat4x4&);
    OE_Vec4 operator * (const OE_Vec4&);
    
    
};

class OE_Mat3x3: public glm::mat3{
public:
    
    using glm::mat3::mat3;
    
    OE_Vec3 operator * (const OE_Vec3&);
};

class OE_Quat : public glm::quat{
public: 
    
    using glm::quat::quat;
    
    OE_Quat(glm::quat);
    
    OE_Quat operator * (const OE_Quat&);
};//*/

/*

#define OE_Abs         glm::abs
#define OE_Pow         glm::pow
#define OE_Sqrt        glm::sqrt

#define OE_Rotate       glm::rotate
//#define OE_Scale        glm::scale

#define OE_Cross        glm::cross
#define OE_Dot          glm::dot

#define OE_Identity     glm::identity
#define OE_Det          glm::determinant
#define OE_Transpose    glm::transpose*/

OE_Mat4x4 OE_Translate(OE_Mat4x4, OE_Vec3);
OE_Mat4x4 OE_Scale(OE_Mat4x4, OE_Vec3);

OE_Mat4x4 OE_Quat2Mat4x4(OE_Quat);
OE_Vec4 OE_Normalize(OE_Vec4);
OE_Quat OE_Normalize(OE_Quat);

float OE_Length(OE_Vec3);
float OE_Length(OE_Vec4);

/*#define OE_Mat2Euler       glm::eulerAngles
#define OE_Slerp        glm::slerp
#define OE_Lerp         glm::lerp*/

std::vector<float> OE_Mat4x4ToSTDVector(OE_Mat4x4);
OE_Mat4x4 OE_Perspective(float, float, float, float);

OE_Vec4   OE_GetClipCoords(OE_Mat4x4, OE_Vec4);

OE_Quat OE_QuatFromAxisAngle(float, float, float, float);

std::vector<float> OE_GetBoundingBoxVertexBuffer(float, float, float, float, float, float);

std::vector<float> OE_GetBoundingSphereVertexBuffer(float, float, size_t);
std::vector<uint32_t> OE_GetBoundingSphereIndexBuffer(float, float, size_t);

#endif // FMATH_H_INCLUDED
