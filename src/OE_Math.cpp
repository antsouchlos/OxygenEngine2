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


int FE_Math::abs(int x){
    return x ? x>=0 : -x;
}

double FE_Math::abs(double x){
    return x ? x>=0 : -x;
}

float FE_Math::abs(float x){
    return x ? x>=0 : -x;
}

int internal_pow(int x, unsigned int y){
    int output = FE_Math::abs(x);
    for(unsigned int i=0; i< y; i++){
        output *= x;
    }
    return output;
}

double internal_pow(double x, unsigned int y){
    double output = x;
    for(unsigned int i=0; i< y; i++){
        output *= x;
    }
    return output;
}

double internal_root(double x, int y){
    return 1.0;
}

double FE_Math::pow(double value, double exponent){

    double root = 1.0;
    double power = exponent;
    // make a ratio out of the input
    for(unsigned int i=0; i< 10; i++){
        power *=10; root *= 10;
    }
    return internal_pow(value, (int)power)/ internal_root(value, (int)root);

}


//cross vector product
FE_Vec4 FE_Math::cross(FE_Vec4 u, FE_Vec4 v){
    //UyVz - UzVy, UzVx - UxVz, UxVy - UyVx
    return FE_Vec4(u.y*v.z - u.z*v.y, u.z*v.x - u.x*v.z, u.x*v.y - u.y*v.x, 1);

}

// returns view matrix
FE_Mat4 FE_Math::view(FE_Vec4 cam_pos, FE_Vec4 cam_quaternion){

    FE_Mat4 output;
    output.setRot(cam_quaternion);
    output.z.w = -cam_pos.z;
    output.y.w = -cam_pos.y;
    output.x.w = -cam_pos.x;
    return output;
}

FE_Mat4 FE_Math::perspective(float fov, float aspect, float near, float far){

    float fov_rad = FE_Math::radians(fov);
	float range = std::tan (fov_rad / 2.0f) * near;
	//std::cout << "range: " << range << std::endl;
	float sx = (2.0f * near) / (range * aspect + range * aspect);
	float sy = near / range;
	float sz = -(far + near) / (far - near);
	float pz = -(2.0f * far * near) / (far - near);
    FE_Mat4 output = FE_Mat4(0.0);
    output.x.x = sx;
	output.y.y = sy;
	output.z.z = sz;
	output.w.z = pz;
	output.z.w = -1.0f;
	//std::cout << output.print(false) << std::endl;
	return output;

}

FE_Vec4 FE_Math::normalize(FE_Vec4 input){

    float length = std::sqrt(input.x*input.x + input.y*input.y + input.z*input.z);
    return FE_Vec4(input.x/length, input.y/length, input.z/length, 1);
}

FE_Quat FE_Math::normalize(FE_Quat input){
    float length = std::sqrt(input.x*input.x + input.y*input.y + input.rad*input.rad + input.z*input.z);
    return FE_Quat(input.rad/length, input.x/length, input.y/length, input.z/length, true);
}

float FE_Math::radians(float deg){
    return (2*3.1415926535*deg)/360;}
float FE_Math::degrees(float rad){
    return (360*rad)/(2*3.1415926535);}

float FE_Math::dot(FE_Quat a, FE_Quat b){
        return a.x*b.x + a.y*b.y + a.z*b.z + a.rad*b.rad;
}

FE_Quat FE_Math::slerp(FE_Quat a, FE_Quat b, float t){


    return FE_Quat();
}
/***********************VECTOR**********************/

FE_Vec4::FE_Vec4(){
    x = 0.0f; y = 0.0f;
    z = 0.0f, w = 0.0f;
}

FE_Vec4::FE_Vec4(float xa, float ya, float za, float wa){
    x=xa; y=ya;
    z=za; w=wa; axis_angle = false;
}
FE_Vec4::FE_Vec4(float xa, float ya, float za, float wa, bool aa){
    x=xa; y=ya;
    z=za; w=wa; axis_angle = aa;
}

std::array<float, 4> FE_Vec4::toArray(){
    return std::array<float, 4>{x, y, z, w};
}

//destructor
FE_Vec4::~FE_Vec4(){}


// overload operators
FE_Vec4 FE_Vec4::operator +(float other){
   return FE_Vec4( x + other, y + other, z + other, w + other);
}

FE_Vec4 FE_Vec4::operator -(float other){
    return FE_Vec4( x - other, y - other, z - other, w - other);
}

FE_Vec4 FE_Vec4::operator *(float other){
    return FE_Vec4( x * other, y * other, z * other, w * other);
}

FE_Vec4 FE_Vec4::operator +(FE_Vec4 other){
   return FE_Vec4( x + other.x, y + other.y, z + other.z, w + other.w);
}

FE_Vec4 FE_Vec4::operator -(FE_Vec4 other){
    return FE_Vec4( x - other.x, y - other.y, z - other.z, w - other.w);
}

float FE_Vec4::operator *(FE_Vec4 other){
    return  x * other.x + y * other.y + z * other.z + w * other.w;
}

