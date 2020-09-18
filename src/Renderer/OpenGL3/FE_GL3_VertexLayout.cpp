#include "FE_GPU_API.h"

#define VERTEXL_LAYOUT_OFFSET(i) ((GLvoid*)(i))
//#ifdef FE_ATTRIB_OPENGL

void FE_GPU_CreateVAOCMD::updateGL3(){

    if(FE_GPU_Command::info->findVAO(name) != FE_GPU_Command::info->vaos.size()){
        string log = "FE: GPU: WARNING: Vertex Layout - '" + name + "' already exists\n";
        log += "................. Not creating vertex layout\n";
        //writeToLog(log);
        //cout << log << endl;
        return;
    }

    FE_GPU_VertexLayout vao;
    vao.name = this->name;

    glGenVertexArrays(1, &vao.vao_id);
    glGenBuffers(1, &vao.ib_id);

    FE_GPU_Command::info->vaos.push_back(vao);
}

void FE_GPU_CreateVAOCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of requesting VAO data
**/

void FE_GPU_SetupVAOCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findVAO(name);
    if(ID == FE_GPU_Command::info->vaos.size()){
        string log = "FE: GPU: WARNING: Vertex Layout - '" + name + "' does not exist\n";
        log += "................. Not setting data of uninitialized vertex layout\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    /// bind the dat sh** else the false freaking VAO will be modified
    FE_GPU_VertexLayout vao = FE_GPU_Command::info->vaos[ID];
    glBindVertexArray(vao.vao_id);

    for(size_t x=0; x < inputs.size(); x++){

        /// do error checking
        size_t vbo_id = FE_GPU_Command::info->findVBO(inputs[x].name);
        if(vbo_id == FE_GPU_Command::info->vbos.size()){
            string log = "FE: GPU: WARNING: Vertex Buffer - '" + inputs[x].name + "' does not exist\n";
            log += "................. Not adding a non-existent vertex buffer to vertex layout - '" + name + "'\n";
            writeToLog(log);
            cout << log << endl;
            return;
        }

        glBindBuffer(GL_ARRAY_BUFFER, FE_GPU_Command::info->vbos[vbo_id].vertex_id);
        glVertexAttribPointer(x, inputs[x].amount, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*inputs[x].stride, VERTEXL_LAYOUT_OFFSET(sizeof(GLfloat)*inputs[x].offset));
        glEnableVertexAttribArray(x);
    }
}
void FE_GPU_SetupVAOCMD::flushGL3(FE_GPU_Thread*){}

/** OpenGL implementation of setting VBO data
**/

void FE_GPU_SetVAOIBDataCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findVAO(name);
    if(ID == FE_GPU_Command::info->vaos.size()){
        string log = "FE: GPU: WARNING: Vertex Layout - '" + name + "' does not exist\n";
        log += ".................. Not setting data of uninitialized vertex layout\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_VertexLayout vao = FE_GPU_Command::info->vaos[ID];
    vao.capacity = memory_size;

    /// determine usage of the buffer
    GLenum buf_usage;
    switch(usage){
        case FE_GPU_STATIC: buf_usage = GL_STATIC_DRAW; break;
        case FE_GPU_DYNAMIC: buf_usage = GL_DYNAMIC_DRAW; break;
        case FE_GPU_STREAM: buf_usage = GL_STREAM_DRAW; break;
        default: buf_usage = GL_STATIC_DRAW;
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ib_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, memory_size*sizeof(GLuint), NULL, buf_usage);
}
void FE_GPU_SetVAOIBDataCMD::flushGL3(FE_GPU_Thread*){}

/**
  */
void FE_GPU_SetVAOIBSubDataCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findVAO(name);
    if(ID == FE_GPU_Command::info->vaos.size()){
        string log = "FE: GPU: WARNING: Vertex Buffer - '" + name + "' does not exist\n";
        log += "................. Not setting data of uninitialized vertex buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }
    FE_GPU_VertexLayout vao = FE_GPU_Command::info->vaos[ID];

    /// copy to c-array
    GLuint actual_data[data.size()];
    copy(data.begin(), data.end(), actual_data);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ib_id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(actual_data), actual_data);
}

void FE_GPU_SetVAOIBSubDataCMD::flushGL3(FE_GPU_Thread*){}

void FE_GPU_DestroyVAOCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findVAO(name);
    if(ID == FE_GPU_Command::info->vaos.size()){
        string log = "FE: GPU: WARNING: Vertex Buffer - '" + name + "' does not exist\n";
        log += "................. Not destroying uninitialized vertex buffer\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }
    FE_GPU_VertexLayout vao = FE_GPU_Command::info->vaos[ID];

    glDeleteVertexArrays(1, &vao.vao_id);
    glDeleteBuffers(1, &vao.ib_id);
}

void FE_GPU_DestroyVAOCMD::flushGL3(FE_GPU_Thread*){}
//#endif
