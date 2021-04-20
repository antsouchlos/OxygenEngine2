#include <OE_Math.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/matrix_float4x4.hpp>             
#include <glm/ext/matrix_transform.hpp> 
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// basically translation from the GLM library

// overloading operators

OE_Quat::OE_Quat(glm::quat q){
    this->w = q.w;
    this->x = q.x;
    this->y = q.y;
    this->z = q.z;
    
}

OE_Vec3 OE_Mat3x3::operator * (const OE_Vec3& other){
    auto temp = static_cast<glm::mat3>(*this) * static_cast<glm::vec3>(other);
    return OE_Vec3(temp[0], temp[1], temp[2]);
}

OE_Mat4x4 OE_Mat4x4::operator * (const OE_Mat4x4& other){
    auto temp = static_cast<glm::mat4>(*this) * static_cast<glm::mat4>(other);
    return OE_Mat4x4(temp[0], temp[1], temp[2], temp[3]);
}

OE_Quat OE_Quat::operator * (const OE_Quat& other){
    auto temp = static_cast<glm::quat>(*this) * static_cast<glm::quat>(other);
    return OE_Quat(temp);
}


// math library functions


OE_Mat4x4 OE_Translate(OE_Mat4x4 mat4, OE_Vec3 vec3){
    auto temp = glm::translate(static_cast<glm::mat4>(mat4), static_cast<glm::vec3>(vec3));
    return OE_Mat4x4(temp[0], temp[1], temp[2], temp[3]);
}

OE_Mat4x4 OE_Scale(OE_Mat4x4 mat4, OE_Vec3 vec3){
    auto temp = glm::scale(static_cast<glm::mat4>(mat4), static_cast<glm::vec3>(vec3));
    return OE_Mat4x4(temp[0], temp[1], temp[2], temp[3]);
}

OE_Mat4x4 OE_Quat2Mat4x4(OE_Quat quat){
    
    auto temp = glm::toMat4(static_cast<glm::quat>(quat));
    return OE_Mat4x4(temp[0], temp[1], temp[2], temp[3]);
}

OE_Vec4 OE_Normalize(OE_Vec4 vec4){
    auto temp = glm::normalize(static_cast<glm::vec4>(vec4));
    return OE_Vec4(temp[0], temp[1], temp[2], temp[3]);
}

OE_Quat OE_Normalize(OE_Quat quat){
    auto temp = glm::normalize(static_cast<glm::quat>(quat));
    return OE_Quat(temp);
}

float OE_Length(OE_Vec3 vec3){
    return std::sqrt(std::pow(vec3[0], 2) + std::pow(vec3[1], 2) + std::pow(vec3[2], 2));
}

float OE_Length(OE_Vec4 vec4){
    return std::sqrt(std::pow(vec4[0], 2) + std::pow(vec4[1], 2) + std::pow(vec4[2], 2) + std::pow(vec4[3], 2));
}


std::vector<float> OE_Mat4x4ToSTDVector(OE_Mat4x4 matrix){
    vector<float> output; output.reserve(16);
    
    for (int i=0; i < 4; i++){
        for (int j=0; j<4; j++){
            output.push_back(matrix[i][j]);
        }
    }
    return output;
}


OE_Mat4x4 OE_Perspective(float fov, float aspect, float near, float far){

    //float fov_rad = FE_Math::radians(fov);
	float fov_rad = fov;
    float range = std::tan (fov_rad / 2.0f) * near;
	//std::cout << "range: " << range << std::endl;
	float sx = (2.0f * near) / (range * aspect + range * aspect);
	float sy = near / range;
	float sz = -(far + near) / (far - near);
	float pz = -(2.0f * far * near) / (far - near);
    OE_Mat4x4 output = OE_Mat4x4(0.0f);
    output[0][0] = sx;
	output[1][1] = sy;
	output[2][2] = sz;
	output[3][2] = pz;
	output[2][3] = -1.0f;
	//std::cout << output.print(false) << std::endl;
	return output;

}

