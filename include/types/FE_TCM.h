#ifndef FE_TCM_H
#define FE_TCM_H
#include "FE_Libs.h"
#include "FMath.h"
using namespace std;

/*class FE_textureCombo{
   public:
    FE_textureCombo();
    ~FE_TextureCombo();
    int indices[2];
    vector<string> modes;
};*/
class FE_TCM : public FE_THREAD_SAFETY_OBJECT
{
    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_TCM();
        ~FE_TCM();
        bool init();
        bool destroy();

        vector<string> getTextureNames();

        FE_TCM* assignTexture(string, string, string);
        FE_TCM* removeTexture(string);

        FE_Vec4 getIntensity();
        FE_TCM* setIntensity(FE_Vec4);

        FE_TCM* setMode(string);
        string getMode();

        string name;

    protected:


        vector<string> textureNames;

        //v1.2 {
        FE_Vec4 value;
        vector<string> tex_indices;
        vector<unsigned int> modes;
        //v1.2 }
        string combine_mode;
    private:
};

#endif // FE_TCM_H
