#include "FE_GPU_API.h"
//#ifdef
void FE_GPU_CreateProgramCMD::updateGL3(){

    if(FE_GPU_Command::info->findProgram(name) != FE_GPU_Command::info->programs.size()){
        string log = "FE: GPU: WARNING: Shader Program - '" + name + "' already exists\n";
        log += ".................. Not creating shader program\n";
        //writeToLog(log);
        //cout << log << endl;
        return;
    }

    FE_GPU_Program prog;
    prog.name = this->name;
    prog.created_program = false;

    FE_GPU_Command::info->programs.push_back(prog);
}

void FE_GPU_CreateProgramCMD::flushGL3(FE_GPU_Thread*){}

void FE_GPU_SetProgramShaderCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findProgram(name);
    if(ID == FE_GPU_Command::info->programs.size()){
        string log = "FE: GPU: WARNING: Shader Program - '" + name + "' does not exist\n";
        log += "................. Not setting program shader\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_Program prog = FE_GPU_Command::info->programs[ID];

    if(prog.created_program) return;
    GLuint shader_id;
    switch(this->shader.type){

        case FE_GPU_VERTEX_SHADER:
            shader_id = prog.shaders.vs.id;
            FE_GPU_Command::info->programs[ID].shaders.vs.id = glCreateShader(GL_VERTEX_SHADER);
            shader_id = FE_GPU_Command::info->programs[ID].shaders.vs.id;
            break;

        case FE_GPU_PIXEL_SHADER:
            shader_id = prog.shaders.fs.id;
            FE_GPU_Command::info->programs[ID].shaders.fs.id = glCreateShader(GL_FRAGMENT_SHADER);
            shader_id = FE_GPU_Command::info->programs[ID].shaders.fs.id;
            break;

        default: return;
    }
    /// copy data to c string
    const GLchar* data= shader.data.c_str();

    /// compile and attach shader
    glShaderSource(shader_id, 1, &data, NULL);
    glCompileShader(shader_id);

    /// IMPORTANT: get shader compiler info
    int parameters = -1;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &parameters);
    if(parameters != GL_TRUE){

        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetShaderInfoLog(shader_id, max_length, &actual_length, log);
        writeToLog(log);
    }
    FE_GPU_Command::info->programs[ID].created_program = false;
}

void FE_GPU_SetProgramShaderCMD::flushGL3(FE_GPU_Thread*){}

void FE_GPU_SetupProgramCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findProgram(name);
    if(ID == FE_GPU_Command::info->programs.size()){
        string log = "FE: GPU: WARNING: Shader Program - '" + name + "' does not exist\n";
        log += "................. Not setting program shader\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_Program prog = FE_GPU_Command::info->programs[ID];
    //if(glIsProgram(prog.id))
    //cout << "this tells us if it is a program or not "<< endl;

    if(prog.created_program) return;

    FE_GPU_Command::info->programs[ID].id = glCreateProgram();
    prog = FE_GPU_Command::info->programs[ID];

    glAttachShader(prog.id, prog.shaders.vs.id);
    glAttachShader(prog.id, prog.shaders.fs.id);

    glBindAttribLocation(prog.id, 0, "fe_position");
    glBindAttribLocation(prog.id, 1, "fe_normals");
    glBindAttribLocation(prog.id, 2, "fe_uvs");

    glLinkProgram(prog.id);


    /// IMPORTANT: check if program is linked correctly
    int parameters = -1;
    glGetProgramiv(prog.id, GL_LINK_STATUS, &parameters);
    if(GL_TRUE != parameters){

        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetProgramInfoLog(prog.id, max_length, &actual_length, log);
        cout << log << endl;
        writeToLog(log);
    }

    /// IMPORTANT: make sure program is runnable
    glValidateProgram(prog.id);
    parameters = -1;
    glGetProgramiv(prog.id, GL_VALIDATE_STATUS, &parameters);
    if(GL_TRUE != parameters){

        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetProgramInfoLog(prog.id, max_length, &actual_length, log);
        cout << log << endl;
        writeToLog(log);

    }
    glUseProgram(prog.id);

    /// get all active uniform blocks
    GLint numBlocks=0;
    glGetProgramiv(prog.id, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);
    for(int id=0; id< numBlocks; id++){

        GLint name_length=0;

        glGetActiveUniformBlockiv(prog.id, id, GL_UNIFORM_BLOCK_NAME_LENGTH, &name_length);

        GLchar ubo_name[name_length];
        glGetActiveUniformBlockName(prog.id, id, name_length, NULL, &ubo_name[0]);

        string actual_name = string(ubo_name);
        writeToLog(actual_name);
        FE_GPU_Command::info->programs[ID].uniforms.push_back(actual_name);

    }
     FE_GPU_Command::info->programs[ID].created_program = true;
}

