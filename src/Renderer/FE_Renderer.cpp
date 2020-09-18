#include "FE_Renderer.h"

FE_Renderer::FE_Renderer()
{

    //this->info = nullptr;
    made_context = false;
    general_data.created_ubo = false;
}

FE_Renderer::~FE_Renderer()
{
    //dtor
}

bool FE_Renderer::update(FE_GPU_Thread* thread, FE_Scene* scene, float aspect_ratio){
    //#ifdef FE_ATTRIB_OPENGL

    updateViewPorts(scene, thread, aspect_ratio);

    updateObjects(scene, thread);

    update2D(scene, thread);
    updateUniforms(thread);

    renderObjects(thread);
    //thread->flush();
    //#endif
    return true;
}


bool FE_Renderer::destroy(FE_GPU_Thread* thread){
    return true;
}


void FE_Renderer::updateViewPorts(FE_Scene* scene, FE_GPU_Thread* thread, float aspect_ratio){

    /// setup perspective view matrix
    //scene->getCamera(scene->getCameraNames()[0])->setAspectRatio(aspect_ratio);
    general_data.perspective_mat = scene->getCamera(scene->getCameraNames()[0])->genProjectionMatrix();
    general_data.view_mat.setPos(scene->getCamera(scene->getCameraNames()[0])->getPos());
    general_data.view_mat.setRot(scene->getCamera(scene->getCameraNames()[0])->getRotEuler());

    ///copy view matrix and perspective_matrix into the buffer
    vector<GLfloat> data_array;
    //data_array.reserve(16*2);
    for(size_t i=0; i < 16; i++){
        data_array.push_back(general_data.perspective_mat.toArray()[i]);
    }
    for(size_t i=0; i < 16; i++){
        data_array.push_back(general_data.view_mat.toArray()[i]);
    }


    thread->deleteCommandQueue("_FE_global_viewports");
    FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue("_FE_global_viewports");

    //cout << "END_FRAME" << endl;
    if(!general_data.created_ubo){
        /*glGenBuffers(1, &general_data.uniform_index);
        glBindBuffer(GL_UNIFORM_BUFFER, general_data.uniform_index);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(data_array), NULL, GL_STREAM_DRAW);
        general_data.created_ubo = true;
        */
        cmd_queue->createUniform("_FE_global_data");
        cmd_queue->setUniformMemory("_FE_global_data", 16*2, FE_GPU_STREAM);
        cmd_queue->setUniformData("_FE_global_data", data_array, 0);

        general_data.created_ubo = true;


    }
    else{
        cmd_queue->Clear();
        cmd_queue->setUniformData("_FE_global_data", data_array, 0);
    /*glBindBuffer(GL_UNIFORM_BUFFER, general_data.uniform_index);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data_array), data_array);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, general_data.uniform_index);
    */

    }
     cmd_queue->setUniformState("_FE_global_data", 0, 0, 0);
    cmd_queue->optimize();
    thread->submit("_FE_global_viewports");
}


