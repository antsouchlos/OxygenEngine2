#include "FE_Material.h"

FE_Material::FE_Material()
{
     createMutex();
}

FE_Material::~FE_Material()
{
    textureNames.clear();
}

vector<GLfloat> FE_Material::getMaterialData(){
    vector<GLfloat> output;
    output.push_back(this->dif_r);
    output.push_back(this->dif_g);
    output.push_back(this->dif_b);
    output.push_back(this->dif_a);

    output.push_back(this->scol_r);
    output.push_back(this->scol_g);
    output.push_back(this->scol_b);
    output.push_back(this->scol_a);

    output.push_back(this->alpha);
    output.push_back(this->specular_hardness);
    output.push_back(this->specular_intensity);
    output.push_back(0.0);

    output.push_back(this->illuminosity.x);
    output.push_back(this->illuminosity.y);
    output.push_back(this->illuminosity.z);
    output.push_back(this->illuminosity.w);

    output.push_back(this->translucency.x);
    output.push_back(this->translucency.y);
    output.push_back(this->translucency.z);
    output.push_back(this->translucency.w);
    return output;
}
bool FE_Material::operator==(const FE_Material other){

    bool output = true;
    output = output && (this->shadeless == other.shadeless);

    output = output && (this->dif_r == other.dif_r);
    output = output && (this->dif_g == other.dif_g);
    output = output && (this->dif_b == other.dif_b);
    output = output && (this->dif_a == other.dif_a);

    output = output && (this->scol_r == other.scol_r);
    output = output && (this->scol_g == other.scol_g);
    output = output && (this->scol_b == other.scol_b);
    output = output && (this->scol_a == other.scol_a);

    output = output && (this->alpha == other.alpha);
    output = output && (this->specular_hardness == other.specular_hardness);
    output = output && (this->specular_intensity == other.specular_intensity);

    output = output && (this->illuminosity == other.illuminosity);
    output = output && (this->translucency == other.translucency);

    output = output && (this->cube_map == other.cube_map);
    output = output && (this->cm_resolution == other.cm_resolution);

    return output;
}

//DONE
FE_Material* FE_Material::setShadeless(bool a_value){
    lockMutex();
    shadeless = a_value;
    unlockMutex();
    return this;
}
//DONE
bool FE_Material::getShadeless(){
    lockMutex();
    auto output = shadeless;
    unlockMutex();
    return output;
}
//DONE
FE_Material* FE_Material::setDiffuse(FE_Vec4 a_value){
    lockMutex();
    this->dif_r = a_value.x;
    this->dif_g = a_value.y;
    this->dif_b = a_value.z;
    this->dif_a = a_value.w;
    unlockMutex();
    return this;
}
//DONE
FE_Vec4 FE_Material::getDiffuse(){
    lockMutex();
    auto output = FE_Vec4(dif_r, dif_g, dif_b, dif_a);
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::setAlpha(float a_value){
     lockMutex();
    alpha = a_value;
    unlockMutex();
    return this;
}
//DONE
float FE_Material::getAlpha(){
    lockMutex();
    auto output = alpha;
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::setSpecularColor(FE_Vec4 a_value){
    lockMutex();
    this->scol_r = a_value.x;
    this->scol_g = a_value.y;
    this->scol_b = a_value.z;
    this->scol_a = a_value.w;
    unlockMutex();
    return this;
}
//DONE
FE_Vec4 FE_Material::getSpecularColor(){
    lockMutex();
    auto output = FE_Vec4(scol_r, scol_g, scol_b, scol_a);
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::setSpecularIntensity(float a_value){
    lockMutex();
    specular_intensity = a_value;
    unlockMutex();
    return this;
}
//DONE
float FE_Material::getSpecularIntensity(){
    lockMutex();
    auto output = specular_intensity;
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::setSpecularHardness(float a_value){
    lockMutex();
    specular_hardness = a_value;
    unlockMutex();
    return this;
}
//DONE
float FE_Material::getSpecularHardness(){
    lockMutex();
    auto output = specular_hardness;
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::setTranslucency(FE_Vec4 a_value){
    lockMutex();
    translucency = a_value;
    unlockMutex();
    return this;
}
//DONE
FE_Vec4 FE_Material::getTranslucency(){
    lockMutex();
    auto output = translucency;
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::setIlluminosity(FE_Vec4 a_value){
    lockMutex();
    illuminosity = a_value;
    unlockMutex();
    return this;
}
//DONE
FE_Vec4 FE_Material::getIlluminosity(){
    lockMutex();
    auto output = illuminosity;
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::setCubeMap(bool a_value){
    lockMutex();
    cube_map = a_value;
    unlockMutex();
    return this;
}
//DONE
bool FE_Material::getCubeMap(){
    lockMutex();
    auto output = cube_map;
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::setCubeMapResolution(int a_value){
    lockMutex();
    cm_resolution = a_value;
    unlockMutex();
    return this;
}
//DONE
int FE_Material::getCubeMapResolution(){
    lockMutex();
    auto output = cm_resolution;
    unlockMutex();
    return output;
}

//DONE
FE_Material* FE_Material::assignTexture(string a_value){
    lockMutex();
    textureNames.push_back(a_value);
    unlockMutex();
    return this;
}
//DONE
FE_Material* FE_Material::removeTexture(string a_value){
    lockMutex();
    for(unsigned int x=0; x< textureNames.size(); x++)
    if(textureNames[x] == a_value)
        textureNames.erase(textureNames.begin() + x);
    unlockMutex();
    return this;
}
