#include "FE_GPU_API.h"

typedef FE_GPU_PUS FE_GPU_ProgramUniformState; // for convenience

///FE_GPU_Info* FE_GPU_ProgramCommand::info = nullptr;
///FE_GPU_Info* FE_GPU_DrawCommand::info = nullptr;

///FE_RENDERTYPE FE_GPU_Program::rend_type = FE_OPENGL3_2;
///FE_GPU_Info* FE_GPU_Program::info = nullptr;


/// non-standard program methods

size_t FE_GPU_Program::hasUniform(string a_name){

    lockMutex();

    size_t output= uniforms.size();

    for(size_t x=0; x < this->uniforms.size(); x++)
    if(uniforms[x] ==a_name)
        output= x;

    unlockMutex();
    return output;
}
size_t FE_GPU_Program::hasUniformSetup(string a_name){
    lockMutex();

    size_t output= uniform_indices.size();

    for(size_t x=0; x < this->uniform_indices.size(); x++)
    if(uniform_indices[x].name ==a_name)
        output= x;

    unlockMutex();
    return output;
}

// standard FE_GPU_.. methods

FE_GPU_CreateProgramCMD::FE_GPU_CreateProgramCMD(){}
FE_GPU_CreateProgramCMD::~FE_GPU_CreateProgramCMD(){}

void FE_GPU_CreateProgramCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_CreateProgramCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

FE_GPU_SetProgramShaderCMD::FE_GPU_SetProgramShaderCMD(){}
FE_GPU_SetProgramShaderCMD::~FE_GPU_SetProgramShaderCMD(){}

void FE_GPU_SetProgramShaderCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetProgramShaderCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

FE_GPU_SetupProgramCMD::FE_GPU_SetupProgramCMD(){}
FE_GPU_SetupProgramCMD::~FE_GPU_SetupProgramCMD(){}

void FE_GPU_SetupProgramCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetupProgramCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

FE_GPU_SetProgramVariableCMD::FE_GPU_SetProgramVariableCMD(){}
FE_GPU_SetProgramVariableCMD::~FE_GPU_SetProgramVariableCMD(){}

void FE_GPU_SetProgramVariableCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetProgramVariableCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

FE_GPU_DestroyProgramCMD::FE_GPU_DestroyProgramCMD(){}
FE_GPU_DestroyProgramCMD::~FE_GPU_DestroyProgramCMD(){}

void FE_GPU_DestroyProgramCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_DestroyProgramCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

FE_GPU_DrawCommand::FE_GPU_DrawCommand(){}
FE_GPU_DrawCommand::~FE_GPU_DrawCommand(){}

void FE_GPU_DrawCommand::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_DrawCommand::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};
