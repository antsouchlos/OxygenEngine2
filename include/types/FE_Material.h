#ifndef FE_MATERIAL_H
#define FE_MATERIAL_H
#include "FE_Libs.h"
#include "FMath.h"
using namespace std;
class FE_Material:  public FE_THREAD_SAFETY_OBJECT
{
    friend class FE_MaterialData;

    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_Material();
        ~FE_Material();

        bool operator==(const FE_Material);
        FE_Material* setShadeless(bool);
        bool getShadeless();

        FE_Material* setDiffuse(FE_Vec4);
        FE_Vec4 getDiffuse();

        FE_Material* setAlpha(float);
        float getAlpha();

        FE_Material* setSpecularColor(FE_Vec4);
        FE_Vec4 getSpecularColor();

        FE_Material* setSpecularIntensity(float);
        float getSpecularIntensity();

        FE_Material* setSpecularHardness(float);
        float getSpecularHardness();

        FE_Material* setTranslucency(FE_Vec4);
        FE_Vec4 getTranslucency();

        FE_Material* setIlluminosity(FE_Vec4);
        FE_Vec4 getIlluminosity();

        FE_Material* setCubeMap(bool);
        bool getCubeMap();

        FE_Material* setCubeMapResolution(int);
        int getCubeMapResolution();

        FE_Material* assignTexture(string);
        FE_Material* removeTexture(string);

        string name;
    protected:



        bool shadeless = false;

        // v1.2(changed type) {
        float dif_r, dif_g, dif_b, dif_a;
        float scol_r, scol_g, scol_b, scol_a;
        //v1.2}

        float alpha;
        float specular_intensity, specular_hardness;

        //v1.2 {
        FE_Vec4 translucency;
        FE_Vec4 illuminosity;
        bool cube_map;
        int cm_resolution;
        //v1.2 }

        vector<string> textureNames;

        vector<GLfloat> getMaterialData();
    private:
};

#endif // FE_MATERIAL_H