bool FE_Vec4::isAxisAngle(){
    return  this->axis_angle;
}

bool FE_Vec4::operator==(FE_Vec4 other){
    bool output = true;
    output = output && fabs(x- other.x) < FE_EPSILON;
    output = output && fabs(y- other.y) < FE_EPSILON;
    output = output && fabs(z- other.z) < FE_EPSILON;
    output = output && fabs(w- other.w) < FE_EPSILON;
    return output;
}
std::string FE_Vec4::print(bool value){
    std::string output;
    if(value){
        output = "FE_Vec4(";
        output += " " + std::to_string(x);
        output += " " + std::to_string(y);
        output += " " + std::to_string(z);
        output += " " + std::to_string(w) + ")";

    }
    else{
        output += " " + std::to_string(x);
        output += " " + std::to_string(y);
        output += " " + std::to_string(z);
        output += " " + std::to_string(w) + ")";
    }
    return output;
}
/******************************************************/


/************************MATRIX************************/
// constructor
FE_Mat4::FE_Mat4(){
    x = FE_Vec4(1.0f, 0.0f, 0.0f, 0.0f); y = FE_Vec4(0.0f, 1.0f, 0.0f, 0.0f);
    z = FE_Vec4(0.0f, 0.0f, 1.0f, 0.0f); w = FE_Vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

FE_Mat4::FE_Mat4(float value){
    x = FE_Vec4(value, value, value, value); y = FE_Vec4(value, value, value, value);
    z = FE_Vec4(value, value, value, value); z = FE_Vec4(value, value, value, value);
}
FE_Mat4::FE_Mat4(FE_Vec4 xa, FE_Vec4 ya, FE_Vec4 za, FE_Vec4 wa){

    x = xa; y = ya;
    z = za; w = wa;
}

FE_Mat4::~FE_Mat4(){};

std::array<float, 16> FE_Mat4::toArray(){
    std::array<float, 16> result = {x.x, x.y, x.z, x.w, y.x, y.y, y.z, y.w, z.x, z.y, z.z, z.w, w.x, w.y, w.z, w.w};
    return result;
}

// overload operators
FE_Mat4 FE_Mat4::operator +(FE_Mat4 other){
    return FE_Mat4(x + other.x, y + other.y, z + other.z, w + other.w);
}

FE_Mat4 FE_Mat4::operator -(FE_Mat4 other){
    return FE_Mat4(x - other.x, y - other.y, z - other.z, w - other.w);
}

FE_Mat4 FE_Mat4::operator *(float other){
    return FE_Mat4(x * other, y * other, z * other, w * other);
}

FE_Vec4 FE_Mat4::operator *(FE_Vec4 other){
    return FE_Vec4(x * other, y * other, z * other, w * other);
}

bool FE_Mat4::operator ==(FE_Mat4 other){
    bool output = true;
    output = output && (x == other.x);
    output = output && (y == other.y);
    output = output && (z == other.z);
    output = output && (w == other.w);
    return output;
}

FE_Mat4 FE_Mat4::transpose(){

        return FE_Mat4(FE_Vec4(x.x, y.x, z.x, w.x),
                     FE_Vec4(x.y, y.y, z.y, w.y),
                     FE_Vec4(x.z, y.z, z.z, w.z),
                     FE_Vec4(x.w, y.w, z.w, w.w));
}
FE_Mat4 FE_Mat4::operator *(FE_Mat4 other){
    return FE_Mat4(*this * other.transpose().x,*this * other.transpose().y, *this * other.transpose().z, *this * other.transpose().w);
}

void FE_Mat4::setPos(FE_Vec4 pos){
    this->x.w = pos.x;
    this->y.w = pos.y;
    this->z.w = pos.z;
}

void FE_Mat4::setRot(FE_Vec4 euler){

	float x = FE_Math::radians(euler.x);
	float y = FE_Math::radians(euler.y);
	float z = FE_Math::radians(euler.z);

    this->x.x = std::cos(y)*std::cos(z);
    this->y.y = std::cos(x)*std::cos(z);
    this->z.z = std::cos(x)*std::cos(y);

    this->x.y = std::sin(z);
    this->x.z = -std::sin(y);

    this->y.x = -std::sin(z);
    this->y.z = std::sin(x);

    this->z.x = std::sin(y);
    this->z.y = -std::sin(x);
	//this->z.w = 0.0f;
	//this->w.x = 0.0f;
	//this->w.y = 0.0f;
	//this->w.z = 0.0f;
	//this->w.w = 1.0f;
}

void FE_Mat4::setRot(FE_Quat quat){
    auto rot = quat.toMatrix();

    this->x.x = rot.x.x;
    this->x.y = rot.x.y;
    this->x.z = rot.x.z;
    this->y.x = rot.y.x;
    this->y.y = rot.y.y;
    this->y.z = rot.y.z;
    this->z.x = rot.z.x;
    this->z.y = rot.z.y;
    this->z.z = rot.z.z;
}

std::string FE_Mat4::print(bool value){
    std::string output;
    if(value){
        output+= x.print(false) + " ";
        output+= y.print(false) + " ";
        output+= z.print(false) + " ";
        output+= w.print(false) + " ";
    }
    else{
        output+= "FE_Mat4(\n";
        output+= x.print(false) + "\n";
        output+= y.print(false) + "\n";
        output+= z.print(false) + "\n";
        output+= w.print(false) + ")";
    }
    return output;
}
/******************************************************/



/******************************************************/
///quaternions

FE_Quat::FE_Quat(){
    x = 0.0f; y = 0.0f;
    z = 0.0f; rad = 0.0f;
}

FE_Quat::FE_Quat(FE_Vec4 v){
    rad = cos (FE_Math::radians(v.w) / 2.0f);
    x = sin (FE_Math::radians(v.w) / 2.0f)*v.x;
    y = sin (FE_Math::radians(v.w) / 2.0f)*v.y;
    z = sin (FE_Math::radians(v.w) / 2.0f)*v.z;
}

void FE_Quat::fromAxisAngle(FE_Vec4 v){
    rad = cos (FE_Math::radians(v.w) / 2.0f);
    x = sin (FE_Math::radians(v.w) / 2.0f)*v.x;
    y = sin (FE_Math::radians(v.w) / 2.0f)*v.y;
    z = sin (FE_Math::radians(v.w) / 2.0f)*v.z;
}

FE_Quat::FE_Quat(float rada, float xa, float ya, float za, bool place){
    rad = (rada);
    x = xa;
    y = ya;
    z = za;
}

FE_Quat FE_Quat::operator + (FE_Quat other){
    FE_Quat result = FE_Quat(rad + other.rad, x+other.x, y+other.y, z+other.z, true);
    return result;
}

FE_Quat FE_Quat::operator * (float k) {
    return FE_Quat(k*rad, k*x, k*y, k*z, true);
}

FE_Quat FE_Quat::operator * (FE_Quat k){
    FE_Quat output;

    output.rad = rad*k.rad - x*k.x - y*k.y - z*k.z;
    output.x   = k.rad*x + k.x*rad - k.y*z + k.z*y;
    output.y   = k.rad*y + k.x*z + k.y*rad - k.z*x;
    output.z   = k.rad* z - k.x*y + k.y*x + k.z*rad;

    return FE_Math::normalize(output);
}

FE_Quat FE_Quat::makeQuat(){
    FE_Quat quat;
    quat.rad = cos (FE_Math::radians(rad) / 2.0f);
    quat.x = sin (FE_Math::radians(rad) / 2.0f)*x;
    quat.y = sin (FE_Math::radians(rad) / 2.0f)*y;
    quat.z = sin (FE_Math::radians(rad) / 2.0f)*z;
    quat.quatted = true;
    return quat;
}

void FE_Quat::fromEuler(FE_Vec4 euler){
    rad = cos(euler.x/2.0) * cos(euler.y/2.0) * cos(euler.z/2.0) + sin(euler.x/2.0) * sin(euler.y/2.0) * sin(euler.z/2.0);
    x = sin(euler.x/2.0) * cos(euler.y/2.0) * cos(euler.z/2.0) + sin(euler.x/2.0) * cos(euler.y/2.0) * cos(euler.z/2.0);
    y = cos(euler.x/2.0) * sin(euler.y/2.0) * cos(euler.z/2.0) + cos(euler.x/2.0) * sin(euler.y/2.0) * cos(euler.z/2.0);
    z = cos(euler.x/2.0) * cos(euler.y/2.0) * sin(euler.z/2.0) + cos(euler.x/2.0) * cos(euler.y/2.0) * sin(euler.z/2.0);
}

FE_Vec4 FE_Quat::toAxisAngle(){
    if(sqrt(1-rad*rad) > 0.001f) return FE_Vec4( x/ sqrt(1-rad*rad), y/ sqrt(1-rad*rad), z/ sqrt(1-rad*rad), 2*acos(rad), true);
    else                         return FE_Vec4(1.0, x, y, z);
}

FE_Mat4 FE_Quat::toMatrix(){
    FE_Mat4 result;
    FE_Math::normalize(*this);
    result.x.x = 1.0f - 2.0f * y * y - 2.0f * z * z;
	result.x.y =	2.0f * x * y + 2.0f * rad * z;
	result.x.z =	2.0f * x * z - 2.0f * rad * y;
	//this->x.w = 0.0f;
	result.y.x = 2.0f * x * y - 2.0f * rad * z;
	result.y.y = 1.0f - 2.0f * x * x - 2.0f * z * z;
	result.y.z = 2.0f * y * z + 2.0f * rad * x;
	//this->y.w = 0.0f;
	result.z.x = 2.0f * x * z + 2.0f * rad * y;
	result.z.y = 2.0f * y * z - 2.0f * rad * x;
	result.z.z = 1.0f - 2.0f * x * x - 2.0f * y * y;

    return result;
}