void FE_Renderer::updateObjects(FE_Scene* scene, FE_GPU_Thread* thread){

    scene->lockMutex();
    size_t current_id = 2;
//    FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue("_FE_objupdate");

    for(auto material: scene->getMaterialNames()){

        /// check if the material is already in the list
        size_t id = 0;
        bool mat_included = false;
        for(size_t i = 0; i < this->materials.size(); i++)
        if(materials[i].name == material){
            mat_included = true;
            id = i;
        }

        if(!mat_included){

            /// add a new material
            FE_MaterialData mat_data;
            mat_data.name = material;

            /// assign initial material data
            mat_data.created = false;
            mat_data.updated_mat = *scene->getMaterial(material);
            materials.push_back(mat_data);

            /// keep material id for later use
            id = materials.size()-1;
        }

        materials[id].update(thread);

        /// order objects by material
        size_t obj_count = 0;
        for (auto obj : scene->getObjectNames())
        if(scene->getObject(obj)->hasMaterial(material)){

            /// check if object is already in the list of rendered objects
            bool isAlreadyIncluded = false;
            for(size_t x=0; x< this->materials[id].meshes.size(); x++)
            if(materials[id].meshes[x].name == obj){

                /// update object properties that change every frame
                isAlreadyIncluded = true;
                //materials[id].meshes[x].getVertices(scene->getObject(obj), material);
                materials[id].meshes[x].getObjectData(scene, scene->getObject(obj));
                materials[id].meshes[x].update(thread);
                break;
            }


            if(!isAlreadyIncluded){

                /// make new rendering object
                FE_MeshRenderData data;

                //data.init(scene->getObject(obj), material, materials[id].computeIBlength(materials[id].meshes.size()), obj, materials[id].meshes.size());
                //data.model_mat = scene->getObject(obj)->getModelMatrix();
                data.name = obj;
                data.mat = material;


                data.getVertices(scene->getObject(obj), material);
                data.getObjectData(scene, scene->getObject(obj));
                data.update(thread);
                materials[id].meshes.push_back(data);
                //materials[id].updateVertices(thread);

                // create new uniform buffers if needed
                /*if(obj_count % 256 == 0 ){
                    if(materials[id].created_ubos.size() <= obj_count/256){

                        //materials[id].ubo_index.push_back(0);
                        //materials[id].offset_index.push_back(0);
                        //materials[id].objects_256 +=1;
                        //glGenBuffers(1, &materials[id].ubo_index[obj_count/256]);
                        //glGenBuffers(1, &materials[id].offset_index[obj_count/256]);
                        cmd_queue->createUniform("_FE_" + material + "_offset");
                        cmd_queue->createUniform("_FE_" + material + "_model_matrices");

                        materials[id].created_ubos.push_back(true);
                    }
                }
                /// setup material program
                FE_GLSimpleProgram program;
                program.chooseProgram(scene, materials[id], cmd_queue);

                cmd_queue->setProgramUniformSlot(material, "FE_SceneData", 1);
                GLuint loca = glGetUniformBlockIndex(program.index, "FE_SceneData");
                glUniformBlockBinding(program.index, loca, 1);
                glBindBufferRange(GL_UNIFORM_BUFFER, 1, general_data.uniform_index, 0, 128);

                cmd_queue->setUniformState("general_data", 1, 0, 128);

                // find any unused uniform block binding
                for(auto x : this->ubo_bindings){
                    if(!x.second){
                        //materials[id].ubo_binding = x.first;
                        //cout << material << " " << x.first << endl;
                        //ubo_bindings[x.first] = true;
                        break;
                    }
                }

                /// setup model_matrix buffer
                //glBindBuffer(GL_UNIFORM_BUFFER, materials[id].ubo_index);
                GLuint loca2 = glGetUniformBlockIndex(program.index, "FE_Objects");
                glUniformBlockBinding(program.index, loca2, 2);
                //glBindBufferBase(GL_UNIFORM_BUFFER, 2, materials[id].ubo_index);

                /// setup object offset buffer
                //glBindBuffer(GL_UNIFORM_BUFFER, materials[id].offset_index);
                GLuint loca3 = glGetUniformBlockIndex(program.index, "FE_ObjectOffsets");
                glUniformBlockBinding(program.index, loca3, 3);
                //glBindBufferBase(GL_UNIFORM_BUFFER, 3, materials[id].offset_index);
                */
            }
            else if(scene->getObject(obj)->changed){

            }
            current_id++;
            obj_count++;
        }
    }
    scene->unlockMutex();
    thread->submit("objupdate");
}

void FE_Renderer::updateFonts(FE_Scene* scene, FE_GPU_Thread* thread){

}

