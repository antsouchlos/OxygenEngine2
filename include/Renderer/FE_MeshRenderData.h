#ifndef FE_MESHRENDERDATA_H
#define FE_MESHRENDERDATA_H

#include "CSL_Interpreter.h"
#include "FE_GPU_API.h"
#include "FMath.h"

#define Shader(src) "#version 150 \n" #src

struct FE_MeshLightData{
    FE_Vec4 position;
    FE_Vec4 rotation;
    FE_Vec4 color;
    float range;
    float value;
    float fov;
    float this_type;
    /// GAMO TON COMPILER MOU MESA
    /// DEN DOULEUEI AYTOMATA TO == OUTE TO =
    bool compare(const FE_MeshLightData&);
    void assignValue(const FE_MeshLightData&);

};

struct FE_MeshVertexData{

    vector<GLfloat> vertices;
    GLuint offset;
    vector<GLuint>  index_buffer;
    GLuint ib_offset;
    vector<GLfloat> light_data;
    GLuint light_offset;

};

class FE_MeshRenderData
{
    public:
        FE_MeshRenderData();
        ~FE_MeshRenderData();

        void init(FE_GPU_Thread*, FE_MeshVertexData, FE_Mat4);
        void destroy(FE_GPU_Thread*);

        void getVertices(FE_Mesh*, string);
        void getObjectData(FE_Scene*, FE_Mesh*);

        void update(FE_GPU_Thread*);
        void draw(FE_GPU_CommandQueue*);
        void drawDummy(FE_GPU_Thread*);

        string name;
        string mat;
        string gpu_names;
        FE_Mat4 model_mat;

        size_t ib_size;
        FE_MeshVertexData obj_data;
        size_t num_polygons;
        size_t vertices_size = 0;
        bool created = false;
        GLuint voffset;
        GLuint vstart;
        GLuint vend;
    private:
        map<string, FE_MeshLightData> light_data;
};

class FE_MaterialData{
public:
    FE_MaterialData();
    ~FE_MaterialData();

    void init(FE_GPU_Thread*, FE_Material);
    void destroy(FE_GPU_Thread*);

    void update(FE_GPU_Thread*);
    void draw(FE_GPU_CommandQueue*);
    void updateVertices(FE_GPU_Thread*);
    void chooseProgram(FE_GPU_CommandQueue*);
    void draw(FE_GPU_Thread*);
    size_t computeVBOlength(size_t);
    size_t computeIBlength(size_t);

    string name;
    string gpu_names;
    vector<FE_MeshRenderData> meshes;
    size_t previous_mesh_size = 0;
    FE_Material updated_mat;
    bool created = false;
private:
    FE_Material old_mat;

    //vector<string> mdata;
    //GLuint ubo_binding;
};

#endif // FE_MESHRENDERDATA_H
