#ifndef FE_GPU_THREAD_H
#define FE_GPU_THREAD_H
#include "FE_GPU_CommandQueue.h"


enum FE_GPU_COMMAND_TYPE{

};
//class FE_GPU_CommandQueue{string name; vector<FE_GPU_Command*> lista void update(FE_GPU_API*);};

/** platform-specific namespaces
  * -OpenGL ES 3.0/ OpenGL 3.2
  * -OpenGL 4.1
  * -OpenGL 4.4 + GL_ARB_bindless_texture
  * -Direct3D 11 (maybe)
  */

//#ifdef FE_ATTRIB_OPENgl
class FE_GPU_Thread;
namespace FE_GL3_Thread{

    void init(FE_GPU_Thread*);
    void destroy(FE_GPU_Thread*);

};
//#endif

/** FE_GPU_Thread provides a platform-independent
  * interface for specifying and maintaining rendering
  * threads . It fakes them on single-threaded
  * renderes such as OpenGL
  */

class FE_GPU_Thread : public FE_THREAD_SAFETY_OBJECT
{
    public:
        FE_GPU_Thread();
        virtual ~FE_GPU_Thread();

        void init(FE_RENDERTYPE);
        void destroy();
        void update();

        FE_GPU_CommandQueue* createCommandQueue(string);
        void deleteCommandQueue(string);

        void flush();
        void submit(string);
        bool isFlushed();


        string name;
        bool flushed;
        bool run;
        bool executed;
        vector<FE_GPU_CommandQueue*> cmd_queues;
        vector<string> pending_cmd_queues;
        FE_RENDERTYPE rend_type;

        static size_t thread_counter;
    private:
};

#endif // FE_GPU_THREAD_H
