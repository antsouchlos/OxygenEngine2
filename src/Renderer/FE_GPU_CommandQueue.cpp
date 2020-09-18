#include "FE_GPU_API.h"

// declare and define static variable
FE_GPU_Info* FE_GPU_CommandQueue::info = nullptr;

FE_GPU_CommandQueue::FE_GPU_CommandQueue(FE_GPU_Thread* threada)
{
    thread = threada;
    flushed = false;
}

FE_GPU_CommandQueue::~FE_GPU_CommandQueue()
{

}

void FE_GPU_CommandQueue::destroy(){
    for(auto command: this->commands){
        delete command;
    }
}
/// internal call
void FE_GPU_CommandQueue::update(){

    for(auto command : this->commands){
        command->update();
    }
}

/// public API calls
void FE_GPU_CommandQueue::optimize(){
    for(auto command : this->commands){
        command->flush(this->thread);

    }
    lockMutex();
    this->flushed = true;
    unlockMutex();
}

//DONE
void FE_GPU_CommandQueue::createVertexBuffer(string a_name){
    if(flushed) return;
    FE_GPU_CreateVBOCMD* cmd = new FE_GPU_CreateVBOCMD();

    cmd->name = a_name;
    lockMutex();
    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setVertexBufferMemory(string a_name, size_t memory_size, FE_GPU_BUFFER_USAGE usage){
    if(flushed) return;
    FE_GPU_SetVBODataCMD* cmd = new FE_GPU_SetVBODataCMD();

    cmd->name = a_name;
    cmd->memory_size = memory_size;
    cmd->usage = usage;
    lockMutex();
    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setVertexBufferData(string a_name, vector<GLfloat> data, size_t offset){

    if(flushed) return;
    FE_GPU_SetVBOSubDataCMD* cmd = new FE_GPU_SetVBOSubDataCMD();

    cmd->name = a_name;
    cmd->offset = 0;
    cmd->data = data;

    lockMutex();
    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::createVertexLayout(string a_name){

    if(flushed) return;
    FE_GPU_CreateVAOCMD* cmd = new FE_GPU_CreateVAOCMD();

    cmd->name = a_name;
    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}
void FE_GPU_CommandQueue::setVertexLayoutFormat(string a_name, vector<FE_GPU_VertexLayoutInput> inputs){
    if(flushed) return;
    FE_GPU_SetupVAOCMD* cmd = new FE_GPU_SetupVAOCMD();

    cmd->name = a_name;
    cmd->inputs = inputs;
    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setVLIndexBufferMemory(string a_name, GLuint memory_size, FE_GPU_BUFFER_USAGE usage){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetVAOIBDataCMD* cmd = new FE_GPU_SetVAOIBDataCMD();

    cmd->name = a_name;
    cmd->memory_size = memory_size;
    cmd->usage = usage;
    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setVLIndexBufferData(string a_name, vector<GLuint> inputs, GLuint offset){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetVAOIBSubDataCMD* cmd = new FE_GPU_SetVAOIBSubDataCMD();

    cmd->name = a_name;
    cmd->offset = offset;
    cmd->data = inputs;
    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}
void FE_GPU_CommandQueue::deleteVertexLayout(string a_name){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_DestroyVAOCMD* cmd = new FE_GPU_DestroyVAOCMD();

    cmd->name = a_name;
    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::createProgram(string a_name){
    if(flushed) return;
    FE_GPU_CreateProgramCMD* cmd = new FE_GPU_CreateProgramCMD();

    cmd->name = a_name;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setProgramVS(string a_name, FE_GPU_Shader shader){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetProgramShaderCMD* cmd = new FE_GPU_SetProgramShaderCMD();

    cmd->name = a_name;
    cmd->shader = shader;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setProgramGS(string a_name, FE_GPU_Shader ){

}

void FE_GPU_CommandQueue::setProgramFS(string a_name, FE_GPU_Shader shader){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetProgramShaderCMD* cmd = new FE_GPU_SetProgramShaderCMD();

    cmd->name = a_name;
    cmd->shader = shader;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}
void FE_GPU_CommandQueue::setProgramTCS(string a_name, FE_GPU_Shader){

}
void FE_GPU_CommandQueue::setProgramTES(string a_name, FE_GPU_Shader){

}
void FE_GPU_CommandQueue::setProgramUniformSlot(string a_name, string uniform_name, GLint slot){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetProgramVariableCMD* cmd = new FE_GPU_SetProgramVariableCMD();

    cmd->name = a_name;

    cmd->ubo_state.name = uniform_name;
    cmd->ubo_state.slot = slot;
    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}
void FE_GPU_CommandQueue::setupProgram(string a_name){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetupProgramCMD* cmd = new FE_GPU_SetupProgramCMD();

    cmd->name = a_name;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();

}


void FE_GPU_CommandQueue::createUniform(string a_name){
    if(flushed) return;
    FE_GPU_CreateUBOCMD* cmd = new FE_GPU_CreateUBOCMD();
    cmd->name = a_name;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setUniformMemory(string a_name, GLuint memory_size, FE_GPU_BUFFER_USAGE usage){
    if(flushed) return;
    FE_GPU_SetUBODataCMD* cmd = new FE_GPU_SetUBODataCMD();
    cmd->name = a_name;
    cmd->memory_size = memory_size;
    cmd->usage = usage;
    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setUniformData(string a_name, vector<GLuint> data_u, GLuint offset){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetUBOSubDataCMD* cmd = new FE_GPU_SetUBOSubDataCMD();

    cmd->name = a_name;
    cmd->data_u = data_u;
    cmd->offset = offset;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}
void FE_GPU_CommandQueue::setUniformData(string a_name, vector<GLfloat> data, GLuint offset){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetUBOSubDataCMD* cmd = new FE_GPU_SetUBOSubDataCMD();

    cmd->name = a_name;
    cmd->data = data;
    cmd->offset = offset;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::setUniformState(string a_name, GLint slot, GLuint offset, GLuint length){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_SetUBOSlotCMD* cmd = new FE_GPU_SetUBOSlotCMD();

    cmd->name = a_name;
    cmd->state.slot = slot;
    cmd->state.offset = offset;
    cmd->state.length = length;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::createLargeBuffer(string a_name, FE_GPU_BUFFER_USAGE){
    switch(rend_type){
        case FE_OPENGL3_2: cout << "";
        case FE_OPENGLES3:  cout << "";
        default: cout << "";
    }
}
void FE_GPU_CommandQueue::setLargeBufferLength(string a_name, int){
    switch(rend_type){
        case FE_OPENGL3_2: cout << "";
        case FE_OPENGLES3:  cout << "";
        default: cout << "";
    }
}
void FE_GPU_CommandQueue::setLargeBuffer(string a_name, GLfloat){
    switch(rend_type){
        case FE_OPENGL3_2: cout << "";
        case FE_OPENGLES3:  cout << "";
        default: cout << "";
    }
}

void FE_GPU_CommandQueue::draw(string a_name, string vao, int, int){

}
void FE_GPU_CommandQueue::draw(string a_name, string){
    switch(rend_type){
        case FE_OPENGL3_2: cout << "";
        case FE_OPENGLES3:  cout << "";
        default: cout << "";
    }
}
void FE_GPU_CommandQueue::drawInstances(string a_name, string, int){
    switch(rend_type){
        case FE_OPENGL3_2: cout << "";
        case FE_OPENGLES3:  cout << "";
        default: cout << "";
    }
}
void FE_GPU_CommandQueue::drawInstances(string a_name, string vao, int voffset, int vcount, int vinstances){
    //#ifdef FE_DEBUG
    //#endif
    if(flushed) return;
    FE_GPU_DrawCommand* cmd = new FE_GPU_DrawCommand();

    cmd->name = a_name;

    cmd->vao = vao;
    cmd->vcount = vcount;
    cmd->voffset = voffset;
    cmd->vinstances = vinstances;
    cmd->destroyed = false;

    lockMutex();

    this->commands.push_back(cmd);
    unlockMutex();
}

void FE_GPU_CommandQueue::Clear(){

    flushed = false;
    for(auto cmd: commands)
        delete cmd;
    commands.clear();
}