void FE_GPU_SetupProgramCMD::flushGL3(FE_GPU_Thread*){}

void FE_GPU_SetProgramVariableCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findProgram(name);
    if(ID == FE_GPU_Command::info->programs.size()){
        string log = "FE: GPU: WARNING: Shader Program - '" + name + "' does not exist\n";
        log += "................. Not setting program variable\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_Program prog = FE_GPU_Command::info->programs[ID];

    if(prog.hasUniform(this->ubo_state.name) != prog.uniforms.size()){

        if(prog.hasUniformSetup(ubo_state.name) == prog.uniform_indices.size())
            FE_GPU_Command::info->programs[ID].uniform_indices.push_back(ubo_state);
        else
            prog.uniform_indices[prog.hasUniformSetup(ubo_state.name)] = ubo_state;
        glUniformBlockBinding(prog.id, prog.hasUniform(ubo_state.name), ubo_state.slot);

    }

}

void FE_GPU_SetProgramVariableCMD::flushGL3(FE_GPU_Thread*){}

void FE_GPU_DestroyProgramCMD::updateGL3(){

    size_t ID = FE_GPU_Command::info->findProgram(name);
    if(ID == FE_GPU_Command::info->programs.size()){
        string log = "FE: GPU: WARNING: Shader Program - '" + name + "' does not exist\n";
        log += "................. Not setting program variable\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_Program prog = FE_GPU_Command::info->programs[ID];

    glDeleteShader(prog.shaders.vs.id);
    glDeleteShader(prog.shaders.fs.id);
    glDeleteProgram(prog.id);
    FE_GPU_Command::info->programs.erase(FE_GPU_Command::info->programs.begin() + ID);
}

void FE_GPU_DestroyProgramCMD::flushGL3(FE_GPU_Thread*){}

void FE_GPU_DrawCommand::updateGL3(){

    size_t ID = FE_GPU_Command::info->findProgram(name);
    if(ID == FE_GPU_Command::info->programs.size()){
        string log = "FE: GPU: WARNING: Shader Program - '" + name + "' does not exist\n";
        log += "................. Not drawing object\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_Program prog = FE_GPU_Command::info->programs[ID];

    size_t vaoID = FE_GPU_Command::info->findVAO(vao);

    if(vaoID == FE_GPU_Command::info->vaos.size()){
        string log = "FE: GPU: WARNING: VertexLayout - '" + vao + "' does not exist\n";
        log += "................. Not drawing object\n";
        writeToLog(log);
        cout << log << endl;
        return;
    }

    FE_GPU_VertexLayout layout = FE_GPU_Command::info->vaos[vaoID];

    glUseProgram(prog.id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, layout.ib_id);
    glBindVertexArray(layout.vao_id);
    if(vcount != 0)
        glDrawRangeElements(GL_TRIANGLES, voffset, vcount, vinstances, GL_UNSIGNED_INT, (GLvoid*)NULL);
    else
        glDrawElements(GL_TRIANGLES, vinstances, GL_UNSIGNED_INT, (GLvoid*)NULL);
}

void FE_GPU_DrawCommand::flushGL3(FE_GPU_Thread*){}
//#endif

