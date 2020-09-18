#ifndef FE_GPU_UNIFORM_H
#define FE_GPU_UNIFORM_H
#include "FE_GPU_VertexLayout.h"

struct FE_GPU_UniformState{
    GLint slot;
    GLuint offset;
    GLuint length;
};

class FE_GPU_CreateUBOCMD: public FE_GPU_Command{
public:
    FE_GPU_CreateUBOCMD();
    ~FE_GPU_CreateUBOCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif
};

class FE_GPU_SetUBODataCMD: public FE_GPU_Command{
public:
    FE_GPU_SetUBODataCMD();
    ~FE_GPU_SetUBODataCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif

    FE_GPU_BUFFER_USAGE usage;
    size_t memory_size;

};

class FE_GPU_SetUBOSubDataCMD: public FE_GPU_Command{
public:
    FE_GPU_SetUBOSubDataCMD();
    ~FE_GPU_SetUBOSubDataCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif
    vector<GLuint> data_u;
    vector<GLfloat> data;
    size_t offset;

};

class FE_GPU_SetUBOSlotCMD: public FE_GPU_Command{
public:
    FE_GPU_SetUBOSlotCMD();
    ~FE_GPU_SetUBOSlotCMD();

    void update() final;
    void flush(FE_GPU_Thread*) final;

    //#ifdef FE_ATTRIB_OPENGL
    void updateGL3();
    void flushGL3(FE_GPU_Thread*);
    //#endif
    FE_GPU_UniformState state;
};

class FE_GPU_DestroyUBOCMD: public FE_GPU_Command{
public:
    FE_GPU_DestroyUBOCMD();
    ~FE_GPU_DestroyUBOCMD();

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

struct FE_GPU_Uniform
{
    string name;
    static FE_RENDERTYPE rend_type;
    bool initialized;
    FE_GPU_UniformState state;

    //#ifdef FE_ATTRIB_OPENGL
    GLuint buffer_id;
    size_t capacity;
    //#endif

};

#endif // FE_GPU_UNIFORM_H
