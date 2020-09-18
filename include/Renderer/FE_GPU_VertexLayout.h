#ifndef FE_GPU_VERTEXLAYOUT_H
#define FE_GPU_VERTEXLAYOUT_H
#include "FE_GPU_VertexBuffer.h"


struct FE_GPU_VertexLayoutInput{
    string name;
    size_t offset;
    GLuint amount;
    GLuint stride;
};

/**
  * Cross-platform way to submit vertex layout commands to the GPU
  */

class FE_GPU_VertexLayoutCommand : public FE_GPU_Command{
public:
    FE_GPU_VertexLayoutCommand();
    ~FE_GPU_VertexLayoutCommand();

    void update() final;
    void flush(FE_GPU_Thread*) final;


    static FE_GPU_Info* info;

    string name;
    vector<FE_GPU_VertexLayoutInput> inputs;
    vector<GLuint> ib_buffer;

    bool inputs_update;
    bool ib_buffer_update;
};

class FE_GPU_CreateVAOCMD : public FE_GPU_Command{
public:
    FE_GPU_CreateVAOCMD();
    ~FE_GPU_CreateVAOCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif
};

class FE_GPU_SetupVAOCMD : public FE_GPU_Command{
public:
    FE_GPU_SetupVAOCMD();
    ~FE_GPU_SetupVAOCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

    vector<FE_GPU_VertexLayoutInput> inputs;
};

class FE_GPU_SetVAOIBDataCMD : public FE_GPU_Command{
public:
    FE_GPU_SetVAOIBDataCMD();
    ~FE_GPU_SetVAOIBDataCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

    FE_GPU_BUFFER_USAGE usage;
    size_t memory_size;
};

class FE_GPU_SetVAOIBSubDataCMD : public FE_GPU_Command{
public:
    FE_GPU_SetVAOIBSubDataCMD();
    ~FE_GPU_SetVAOIBSubDataCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

    vector<GLuint> data;
    size_t offset;
};

class FE_GPU_DestroyVAOCMD : public FE_GPU_Command{
public:
    FE_GPU_DestroyVAOCMD();
    ~FE_GPU_DestroyVAOCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

};


/** Internal structure:
  * -API only interests
  *  the authors of Fluorin Engine
  * -simplifies potential rewrite to another
  *  graphics API
  */

struct FE_GPU_VertexLayout{

    /// OpenGL - specific
    //#ifdef FE_ATTRIB_OPENGL
    GLuint vao_id;
    GLuint ib_id;
    size_t capacity;
    //#endif
    string name;
    vector<FE_GPU_VertexLayoutInput> vertex_buffers;
};

#endif // FE_GPU_VERTEXLAYOUT_H
