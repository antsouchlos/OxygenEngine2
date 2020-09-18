#ifndef FE_TEXTURE_H
#define FE_TEXTURE_H
#include "FE_Libs.h"

using namespace std;
class FE_Texture: public FE_THREAD_SAFETY_OBJECT
{
    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_Texture();
        ~FE_Texture();

        FE_Texture* setPath(string);
        string getPath();

        FE_Texture* setType(string);
        string getType();

        FE_Texture* setStencil(string);
        string getStencil();

        FE_Texture* setUVMap(string);
        string getUVMap();

        FE_Texture* setStencilIndex(int);
        int getStencilIndex();

        std::string name;

    protected:
        std::string path, type, stencil_name;
        // v1.2 {
        std::string uv_map;
        //v1.2 }
        int stencil_num;
    private:
};

#endif // FE_TEXTURE_H
