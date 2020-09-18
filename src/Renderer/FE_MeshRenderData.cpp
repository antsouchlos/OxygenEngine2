#include "FE_MeshRenderData.h"

bool FE_MeshLightData::compare(const FE_MeshLightData &other){
    bool output = true;
    output = output && position == other.position;
    output = output && rotation == other.rotation;
    output = output && color == other.color;

    output = output && (fabs(range- other.range) < FE_EPSILON);
    output = output && (fabs(fov- other.fov) < FE_EPSILON);
    output = output && (fabs(this_type- other.this_type) < FE_EPSILON);
    output = output && (fabs(value- other.value) < FE_EPSILON);
    return output;
}

void FE_MeshLightData::assignValue(const FE_MeshLightData& other){
    this->position = other.position;
    this->rotation = other.rotation;
    this->color = other.color;
    this->range = other.range;
    this->fov = other.fov;
    this->value = other.value;
    this->this_type = other.this_type;
 }

FE_MeshRenderData::FE_MeshRenderData()
{
    //ctor
}

FE_MeshRenderData::~FE_MeshRenderData()
{
    //dtor
}

/** This method is run everytime
  * the vertex/polygon layout is changed
  * and calculates the new layout
  */

void FE_MeshRenderData::getVertices(FE_Mesh* mesh, string mat_name){

    if(mesh->getVertexGroup(mat_name)->getNumPolygons() == this->vertices_size) return;

    FE_MeshVertexData data;
    data.offset = 0;
    data.ib_offset = 0;
    /// make a temporary index vector to copy vertices to the ID buffer
    /*vector<GLuint> temporary_ib_data;
    //size_t global_offset = 0;
    ///calculate vertex layout
    for(size_t i = 0; i < mesh->getVertexGroup(mat_name)->getNumPolygons(); i++){
        for(auto o : {0, 1, 2}){

            int id_found = 0;
            auto v = mesh->vertices[mesh->getVertexGroup(mat_name)->polygons[i]*3+o];
            bool found = false;
            for(size_t x=0; x < temporary_ib_data.size(); x++){

                if(mesh->vertices[temporary_ib_data[x]]->equivalents(v)){

                    id_found = x;
                    found = true;
                    //global_offset++;
                    break;
                }
            }
            if(!found){
                id_found = v->n;
                temporary_ib_data.push_back(mesh->vertices[mesh->getVertexGroup(mat_name)->polygons[i]*3+o]->n);
            }
            data.index_buffer.push_back(id_found);
        }
    }*/


    for(size_t i = 0; i < mesh->getVertexGroup(mat_name)->getNumPolygons(); i++){
        for(auto o : {0, 1, 2}){

            auto v = mesh->vertices[mesh->getVertexGroup(mat_name)->polygons[i]*3+o];
            //bool found = false;

            /*for(size_t x = 0; x< vertex_indices.size(); x++){
                if(mesh->vertices[vertex_indices[x]]->equivalents(v)){

                    data.index_buffer.push_back(v->n);
                    found = true;
                    break;
                }
                else if(mesh->vertices[vertex_indices[x]]->n == v->n){

                    data.index_buffer.push_back(v->n);
                    found = true;
                    break;
                }

            }
            if(!found){

             data.index_buffer.push_back(v->n);
            }*/
            data.index_buffer.push_back(v->n);
        }
    }
    vector<GLuint> actual_vertex_indices;
    GLuint CONSTANT_SIZE = *max_element(data.index_buffer.begin(), data.index_buffer.end());
    //CONSTANT_SIZE = mesh->vertices.size();
    actual_vertex_indices.reserve(CONSTANT_SIZE);
    cout << CONSTANT_SIZE << endl;
    for(GLuint x=0; x <= CONSTANT_SIZE; x++){
        /// upload vertex data
        FE_Vertex vertex;
        for(auto i : mesh->vertices){
            if(i->n ==x)
               vertex = *i;
        }
        data.vertices.push_back(vertex.x);
        data.vertices.push_back(vertex.y);
        data.vertices.push_back(vertex.z);

        /// upload vertex normal data
        data.vertices.push_back(vertex.normalx);
        data.vertices.push_back(vertex.normaly);
        data.vertices.push_back(vertex.normalz);

        /// upload uv coordinates
        if(vertex.getUVMapNames().size()>0){
            data.vertices.push_back(vertex.getUVMap(vertex.getUVMapNames()[0])->getUV().u);
            data.vertices.push_back(vertex.getUVMap(vertex.getUVMapNames()[0])->getUV().v);
        }
        else{
            data.vertices.push_back(1.0f);
            data.vertices.push_back(1.0f);
        }
    }
    if(!created){
        //for(auto x : vertex_indices)
        //writeToLog(to_string(x) + " ");
        //writeToLog("\n");
    }

    //obj_data.index_buffer = index_buffer;
    this->obj_data.ib_offset = 0;
    //this->obj_data.index_buffer.push_back()
    this->obj_data = data;

}

