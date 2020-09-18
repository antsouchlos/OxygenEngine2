#ifndef FE_GPU_VERTEXBUFFER_H
#define FE_GPU_VERTEXBUFFER_H
#include "FE_EventHandler.h"

enum FE_RENDERTYPE{
    FE_OPENGL3_2,
    FE_OPENGL4_1,
    FE_OPENGL4_3,
    FE_OPENGL4_4,
    FE_DIRECT3D_11,
    FE_OPENGLES3_1,
    FE_OPENGLES3
};

enum FE_GPU_BUFFER_USAGE{
    FE_GPU_DYNAMIC,
    FE_GPU_STATIC,
    FE_GPU_STREAM
};

struct FE_VertexData{
    GLfloat pos[3];
    GLfloat nor[3];
    GLfloat uv[2];
};

/** Base class of all gpu commands
  *
  */
class FE_GPU_Thread;
class FE_GPU_Info;

class FE_GPU_Command{
public:
    FE_GPU_Command();
    virtual ~FE_GPU_Command();
    virtual void update()=0;
    virtual void flush(FE_GPU_Thread*)=0;

    static FE_GPU_Info* info;
    static FE_RENDERTYPE rend_type;
    bool destroyed;
    string name;
};

/**
  * Cross-platform way to submit vertex buffer commands to the GPU
  */

class FE_GPU_CreateVBOCMD: public FE_GPU_Command{
public:
    FE_GPU_CreateVBOCMD();
    ~FE_GPU_CreateVBOCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif
};

class FE_GPU_SetVBODataCMD: public FE_GPU_Command{
public:
    FE_GPU_SetVBODataCMD();
    ~FE_GPU_SetVBODataCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

    FE_GPU_BUFFER_USAGE usage;
    size_t memory_size;

};

class FE_GPU_SetVBOSubDataCMD: public FE_GPU_Command{
public:
    FE_GPU_SetVBOSubDataCMD();
    ~FE_GPU_SetVBOSubDataCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

    vector<GLfloat> data;
    size_t offset;

};

class FE_GPU_DestroyVBOCMD: public FE_GPU_Command{
public:
    FE_GPU_DestroyVBOCMD();
    ~FE_GPU_DestroyVBOCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif
};
/// OpenGL (ES) specific:
class FE_GPU_VertexBuffer;
class FE_GPU_Thread;

/** Internal structure:
  * -API only interests
  *  the authors of the engine
  * -simplifies any rewrite to another
  *  graphics API
  */


struct FE_GPU_VertexBuffer{

    string name;

    //#ifdef FE_ATTRIB_OPENGL
    size_t capacity;
    GLuint vertex_id;
    //#endif
};

#endif // FE_GPU_VERTEXBUFFER_H
