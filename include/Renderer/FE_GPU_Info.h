#ifndef FE_GPU_INFO_H
#define FE_GPU_INFO_H
#include "FE_GPU_Program.h"

//struct FE_GPU_VertexBuffer{string name;};
//struct FE_GPU_Program{string name;};
//struct FE_GPU_VertexLayout{string name;};
//struct FE_GPU_Uniform{string name;};
//struct FE_GPU_Command{string name;};
//struct FE_Shader{string name;};

//struct FE_VertexData;
//struct FE_GPU_VertexLayoutInput{string name;};

class FE_GPU_Info
{
    public:
        FE_GPU_Info();
        virtual ~FE_GPU_Info();
        size_t findVBO(string);
        size_t findVAO(string);
        size_t findProgram(string);
        size_t findUniform(string);

        vector<FE_GPU_VertexBuffer> vbos;
        vector<FE_GPU_VertexLayout> vaos;
        vector<FE_GPU_Program> programs;
        vector<FE_GPU_Uniform> ubos;

        array<string, 16> ubo_status;

        FE_RENDERTYPE rend_type;
    protected:
    private:
};

#endif // FE_GPU_INFO_H