/** This method is run everytime
  * the vertex/polygon layout is changed
  * and calculates the new layout
  */

void FE_MeshRenderData::getObjectData(FE_Scene* scenea, FE_Mesh* objecta){

    vector<FE_MeshLightData> local_light_data;

    vector<GLfloat> output;
    /// dont change the model matrix if unnecessary
    obj_data.light_offset = 0;

    model_mat = objecta->getModelMatrix();
    for(auto x : model_mat.toArray())
        output.push_back(x);
    obj_data.light_offset = 0;


    if(objecta->lights.size() > 16) return;

    for(size_t x=0; x< objecta->lights.size(); x++){

        FE_Light light = *scenea->getLight(objecta->lights[x]);

        /// copy all relevant data
        local_light_data.push_back(FE_MeshLightData());

        local_light_data.back().position = light.getPos();
        local_light_data.back().rotation = light.getRotEuler();
        local_light_data.back().color = light.getColor();

        local_light_data.back().range = light.getRange();
        local_light_data.back().fov = light.getFOV();
        local_light_data.back().value = light.getValue();

        GLfloat light_type = 0.0f;
        if(light.getType() == "POINT")
            light_type = 1.0f;
        else if(light.getType() == "SUN")
            light_type = 2.0f;
        else if(light.getType() == "AMBIENT")
            light_type = 3.0f;
        else if(light.getType() == "SPOT")
            light_type = 4.0f;

        local_light_data.back().this_type = light_type;
        this->light_data[objecta->lights[x]].this_type = light_type;
        if(local_light_data.back().compare(this->light_data[objecta->lights[x]])){

            output.push_back(local_light_data.back().position.x);
            output.push_back(local_light_data.back().position.y);
            output.push_back(local_light_data.back().position.z);
            output.push_back(local_light_data.back().position.w);

            output.push_back(local_light_data.back().rotation.x);
            output.push_back(local_light_data.back().rotation.y);
            output.push_back(local_light_data.back().rotation.z);
            output.push_back(local_light_data.back().rotation.w);

            output.push_back(local_light_data.back().color.x);
            output.push_back(local_light_data.back().color.y);
            output.push_back(local_light_data.back().color.z);
            output.push_back(local_light_data.back().color.w);

            output.push_back(local_light_data.back().range);
            output.push_back(local_light_data.back().fov);
            output.push_back(local_light_data.back().value);
            output.push_back(local_light_data.back().this_type);
            this->light_data[objecta->lights[x]].assignValue(local_light_data.back());
        }
    }
    while(output.size() < 16+16*16)
        output.push_back(0.0f);
    //writeToLog(to_string(output.size()));
    this->obj_data.light_data = output;
    //cout << obj_data.light_data.size() << endl;
}

/** This method is run every frame
  * and stores every change in the buffers
  */

