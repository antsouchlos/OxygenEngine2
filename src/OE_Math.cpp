#include <OE_Math.h>

using namespace std;

std::vector<float> OE_Mat4x4ToSTDVector(OE_Mat4x4 matrix){
    vector<float> output; output.reserve(16);
    
    float* mat = glm::value_ptr(matrix);
    
    for (int i=0; i < 4; i++){
        for (int j=0; j<4; j++){
            output.push_back(mat[4*i+j]);
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
