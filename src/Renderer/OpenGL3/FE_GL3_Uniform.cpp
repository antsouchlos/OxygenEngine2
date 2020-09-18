#include "FE_GPU_API.h"
//#ifdef FE_ATTRIB_OPENGL
/** OpenGL implementation of create UBO
**/

void FE_GPU_CreateUBOCMD::updateGL3(){

    if(FE_GPU_Command::info->findUniform(name) != FE_GPU_Command::info->ubos.size()){
        string log = "FE: GPU: WARNING: Uniform Buffer - '" + name + "' already exists\n";
        log += ".................. Not creating Uniform buffer\n";
        //writeToLog(log);
        //cout << log << endl;
        return;
    }

    FE_GPU_Uniform buffer;
    buffer.name = this->name;

    glGenBuffers(1, &buffer.buffer_id);

    FE_GPU_Command::info->ubos.push_back(buffer);
}

void FE_GPU_CreateUBOCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of requesting UBO data
**/

void FE_GPU_SetUBODataCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findUniform(name);
    if(ID == FE_GPU_Command::info->ubos.size()){
        string log = "FE: GPU: WARNING: Uniform Buffer - '" + name + "' does not exist\n";
        log += "................. Not setting memory of uninitialized Uniform buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_Uniform buffer = FE_GPU_Command::info->ubos[ID];
    buffer.capacity = memory_size;

    /// determine usage of the buffer
    GLenum buf_usage;
    switch(usage){
        case FE_GPU_STATIC: buf_usage = GL_STATIC_DRAW; break;
        case FE_GPU_DYNAMIC: buf_usage = GL_DYNAMIC_DRAW; break;
        case FE_GPU_STREAM: buf_usage = GL_STREAM_DRAW; break;
        default: buf_usage = GL_STATIC_DRAW;
    }
    glBindBuffer(GL_UNIFORM_BUFFER, buffer.buffer_id);
    glBufferData(GL_UNIFORM_BUFFER, memory_size*sizeof(GLfloat), NULL, buf_usage);
}

void FE_GPU_SetUBODataCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of setting UBO data
**/

void FE_GPU_SetUBOSubDataCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findUniform(name);
    if(ID == FE_GPU_Command::info->ubos.size()){
        string log = "FE: GPU: WARNING: Uniform Buffer - '" + name + "' does not exist\n";
        log += "................. Not setting data of uninitialized Uniform buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }
    FE_GPU_Uniform buffer = FE_GPU_Command::info->ubos[ID];

    /// copy to c-array
    if(data.size() >= data_u.size()){

        GLfloat actual_data[data.size()];
        copy(data.begin(), data.end(), actual_data);

        glBindBuffer(GL_UNIFORM_BUFFER, buffer.buffer_id);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(actual_data), actual_data);

    }
    else{

        GLuint actual_data[data.size()];
        copy(data.begin(), data.end(), actual_data);

        glBindBuffer(GL_ARRAY_BUFFER, buffer.buffer_id);
        glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(actual_data), actual_data);

    }
}

void FE_GPU_SetUBOSubDataCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of deleting a UBO
**/

void FE_GPU_SetUBOSlotCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findUniform(name);
    if(ID == FE_GPU_Command::info->ubos.size()){
        string log = "FE: GPU: WARNING: Uniform Buffer - '" + name + "' does not exist\n";
        log += "................. Not destroying uninitialized Uniform buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }
    FE_GPU_Uniform buffer = FE_GPU_Command::info->ubos[ID];

    if(state.length == 0)
        glBindBufferBase(GL_UNIFORM_BUFFER, state.slot, buffer.buffer_id);
    else
        glBindBufferRange(GL_UNIFORM_BUFFER, state.slot, buffer.buffer_id, state.offset, state.length);
}

void FE_GPU_SetUBOSlotCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of deleting a UBO
**/

void FE_GPU_DestroyUBOCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findUniform(name);
    if(ID == FE_GPU_Command::info->ubos.size()){
        string log = "FE: GPU: WARNING: Uniform Buffer - '" + name + "' does not exist\n";
        log += "................. Not destroying uninitialized Uniform buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }
    FE_GPU_Uniform buffer = FE_GPU_Command::info->ubos[ID];

    glDeleteBuffers(1, &buffer.buffer_id);
    FE_GPU_Command::info->ubos.erase(FE_GPU_Command::info->ubos.begin()+ID);
}

void FE_GPU_DestroyUBOCMD::flushGL3(FE_GPU_Thread*){}
//#endif