void FE_MeshRenderData::update(FE_GPU_Thread* thread/*, string obj, size_t id*/){

    this->gpu_names = "_FE_" + this->name + "_FE_" + this->mat + "_FE_";
    if(this->obj_data.vertices.size() == 0 && this->obj_data.index_buffer.size() == 0 && this->obj_data.light_data.size()==0) return;


    if(!created){
        FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue(gpu_names + "new_vertices");
        /*for(size_t x=0; x < obj_data.vertices.size(); x+=8){
        writeToLog(to_string(obj_data.vertices[x+0]) + " ");
        writeToLog(to_string(obj_data.vertices[x+1]) + " ");
        writeToLog(to_string(obj_data.vertices[x+2]) + " \n");

        writeToLog(to_string(obj_data.vertices[x+3]) + " ");
        writeToLog(to_string(obj_data.vertices[x+4]) + " ");
        writeToLog(to_string(obj_data.vertices[x+5]) + " \n");

        writeToLog(to_string(obj_data.vertices[x+6]) + " ");
        writeToLog(to_string(obj_data.vertices[x+7]) + " \n");

        }
        for(size_t x=0; x < obj_data.index_buffer.size(); x+=3){
            writeToLog(to_string(obj_data.index_buffer[x+0]) + " ");
            writeToLog(to_string(obj_data.index_buffer[x+1]) + " ");
            writeToLog(to_string(obj_data.index_buffer[x+2]) + " \n");
        }*/
        /// stores all vertices
        cmd_queue->createVertexBuffer(gpu_names);
        cmd_queue->setVertexBufferMemory(gpu_names, this->obj_data.vertices.size(), FE_GPU_STATIC);
        //for(auto x : obj_data)
        //cout << << endl;
        cmd_queue->setVertexBufferData(gpu_names, this->obj_data.vertices, 0);

        /// stores the index buffer (set here)
        /// and the layout of the vertex shader (not set here)
        cmd_queue->createVertexLayout(gpu_names);
        vector<FE_GPU_VertexLayoutInput> inputs;
        FE_GPU_VertexLayoutInput input;
        input.name = gpu_names;
        // position
        /// amount is the goddamn number (of GLfloats [4 bytes]) used in each f**king vertex shader instance in the GPU
        input.amount = 3;
            /// offset is the freaking initial offset (in GLfloats [4 bytes]) from the start of the buffer sh**
        input.offset = 0;
        /// stride is the motheryacking difference (in GLfloats [4 bytes]) between each fu**ng instance of vertex attribute
        input.stride = 8;
        inputs.push_back(input);

        /// i can use the same FE_GPU_VertexLayoutInput since std::vector makes MOTHERYACKING COPIES OF THE INSTANCES
        /// that said in other to prevent further goddamn questions on dat sh**
        // normals
        input.amount = 3;
        input.offset = 3;
        input.stride = 8;
        inputs.push_back(input);

        // uvs
        input.amount = 2;
        input.offset = 6;
        input.stride = 8;
        inputs.push_back(input);

        /// initialize the vertex layout + index buffer
        cmd_queue->setVertexLayoutFormat(gpu_names, inputs);
        cmd_queue->setVLIndexBufferMemory(gpu_names, this->obj_data.index_buffer.size(), FE_GPU_STATIC);
        cmd_queue->setVLIndexBufferData(gpu_names, this->obj_data.index_buffer, 0);

        /// initialize the lights and position layout
        /// the size will NEVER change ... EVER
        cmd_queue->createUniform(gpu_names);
        cmd_queue->setUniformMemory(gpu_names, obj_data.light_data.size(), FE_GPU_STREAM);
        cmd_queue->setUniformData(gpu_names, obj_data.light_data, 0);

        cmd_queue->optimize();
        thread->submit(gpu_names + "new_vertices");

        /// store the buffer to reset the size automatically if vertices change
        this->vertices_size = obj_data.vertices.size();
        this->ib_size = obj_data.index_buffer.size();
        created = true;
        //writeToLog("NOTHING IS IMPOSSIBLE");
    }
    else{

        thread->deleteCommandQueue("_FE_" + this->name + "_FE_" + mat + "vertices");
        //cout << " " << endl;
        FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue("_FE_" + this->name + "_FE_" + mat + "vertices");
        cmd_queue->Clear();
        /// change size of vertex buffer
        if(this->vertices_size != this->obj_data.vertices.size() && this->obj_data.vertices.size() != 0 && this->obj_data.offset == 0)
            cmd_queue->setVertexBufferMemory(gpu_names, this->obj_data.vertices.size(), FE_GPU_STATIC);

        /// change size of index buffer
        if(this->ib_size != this->obj_data.index_buffer.size() && this->obj_data.index_buffer.size() != 0 && this->obj_data.ib_offset == 0)
            cmd_queue->setVLIndexBufferMemory(gpu_names, this->obj_data.index_buffer.size(), FE_GPU_STATIC);

        /******************************/
        /// update data of vertex buffer
        if(this->obj_data.vertices.size() != 0)
            cmd_queue->setVertexBufferData(gpu_names, this->obj_data.vertices, this->obj_data.offset);

        /// update data of index buffer
        if(this->obj_data.index_buffer.size() != 0)
            cmd_queue->setVLIndexBufferData(gpu_names, this->obj_data.index_buffer, this->obj_data.ib_offset);

        ///update data of index buffer
        if(this->obj_data.light_data.size() != 0)
            cmd_queue->setUniformData(gpu_names, obj_data.light_data, 0);

        cmd_queue->optimize();
        thread->submit("_FE_" + this->name + "_FE_" + mat + "vertices");
    }
    this->obj_data.light_data.clear();
    this->obj_data.vertices.clear();
    this->obj_data.index_buffer.clear();
}

