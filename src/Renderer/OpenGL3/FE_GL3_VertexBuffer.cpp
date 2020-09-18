#include "FE_GPU_API.h"

//#ifdef FE_ATTRIB_OPENGL

/** OpenGL implementation of create VBO
**/

void FE_GPU_CreateVBOCMD::updateGL3(){

    if(FE_GPU_Command::info->findVBO(name) != FE_GPU_Command::info->vbos.size()){
        string log = "FE: GPU: WARNING: Vertex Buffer - '" + name + "' already exists\n";
        log += ".................. Not creating vertex buffer\n";
        //writeToLog(log);
        //cout << log << endl;
        return;
    }
    FE_GPU_VertexBuffer buffer;
    buffer.name = this->name;

    glGenBuffers(1, &buffer.vertex_id);

    FE_GPU_Command::info->vbos.push_back(buffer);
}

void FE_GPU_CreateVBOCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of requesting VBO data
**/

void FE_GPU_SetVBODataCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findVBO(name);
    if(ID == FE_GPU_Command::info->vbos.size()){
        string log = "FE: GPU: WARNING: Vertex Buffer - '" + name + "' does not exist\n";
        log += "................. Not setting memory of uninitialized vertex buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_VertexBuffer buffer = FE_GPU_Command::info->vbos[ID];
    buffer.capacity = memory_size;

    /// determine usage of the buffer
    GLenum buf_usage;
    switch(usage){
        case FE_GPU_STATIC: buf_usage = GL_STATIC_DRAW; break;
        case FE_GPU_DYNAMIC: buf_usage = GL_DYNAMIC_DRAW; break;
        case FE_GPU_STREAM: buf_usage = GL_STREAM_DRAW; break;
        default: buf_usage = GL_STATIC_DRAW;
    }
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vertex_id);
    glBufferData(GL_ARRAY_BUFFER, memory_size*sizeof(GLfloat), NULL, buf_usage);
}

void FE_GPU_SetVBODataCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of setting VBO data
**/

void FE_GPU_SetVBOSubDataCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findVBO(name);
    if(ID == FE_GPU_Command::info->vbos.size()){
        string log = "FE: GPU: WARNING: Vertex Buffer - '" + name + "' does not exist\n";
        log += "................. Not setting data of uninitialized vertex buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }
    FE_GPU_VertexBuffer buffer = FE_GPU_Command::info->vbos[ID];

    /// copy to c-array
    //for(auto x : data) writeTo
    GLfloat actual_data[data.size()];
    copy(data.begin(), data.end(), actual_data);

	
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vertex_id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(actual_data), actual_data);
}

void FE_GPU_SetVBOSubDataCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of deleting a VBO
**/

void FE_GPU_DestroyVBOCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findVBO(name);
    if(ID == FE_GPU_Command::info->vbos.size()){
        string log = "FE: GPU: WARNING: Vertex Buffer - '" + name + "' does not exist\n";
        log += "................. Not destroying uninitialized vertex buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }
    FE_GPU_VertexBuffer buffer = FE_GPU_Command::info->vbos[ID];

    glDeleteBuffers(1, &buffer.vertex_id);
    FE_GPU_Command::info->ubos.erase(FE_GPU_Command::info->ubos.begin()+ID);
}

void FE_GPU_DestroyVBOCMD::flushGL3(FE_GPU_Thread*){}
//#endif