size_t FE_Renderer::findProgram(string prog){
    /*for(size_t i=0; i < programs.size(); i++)
    if(programs[i].name == prog){
        return i;
    }

    return 0;*/
    return 0;
}
void FE_Renderer::updateUniforms(FE_GPU_Thread* thread){
    for(auto &mat : materials){

        size_t sizea = 16*mat.meshes.size();
        while(sizea % 256 != 0)
            sizea += 16;

        for(size_t i=0; i < sizea/256; i++){


            //FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue(mat.name + "uniforms");
            /// setup model matrices
            /*GLfloat model_matrices[256*16];
            glBindBuffer(GL_UNIFORM_BUFFER, mat.ubo_index[i]);
            */
            /*
            if(mat.meshes.size() != mat.vao.length)
                cmd_queue->setUniformMemory(mat.name + "_model_matrices", 256*16, FE_GPU_STREAM);
                //glBufferData(GL_UNIFORM_BUFFER, sizeof(model_matrices), NULL, GL_STREAM_DRAW);

            vector<GLfloat> model_matrices;
            model_matrices.reserve(256*16);
            for(size_t x=0; x< 256*16; x++)
                model_matrices[x] = 50.0f;

            if(mat.meshes.size() < (i+1)*256u)
                for(size_t x=i*256; x < mat.meshes.size(); x++)
                for(size_t y = 0; y < 16; y++){
                    model_matrices[x*16+y] = mat.meshes[x].model_mat.toArray()[y];
                }
            else
                for(size_t x=i*256; x < (i+1)*256; x++)
                for(size_t y = 0; y < 16; y++){
                    model_matrices[x*16+y] = mat.meshes[x].model_mat.toArray()[y];
                }

            //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(model_matrices), model_matrices);
            cout << mat.name + "_model_matrices" << endl;
            cmd_queue->setUniformData(mat.name + "_model_matrices", model_matrices, 0);
            */

            /// setup object offsets
            /*GLuint model_offsets[256*4];
            glBindBuffer(GL_UNIFORM_BUFFER, mat.offset_index[i]);
            */

            /*vector<GLuint> model_offsets;
            model_offsets.reserve(256*4);

            if(mat.meshes.size() != mat.vao.length){

                mat.vao.length = mat.meshes.size();
                cmd_queue->setUniformMemory(mat.name + "_offsets", model_offsets.size(), FE_GPU_STREAM);
                //glBufferData(GL_UNIFORM_BUFFER, sizeof(model_offsets), NULL, GL_STREAM_DRAW);
            }

            for(size_t x=0; x< 256*4; x++)
                model_offsets[x] = 50;

            for(size_t x=0; x < mat.meshes.size()*4; x+=4)
                model_offsets[x] = mat.meshes[x].offset;

            //glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(model_offsets), model_offsets);
            cmd_queue->setUniformData(mat.name + "_offsets", model_offsets, 0);
            thread->submit(mat.name + "uniforms");
            */
        }
    }
}

void FE_Renderer::update2D(FE_Scene* scene, FE_GPU_Thread* thread){

}


