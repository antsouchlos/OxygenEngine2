#pragma once

#include "FE_MeshRenderData.h"

void DebugCallback(unsigned int source, unsigned int type,
unsigned int id, unsigned int severity,
                                  int length, const char* message, void* userParam);

struct FE_GeneralData{
    FE_Mat4 perspective_mat;
    FE_Mat4 view_mat;
    GLuint uniform_index;
    GLuint uniform_binding_offset = 1;
    bool created_ubo = false;
    GLuint soft_body_id[2];
};

struct FE_GLShader{
    GLuint index = 0;
    const GLchar* source;
};

/** This class is specific to OpenGL 3.2 / OpenGL ES 3.0
  * since the concept of 'programs' is only mandatory on those
  */


struct FE_Viewport{
    int width, height;
    int x, y;
};

class FE_Renderer : public FE_THREAD_SAFETY_OBJECT
{
    friend class FEngine;
    public:

        FE_Renderer();
        ~FE_Renderer();
        bool init(FE_Scene*);
        bool update(FE_GPU_Thread*, FE_Scene*, float);

        //void createThread(string);
        //void flushThread(string);
        //void removeThread(string);

        bool destroy(FE_GPU_Thread*);
        //void writeToLog(const char*);
        //void writeToLog(string);
        size_t findProgram(string);

    protected:

        void updateViewPorts(FE_Scene*, FE_GPU_Thread*, float);
        void updateObjects(FE_Scene*, FE_GPU_Thread*);
        void update2D(FE_Scene*, FE_GPU_Thread*);
        void updateUniforms(FE_GPU_Thread*);
        void renderObjects(FE_GPU_Thread*);
        void updateFonts(FE_Scene*, FE_GPU_Thread*);
        void renderFonts(FE_GPU_Thread*);
        void render2D(FE_GPU_Thread*);

        void checkShader(GLuint);
        void checkLinking(GLuint);
        void validateProgram(GLuint);

        //SDL_RWops* logfile;


        vector<FE_MaterialData> materials;
        FE_GeneralData general_data;
        bool made_context;
};


