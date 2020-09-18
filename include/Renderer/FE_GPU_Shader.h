#ifndef FE_GPU_SHADER_H
#define FE_GPU_SHADER_H
#include "FE_Libs.h"

enum FE_GPU_SHADER_TYPE{

    FE_GPU_VERTEX_SHADER,
    FE_GPU_PIXEL_SHADER,
    FE_GPU_GEOMETRY_SHADER,
    FE_GPU_TESS_HULL_SHADER,
    FE_GPU_TESS_DOMAIN_SHADER,
    FE_GPU_NO_SHADER
};

class FE_GPU_Shader
{
    public:
        FE_GPU_Shader();
        virtual ~FE_GPU_Shader();
        string data;
        FE_GPU_SHADER_TYPE type;

        //opengl specific
        GLuint id;
    protected:
    private:
};

#endif // FE_GPU_SHADER_H