void FE_Renderer::renderObjects(FE_GPU_Thread* thread){
    for(auto &mat: materials){

        if(mat.meshes.size() != mat.previous_mesh_size){
            thread->deleteCommandQueue("_FE_" + mat.name +"_FE_" + "draw");
            mat.previous_mesh_size = mat.meshes.size();
        }
        mat.draw(thread);

        /*glUseProgram(programs[findProgram(mat.program)].index);
        glBindBuffer(GL_ARRAY_BUFFER, mat.vao.vbo_index);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mat.vao.ib_index);
        glBindVertexArray(mat.vao.vao_index);
        */
        /*FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue(mat.name + "draw");
        vector<FE_GL_Offset> offset_list = mat.getOffsetList();
        for(size_t x=0; x < offset_list.size(); x++){

            // put the uniform buffer in the list
            //glUniformBlockBinding(programs[findProgram(mat.program)].index, glGetUniformBlockIndex(programs[findProgram(mat.program)].index, "FE_Objects"), 2);
            //glBindBufferRange(GL_UNIFORM_BUFFER, 2, mat.ubo_index   , offset_list[x].prev_ubo, offset_list[x].ubo_size);
            //glBindBufferRange(GL_UNIFORM_BUFFER, 3, mat.offset_index, offset_list[x].prev_ubo, offset_list[x].ubo_size);
            glBindBufferBase(GL_UNIFORM_BUFFER, 2, mat.ubo_index[x]);
            glBindBufferBase(GL_UNIFORM_BUFFER, 3, mat.offset_index[x]);

            glDrawRangeElements(GL_TRIANGLES, offset_list[x].previous, offset_list[x].current+offset_list[x].previous, offset_list[x].current, GL_UNSIGNED_INT, 0);

            cmd_queue->setUniformState(mat.name +"_model_matrices", 2, 0, 0);
            cmd_queue->setUniformState(mat.name +"_offsets", 2, 0, 0);
            cmd_queue->drawInstances(mat.name, mat.name, offset_list[x].previous, offset_list[x].current+offset_list[x].previous, offset_list[x].current);
        }*/
    }
}

void FE_Renderer::renderFonts(FE_GPU_Thread* thread){

}

void FE_Renderer::render2D(FE_GPU_Thread* thread){

}
/** OpenGL 3.2, OpenGL ES 3.0 specific
  * This method runs everytime some critical material info is updated
  * such as textures and forces the material program to be updated
  */
/*
void FE_GLSimpleProgram::chooseProgram(FE_Scene* scene, FE_MaterialData mat, FE_GPU_CommandQueue* cmd_queue){

    name = mat.name;



    cmd_queue->createProgram(name);

    index = glCreateProgram();
    vs.index = glCreateShader(GL_VERTEX_SHADER);
    fs.index = glCreateShader(GL_FRAGMENT_SHADER);



    // set shader sources
    FE_GPU_Shader shader;
    shader.type = FE_GPU_VERTEX_SHADER;
    shader.data = string(Shader(

    in vec3 fe_position;
    in vec3 fe_normals;
    in vec2 fe_uvs;

    out vec4 normals;
    out vec2 uvs;

    layout(std140) uniform FE_Objects{
        mat4 model[256];
    };

    layout(std140) uniform FE_SceneData{
        mat4 perspective;
        mat4 view;
    };

    layout (std140) uniform FE_ObjectOffsets{
        uint offset[256];
    };
    void main(){
        normals = transpose(view)*transpose(model[0])*vec4(fe_normals, 1.0);
        uvs = fe_uvs;
        vec4 final_position = perspective*transpose(view)*transpose(model[0])*vec4(fe_position, 1.0);
        gl_Position = final_position;
    }));
    cmd_queue->setProgramVS(name, shader);

    shader.type = FE_GPU_PIXEL_SHADER;
    shader.data = string(Shader(

    precision mediump float;

    in vec4 normals;
    in vec2 uvs;

    out vec4 fragColor;
    void main(){
        //fragColor = vec4(1.0, 1.0, 1.0, 1.0);
        fragColor = normals;
    }));
    cmd_queue->setProgramFS(name, shader);

    glShaderSource( vs.index, 1, &vs.source, NULL);
    glCompileShader( vs.index);
    checkShader   ( vs.index);

    glShaderSource( fs.index, 1, &fs.source, NULL);
    glCompileShader( fs.index);
    checkShader   ( fs.index);


    glAttachShader(index, vs.index);
    glAttachShader(index, fs.index);

    /// bind vertex attributes
    glBindAttribLocation(index, 0, "fe_position");
    glBindAttribLocation(index, 1, "fe_normals");
    glBindAttribLocation(index, 2, "fe_uvs");


    /// link the program and check for errors
    glLinkProgram(index);

    checkLinking(index);
    validateProgram(index);

    cmd_queue->setupProgram(name);
}
*/
//void FE_Renderer::writeToLog(string log){
//    writeToLog(log.c_str());
//}
//void FE_Renderer::writeToLog(const char* log){

    //this->logfile = SDL_RWFromFile("FE_GL_log.txt", "a");
    //SDL_RWwrite(logfile, log, 1, strlen(log));
    //SDL_RWclose(this->logfile);
