#include "FtaskManager.h"

//#include "FGLShaderManager.h"
#include "FKeyboard.h"
#include "FMouse.h"
#include "FGamepad.h"
//#include "FWorldManager.h"
#include <iostream>

/*
 * --------------------------------ATTENTION - IMPORTANT--------------------------------
 * I am not sure where to include the FE_Writer, so I am jst going to do it here
 */
//#include "CSL_Interpreter.h"

using namespace std;

class FEngine;
int updateRendererTaskA(void*, FTask);
//typedef function<bool()> FE_COMMAND;

struct FThreaddata{

    /* This struct is passed to the update_thread function
     * It passes the task manager pointer and the thread name it runs on
     * together with a boolean which tells the thread to be either
     * asynchronous or sync with other threads,
     * so it can run methods from classes inherited from FTaskManager class
     * without any fuss in another thread
     */
    static FEngine* taskMgr;
    string name;

};



class FEngine: public FtaskManager{

    public:

        FEngine();
        ~FEngine();


        void Init(string, int, int, bool, FE_RENDERTYPE);
        void restartRenderer(FE_RENDERTYPE);
        void CreateNewThread(string, bool);
        void Step();
        void Start();
        void Destroy();
        void AddControllers();
        int updateRendererTask();

        //FGLShaderManager*   GetShaderManager();
        FGamepad*           GetController(unsigned int);
        FKeyboard*          GetKeyboard();
        FMouse*             GetMouse();


    protected:


        string      title;
        int         x, y;
        FKeyboard   keyboard;
        FMouse      mouse;
        void inputHandler();
        vector<FGamepad> controllers;

        FE_GPU_API api;
        FE_GPU_Info* info = nullptr;
        //FGLShaderManager*  shaderManager;
        FE_RENDERTYPE render_type;
        int countar = 0;
    private:

        bool restart_renderer;
        //vector<SDL_sem*> starting_semaphores;

};