void FE_MeshRenderData::drawDummy(FE_GPU_Thread* thread){
    /// simple draw with elementary shader
    FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue("_FE_" + this->name + "_FE_" + mat + "drawDummy");
    cmd_queue->drawInstances("_FE_dummy_FE_", gpu_names, 0, 0, (GLuint)this->ib_size);
    cmd_queue->optimize();
    thread->submit("_FE_" + this->name + "_FE_" + mat + "drawDummy");

}

void FE_MeshRenderData::draw(FE_GPU_CommandQueue* cmd_queue){

    cmd_queue->setUniformState(gpu_names, 2, 0, 0);

    cmd_queue->drawInstances("_FE_" + this->mat + "_FE_", gpu_names, 0, 0, (GLuint)this->ib_size);

}
/****************************************/
// MATERIAL RENDER DATA
/****************************************/
FE_MaterialData::FE_MaterialData(){

}
FE_MaterialData::~FE_MaterialData(){

}

void FE_MaterialData::init(FE_GPU_Thread* thread, FE_Material material){



    FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue(name + "b");
    cmd_queue->createVertexBuffer(this->name);
    cmd_queue->createVertexLayout(this->name);
    thread->submit(name + "b");

}

void FE_MaterialData::update(FE_GPU_Thread* thread){

    if(!(this->updated_mat == old_mat)){
        old_mat = updated_mat;
        //writeToLog("DOING IT");
        thread->deleteCommandQueue("_FE_" + this->name + "_FE_" + "create_material");
        FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue("_FE_" + this->name + "_FE_" + "create_material");

        cmd_queue->createUniform("_FE_" + this->name + "_FE_" + "uniform");
        if(!created){
            cmd_queue->setUniformMemory("_FE_" + this->name + "_FE_" + "uniform", old_mat.getMaterialData().size(), FE_GPU_DYNAMIC);
            created = true;
        }

        cmd_queue->setUniformData("_FE_" + this->name + "_FE_" + "uniform", old_mat.getMaterialData(), 0);

        chooseProgram(cmd_queue);
        cmd_queue->optimize();
        thread->submit("_FE_" + this->name + "_FE_" + "create_material");

    }

}
void FE_MaterialData::chooseProgram(FE_GPU_CommandQueue* cmd_queue){

    ///cmd_queue->deleteProgram("_FE_" + this->mat + "_FE_");
    cmd_queue->createProgram("_FE_" + this->name + "_FE_");

    /// set shader sources
    FE_GPU_Shader shader;
    shader.type = FE_GPU_VERTEX_SHADER;
    shader.data = string(Shader(

    struct FE_MeshLightData{
        vec4 position;
        vec4 rotation;
        vec4 color;
        vec4 general;
    };

    in vec3 fe_position;
    in vec3 fe_normals;
    in vec2 fe_uvs;

    out vec4 pos;
    out vec4 normals;
    out vec2 uvs;

    layout(std140) uniform FE_SceneData{
        mat4 perspective;
        mat4 view;
    };

    layout(std140) uniform FE_Object{
        mat4 model_matrix;
        FE_MeshLightData lights[16];
    };

    void main(){
        pos = transpose(view)*transpose(model_matrix)*vec4(fe_position, 1.0);
        normals = normalize(transpose(model_matrix)*vec4(fe_normals, 1.0));
        uvs = fe_uvs;
        vec4 final_position = perspective*transpose(view)*transpose(model_matrix)*vec4(fe_position, 1.0);

        gl_Position = final_position;

    }));

    cmd_queue->setProgramVS("_FE_" + this->name + "_FE_", shader);

    shader.type = FE_GPU_PIXEL_SHADER;
    shader.data = string(Shader(

    precision mediump float;

    in vec4 pos;
    in vec4 normals;
    in vec2 uvs;

    struct FE_MeshLightData{
        vec4 position;
        vec4 rotation;
        vec4 color;
        vec4 general;
    };
    layout(std140) uniform FE_SceneData{
        mat4 perspective;
        mat4 view;
    };

    layout(std140) uniform FE_Object{
        mat4 model_matrix;
        FE_MeshLightData lights[16];
    };

    layout (std140) uniform FE_Material{
        vec4 diffuse_color;
        vec4 specular_color;
        vec4 general_data;
        vec4 translucency;
        vec4 illuminosity;
    };

    out vec4 fragColor;

    vec3 shading(in vec3 light, in vec3 pos, in vec3 normal,in vec3 diffuse_intensity,in float light_type, in float specular_intensity){

        if(light_type > 0.5f && light_type < 1.0f){
            /// point light
            vec3 s = normalize(light-pos);
            vec3 v = normalize(-pos);

            //vec3 r = reflect(-s, tnorm);
            float sDotN = max( dot(normal,s), 0.0f);

            vec3 diffuse =  diffuse_intensity*sDotN;
            vec3 specular = vec3(0.0f);

            if(sDotN>0.0f){
                float outputa = pow(max(dot(normalize(v+s), normal), 0.0f), specular_intensity);
                //if(outputa < 0.8f) specular = vec3(0.2f);
                //else
                specular = vec3(outputa);

            }
            return diffuse+specular;
        }
        else if (light_type > 1.0f && light_type < 2.0f){

            /// directional light (sun)
            vec3 n = normalize(light);
            vec3 v = normalize(-pos);

            float sDotN = max( dot(normal, n), 0.0f);

            vec3 diffuse =  diffuse_intensity*sDotN;
            vec3 specular = vec3(0.0f);

            if(sDotN>0.0f){
                float outputa = pow(max(dot(normalize(v+n), normal), 0.0f), specular_intensity);
                //if(outputa < 0.8f) specular = vec3(0.2f);
                //else
                specular = vec3(outputa);

            }
            return diffuse+specular;
        }
    }

    vec3 toVec3(vec4 inputa){
        return inputa.xyz;
    }

    void main(){
        //fragColor = vec4(1.0, 1.0, 1.0, 1.0);
        vec4 actual_output = vec4(0.0, 0.0, 0.0, 1.0);
        //actual_output = normals;
        //for(int i=0; i < 16; i++){
        //    actual_output += vec4(shading(toVec3(lights[i].position), toVec3(pos), toVec3(normals), toVec3(diffuse_color), general_data[2], 1.0), 0.0);
        //}
        actual_output += vec4(shading(toVec3(transpose(view)*vec4(0.0, 0.0, 15.0, 1.0)),
                                      toVec3(pos),
                                       toVec3(normals), vec3(1.0, 0.0, 0.0), 1.4, 5.0), 1.0);

        fragColor = actual_output;
    }));
    cmd_queue->setProgramFS("_FE_" + this->name + "_FE_", shader);
    cmd_queue->setupProgram("_FE_" + this->name + "_FE_");

    cmd_queue->setProgramUniformSlot("_FE_" + this->name + "_FE_", "FE_Object", 2);
    cmd_queue->setProgramUniformSlot("_FE_" + this->name + "_FE_", "FE_SceneData", 0);
    cmd_queue->setProgramUniformSlot("_FE_" + this->name + "_FE_", "FE_Material", 1);
}

void FE_MaterialData::draw(FE_GPU_Thread* thread){

    /// if meshes are not the same size
    /// recalculate the size
    if(meshes.size() != previous_mesh_size)
        thread->deleteCommandQueue("_FE_" + this->name + "_FE_" + "draw");
    previous_mesh_size = meshes.size();

    /// set material-wide state
    /// (material uniform)
    //cout << "DOING IT" << endl;
    FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue("_FE_" + this->name + "_FE_" + "draw");
    cmd_queue->setUniformState("_FE_" + name + "_FE_" + "uniform", 1, 0, 0);

    for(FE_MeshRenderData obje : this->meshes){

        /// set object-wide state
        /// (vertex buffer/layout + uniform)
        obje.draw(cmd_queue);
    }

    cmd_queue->optimize();
    thread->submit("_FE_" + this->name + "_FE_" + "draw");

}
void FE_MaterialData::destroy(FE_GPU_Thread* thread){


}
