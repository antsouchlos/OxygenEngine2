#include "FE_GPU_API.h"

FE_GPU_CreateUBOCMD::FE_GPU_CreateUBOCMD(){}
FE_GPU_CreateUBOCMD::~FE_GPU_CreateUBOCMD(){}

void FE_GPU_CreateUBOCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_CreateUBOCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_SetUBODataCMD
  * command that creates a buffer
  **/

FE_GPU_SetUBODataCMD::FE_GPU_SetUBODataCMD(){}
FE_GPU_SetUBODataCMD::~FE_GPU_SetUBODataCMD(){}

void FE_GPU_SetUBODataCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetUBODataCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_SetUBOSubDataCMD
  * command that creates a buffer
  **/

FE_GPU_SetUBOSubDataCMD::FE_GPU_SetUBOSubDataCMD(){}
FE_GPU_SetUBOSubDataCMD::~FE_GPU_SetUBOSubDataCMD(){}

void FE_GPU_SetUBOSubDataCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetUBOSubDataCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_SetUBOSlotCMD
  * command that creates a buffer
  **/

FE_GPU_SetUBOSlotCMD::FE_GPU_SetUBOSlotCMD(){}
FE_GPU_SetUBOSlotCMD::~FE_GPU_SetUBOSlotCMD(){}

void FE_GPU_SetUBOSlotCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_SetUBOSlotCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};

/** FE_GPU_DestroyUBOCMD
  * command that creates a buffer
  **/

FE_GPU_DestroyUBOCMD::FE_GPU_DestroyUBOCMD(){}
FE_GPU_DestroyUBOCMD::~FE_GPU_DestroyUBOCMD(){}

void FE_GPU_DestroyUBOCMD::update(){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: updateGL3(); break;
        case FE_OPENGLES3: updateGL3(); break;
        default: cout << "";
    }
};
void FE_GPU_DestroyUBOCMD::flush(FE_GPU_Thread* thread){
    /// api-specific codebases
    switch(FE_GPU_Command::rend_type){
        case FE_OPENGL3_2: flushGL3(thread); break;
        case FE_OPENGLES3: flushGL3(thread); break;
        default: cout << "";
    }
};