OE_Quat OE_QuatFromAxisAngle(float a, float xx, float yy, float zz){
    float factor = std::sin( a / 2.0f );
    return OE_Normalize(OE_Quat(std::cos (a / 2.0f), xx*factor, yy*factor, zz*factor));
}

std::vector<float> OE_GetBoundingBoxVertexBuffer(float max_x, float min_x, float max_y, float min_y, float max_z, float min_z){
    
    std::vector<float> output;
    output.reserve(216);
    
    // max_x
    output.push_back(max_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    
    output.push_back(max_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(1.0f); output.push_back(0.0f);output.push_back(0.0f);
    
    // min_x
    output.push_back(min_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(-1.0f); output.push_back(0.0f);output.push_back(0.0f);
    
    // max_y
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    
    output.push_back(min_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(1.0f);output.push_back(0.0f);
    
    // min_y
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    
    output.push_back(max_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(-1.0f);output.push_back(0.0f);
    
    // max_z
    output.push_back(min_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    
    output.push_back(max_x); output.push_back(max_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(max_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(1.0f);
    
    // min_z
    output.push_back(max_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    output.push_back(min_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    
    output.push_back(min_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    output.push_back(max_x); output.push_back(max_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    output.push_back(max_x); output.push_back(min_y); output.push_back(min_z);
    output.push_back(0.0f); output.push_back(0.0f);output.push_back(-1.0f);
    
    return output;
}

std::vector<float> OE_GetBoundingSphereVertexBuffer(float r1, float r2, size_t n){
    std::vector<float> vbo;
    
    assert(n % 2 == 0);
    
    // push top vertex
    vbo.push_back(0.0f);
    vbo.push_back(0.0f);
    vbo.push_back(r1);
    
    // push top vertex normal
    vbo.push_back(0.0f);
    vbo.push_back(0.0f);
    vbo.push_back(r1);
    
    for (uint32_t k = 0; k < 2 * n; k++){
        for (uint32_t m = 1; m < n; m++){
            
            // push once for position
            vbo.push_back(r2*std::cos(M_PI / n * k)*std::sin(M_PI / n * m));
            vbo.push_back(r2*std::sin(M_PI / n * k)*std::sin(M_PI / n * m));
            vbo.push_back(r1*std::cos(M_PI / n * m));
            // and once for normal
            vbo.push_back(r2*std::cos(M_PI / n * k)*std::sin(M_PI / n * m));
            vbo.push_back(r2*std::sin(M_PI / n * k)*std::sin(M_PI / n * m));
            vbo.push_back(r1*std::cos(M_PI / n * m));
        }
    }
    
    // push bottom vertex
    vbo.push_back(0.0f);
    vbo.push_back(0.0f);
    vbo.push_back(-r1);
    
    // push bottom vertex normal
    vbo.push_back(0.0f);
    vbo.push_back(0.0f);
    vbo.push_back(-r1);
    
    return vbo;
}

std::vector<uint32_t> OE_GetBoundingSphereIndexBuffer(float r1, float r2, size_t n){
    std::vector<uint32_t> ibo;
    
    assert(n % 2 == 0);
    
    for (uint32_t k = 0; k < 2 * n; k++){
        
        ibo.push_back(0);
        ibo.push_back(1 + (n - 1) * k);
        ibo.push_back(1 + (n - 1) * ((k + 1) % (2*n)));
        
        for (uint32_t m = 1; m < n - 1; m++){
            // one square
            ibo.push_back(m + (n - 1) * k);
            ibo.push_back(m + 1 + (n - 1) * k);
            ibo.push_back(m + (n - 1) * ((k + 1) % (2*n)));
            
            ibo.push_back(m + (n - 1) * ((k + 1) % (2*n)));
            ibo.push_back(m + 1 + (n - 1) * k);
            ibo.push_back(m + 1 + (n - 1) * ((k + 1) % (2*n)));
        }
        ibo.push_back(n - 1 + (n - 1) * k);
        ibo.push_back((n - 1)*(2*n) + 1);
        ibo.push_back(n - 1 + (n - 1) * ((k + 1) % (2*n)));
    }
    return ibo;
}