//}

/*bool FE_GLSimpleProgram::checkShader(GLuint index){
    // check shader compilation
    int parameters = -1;
    glGetShaderiv(index, GL_COMPILE_STATUS, &parameters);
    if(parameters != GL_TRUE){
        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetShaderInfoLog(index, max_length, &actual_length, log);
        writeToLog(log);
        return false;
    }
    return true;
}

bool FE_GLSimpleProgram::checkLinking(GLuint program_index){
    // check shader program link for errors
    int parameters = -1;
    glGetProgramiv(program_index, GL_LINK_STATUS, &parameters);
    if(GL_TRUE != parameters){
        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetProgramInfoLog(program_index, max_length, &actual_length, log);
        cout << log << endl;
        writeToLog(log);
    }
    return false;
}

bool FE_GLSimpleProgram::validateProgram(GLuint program){

    //make sure program is runnable
    glValidateProgram(program);
    int parameters = -1;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &parameters);
    if(GL_TRUE != parameters){
        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetProgramInfoLog(program, max_length, &actual_length, log);
        cout << log << endl;
        writeToLog(log);
        return false;
    }
    return true;
}*/

#ifdef FE_LINUX
void DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, void* userParam){
    if(type == GL_DEBUG_TYPE_ERROR_ARB){
        cout <<"Error"<< endl;
        //writeToLog("OPENGL: Error");
    }
    else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB){
        cout << "Deprecated behavior" << endl;
        //writeToLog("OPENGL: Deprecated behavior\n");
    }
    else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB){
        cout << "Undefined behavior" << endl;
        //writeToLog("OPENGL: Undefined behavior\n");
    }
    else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB){
        cout << "Portability" << endl;
        //writeToLog("OPENGL: Portability\n");
    }
    else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB){
        cout << "Performance" << endl;
        //writeToLog("OPENGL: Performance\n");
    }
    else if(type == GL_DEBUG_TYPE_OTHER_ARB){
        //writeToLog("OPENGL: Other\n");
        cout << "Other" << endl;
    }
    cout << message << endl;
    //writeToLog(message);

}
#endif
#ifdef FE_WINDOWS
void DebugCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam){
    if(type == GL_DEBUG_TYPE_ERROR_ARB){
        cout <<"Error"<< endl;
        //writeToLog("OPENGL: Error");
    }
    else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB){
        cout << "Deprecated behavior" << endl;
        //writeToLog("OPENGL: Deprecated behavior\n");
    }
    else if(type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB){
        cout << "Undefined behavior" << endl;
        //writeToLog("OPENGL: Undefined behavior\n");
    }
    else if(type == GL_DEBUG_TYPE_PORTABILITY_ARB){
        cout << "Portability" << endl;
        //writeToLog("OPENGL: Portability\n");
    }
    else if(type == GL_DEBUG_TYPE_PERFORMANCE_ARB){
        cout << "Performance" << endl;
        //writeToLog("OPENGL: Performance\n");
    }
    else if(type == GL_DEBUG_TYPE_OTHER_ARB){
        //writeToLog("OPENGL: Other\n");
        cout << "Other" << endl;
    }
    cout << message << endl;
    //writeToLog(message);

}
#endif
void writeToLog(string log){
    writeToLog(log.c_str());
}

void writeToLog(const char* log){
    SDL_RWops* logfile = SDL_RWFromFile("FE_GL_log.txt", "a");
    SDL_RWwrite(logfile, log, 1, strlen(log));
    SDL_RWclose(logfile);
}

