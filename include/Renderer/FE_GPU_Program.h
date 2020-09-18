#ifndef FE_GPU_PROGRAM_H
#define FE_GPU_PROGRAM_H

#include "FE_GPU_Shader.h"
#include "FE_GPU_Uniform.h"

struct FE_GPU_ProgramUniformState{
    string name;
    GLint slot;
};
typedef FE_GPU_ProgramUniformState FE_GPU_PUS ; // for convenience

struct FE_GPU_ProgramShaders{
    FE_GPU_Shader vs;
    FE_GPU_Shader gs;
    FE_GPU_Shader tcs;
    FE_GPU_Shader tes;
    FE_GPU_Shader fs;
};


/**
  * Cross-platform way to submit shader program commands to the GPU
  */

class FE_GPU_CreateProgramCMD : public FE_GPU_Command{
public:
    FE_GPU_CreateProgramCMD();
    ~FE_GPU_CreateProgramCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif
};

class FE_GPU_SetProgramShaderCMD : public FE_GPU_Command{
public:
    FE_GPU_SetProgramShaderCMD();
    ~FE_GPU_SetProgramShaderCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

    FE_GPU_Shader shader;
};

class FE_GPU_SetupProgramCMD : public FE_GPU_Command{
public:
    FE_GPU_SetupProgramCMD();
    ~FE_GPU_SetupProgramCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif
};

class FE_GPU_SetProgramVariableCMD : public FE_GPU_Command{
public:
    FE_GPU_SetProgramVariableCMD();
    ~FE_GPU_SetProgramVariableCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif


    FE_GPU_ProgramUniformState ubo_state;
};

class FE_GPU_DestroyProgramCMD : public FE_GPU_Command{
public:
    FE_GPU_DestroyProgramCMD();
    ~FE_GPU_DestroyProgramCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

};

/**
  * Cross-platform way to submit draw commands to the GPU
  */

class FE_GPU_DrawCommand : public FE_GPU_Command{
public:
    FE_GPU_DrawCommand();
    ~FE_GPU_DrawCommand();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

    string vao;
    GLuint voffset;
    GLuint vcount;
    GLuint vinstances;

};

/** Internal structure:
  * -API only interests
  *  the authors of Fluorin Engine
  * -simplifies potential rewrite to another
  *  graphics API
  */

class FE_GPU_Program : FE_THREAD_SAFETY_OBJECT
{
public:
    /// non-standard methods
    size_t hasUniform(string);
    size_t hasUniformSetup(string);

    /// universal members
    vector<FE_GPU_PUS> uniform_indices;
    FE_GPU_ProgramShaders shaders;
    string name;
    vector<string> uniforms;
    /// OpenGL specific
    GLuint id;
    bool created_program;
};

#endif // FE_GPU_PROGRAM_H
