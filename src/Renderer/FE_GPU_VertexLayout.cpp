#include "FE_GPU_API.h"

//FE_GPU_Info* FE_GPU_VertexLayoutCommand::info = nullptr;
//FE_GPU_Info* FE_GPU_VertexLayout::info = nullptr;
//FE_RENDERTYPE FE_GPU_VertexLayout::rend_type = FE_OPENGL3_2;



FE_GPU_CreateVAOCMD::FE_GPU_CreateVAOCMD(){}
FE_GPU_CreateVAOCMD::~FE_GPU_CreateVAOCMD(){}

void FE_GPU_CreateVAOCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_CreateVAOCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_SetupVAOCMD
  * command that creates a buffer
  **/

FE_GPU_SetupVAOCMD::FE_GPU_SetupVAOCMD(){}
FE_GPU_SetupVAOCMD::~FE_GPU_SetupVAOCMD(){}

void FE_GPU_SetupVAOCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetupVAOCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_SetVAOIBDataCMD
  * command that creates a buffer
  **/

FE_GPU_SetVAOIBDataCMD::FE_GPU_SetVAOIBDataCMD(){}
FE_GPU_SetVAOIBDataCMD::~FE_GPU_SetVAOIBDataCMD(){}

void FE_GPU_SetVAOIBDataCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetVAOIBDataCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_SetVAOIBSubDataCMD
  * command that modifies buffer data
  **/

FE_GPU_SetVAOIBSubDataCMD::FE_GPU_SetVAOIBSubDataCMD(){}
FE_GPU_SetVAOIBSubDataCMD::~FE_GPU_SetVAOIBSubDataCMD(){}

void FE_GPU_SetVAOIBSubDataCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetVAOIBSubDataCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_DestroyVAOCMD
  * command that creates a buffer
  **/

FE_GPU_DestroyVAOCMD::FE_GPU_DestroyVAOCMD(){}
FE_GPU_DestroyVAOCMD::~FE_GPU_DestroyVAOCMD(){}

void FE_GPU_DestroyVAOCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_DestroyVAOCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};
