#include "FE_GPU_API.h"


FE_GPU_Command::FE_GPU_Command(){}
FE_GPU_Command::~FE_GPU_Command(){}

/** FE_GPU_CreateVBOCMD
  * command that creates a buffer
  **/
FE_RENDERTYPE FE_GPU_Command::rend_type;
FE_GPU_Info* FE_GPU_Command::info = nullptr;

FE_GPU_CreateVBOCMD::FE_GPU_CreateVBOCMD(){}
FE_GPU_CreateVBOCMD::~FE_GPU_CreateVBOCMD(){}

void FE_GPU_CreateVBOCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_CreateVBOCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_SetVBODataCMD
  * command that creates a buffer
  **/

FE_GPU_SetVBODataCMD::FE_GPU_SetVBODataCMD(){}
FE_GPU_SetVBODataCMD::~FE_GPU_SetVBODataCMD(){}

void FE_GPU_SetVBODataCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetVBODataCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_SetVBOSubDataCMD
  * command that creates a buffer
  **/

FE_GPU_SetVBOSubDataCMD::FE_GPU_SetVBOSubDataCMD(){}
FE_GPU_SetVBOSubDataCMD::~FE_GPU_SetVBOSubDataCMD(){}

void FE_GPU_SetVBOSubDataCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetVBOSubDataCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_DestroyVBOCMD
  * command that creates a buffer
  **/

FE_GPU_DestroyVBOCMD::FE_GPU_DestroyVBOCMD(){}
FE_GPU_DestroyVBOCMD::~FE_GPU_DestroyVBOCMD(){}

void FE_GPU_DestroyVBOCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_DestroyVBOCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};
