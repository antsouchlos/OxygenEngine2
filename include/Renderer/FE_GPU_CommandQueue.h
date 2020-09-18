#ifndef FE_GPU_COMMANDQUEUE_H
#define FE_GPU_COMMANDQUEUE_H
#include "FE_GPU_Info.h"

class FE_GPU_API;
class FE_GPU_CommandQueue;


/** FE_GPU_CommandQueue provides a platform-independent
  * interface for sending rendering commands to the renderer
  * Creating one and assigning commands to one is fully thread-safe
  * Assigned commands will execute as soon as submit() is called
  */

class FE_GPU_CommandQueue: public FE_THREAD_SAFETY_OBJECT
{
    public:
        friend class FE_Renderer;
        friend class FE_GPU_Thread;

        FE_GPU_CommandQueue(FE_GPU_Thread*);
        virtual ~FE_GPU_CommandQueue();

        void init();
        void destroy();
        void update();

        void createVertexBuffer(string);
        void setVertexBufferMemory(string, size_t, FE_GPU_BUFFER_USAGE);
        void setVertexBufferData(string, vector<GLfloat>, size_t);
        void deleteVertexBuffer(string);

        void createVertexLayout(string);
        void setVertexLayoutFormat(string, vector<FE_GPU_VertexLayoutInput>);
        void setVLIndexBufferData(string, vector<GLuint>, GLuint);
        void setVLIndexBufferMemory(string, GLuint, FE_GPU_BUFFER_USAGE);
        void deleteVertexLayout(string);

        void createProgram(string);
        void setProgramVS(string, FE_GPU_Shader);
        void setProgramGS(string, FE_GPU_Shader);
        void setProgramFS(string, FE_GPU_Shader);
        void setProgramTCS(string, FE_GPU_Shader);
        void setProgramTES(string, FE_GPU_Shader);
        void setupProgram(string);

        void setProgramUniformSlot(string, string, GLint);


        void createUniform(string);
        void setUniformMemory(string, GLuint, FE_GPU_BUFFER_USAGE);
        void setUniformData(string, vector<GLfloat>, GLuint);
        void setUniformData(string, vector<GLuint>, GLuint);
        void setUniformState(string, GLint, GLuint, GLuint);
        void deleteUniform(string);

        void createLargeBuffer(string, FE_GPU_BUFFER_USAGE);
        void setLargeBufferLength(string, int);
        void setLargeBuffer(string, GLfloat);

        void draw(string, string, int, int);
        void draw(string, string);
        void drawInstances(string, string, int);
        void drawInstances(string, string, int, int, int);

        void optimize();
        void Clear();
        vector<FE_GPU_Command*> commands;
        FE_RENDERTYPE rend_type;
        FE_GPU_Thread* thread;
        bool flushed;
        string name;
        static FE_GPU_Info* info;
    private:
};

#endif // FE_GPU_COMMANDQUEUE_H
