#include "FE_GPU_API.h"

FE_GPU_API::FE_GPU_API(){
    made_context = false;
}

FE_GPU_API::~FE_GPU_API(){

}

void FE_GPU_API::init(FE_RENDERTYPE rend_type, FE_GPU_Info* info, string titlea, int xa, int ya, bool IsFullscreen){

    this->rend_type = rend_type;
    title = titlea;
    width = xa;
    height = ya;
    fullscreen = IsFullscreen;
    //FE_GPU_VertexBuffer::rend_type = rend_type;
    FE_GPU_Command::rend_type = rend_type;
    //FE_GPU_VertexLayout::rend_type = rend_type;
    //FE_GPU_Uniform::rend_type = rend_type;
    //FE_GPU_Program::rend_type = rend_type;


    // set static variables
    //FE_GPU_VertexBuffer::info = info;
    FE_GPU_Command::info = info;
    //FE_GPU_Program::info = info;
    //FE_GPU_ProgramCommand::info = info;
    //FE_GPU_VertexLayout::info = info;
    //FE_GPU_VertexBufferCommand::info = info;
    //FE_GPU_VertexLayoutCommand::info = info;
    //FE_GPU_UniformCommand::info = info;
    //FE_GPU_DrawCommand::info = info;
    //cout <<"init" << endl;
      #ifdef FE_DEBUG

    /// get current time and date
    time_t t = time(0);
    struct tm* atm = localtime(&t);
    char       buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", atm);
    writeToLog("\n\n//////////////////////////");
    writeToLog("\n" + string(buf));
    #endif

    switch(rend_type){
        case FE_OPENGL3_2: FE_GL3_API::init(this, title, xa, ya, fullscreen); break;
        case FE_OPENGLES3: FE_GL3_API::init(this, title, xa, ya, fullscreen); break;
        default: cout << "";
    }

}

FE_GPU_Thread* FE_GPU_API::createThread(string a_name){

    this->lockMutex();

    for(auto thread: this->threads)
    if(thread->name == a_name)
        return thread;

    FE_GPU_Thread* newthread = new FE_GPU_Thread();
    newthread->name = a_name;
    newthread->init(rend_type);
    this->threads.push_back(newthread);

    this->unlockMutex();
    return newthread;
}

void FE_GPU_API::removeThread(string a_name){
    this->lockMutex();

    for(size_t x=0; x < this->threads.size(); x++)
    if(this->threads[x]->name == a_name){

        delete this->threads[x];
        this->threads.erase(this->threads.begin()+x);
        break;
    }

    this->unlockMutex();
}


void FE_GPU_API::update(){

    ///main freaking rendering loop

    while(FE_GPU_Thread::thread_counter != threads.size()){

        //cout << threads.size() << endl;
        for(auto thread : this->threads){
            //cout << "STUCK HERE" << endl;
            thread->update();
        }

        /// only get the he*l out of the while-loop
        /// if ALL threads have finished
        for(auto thread : threads){
            thread->lockMutex();

            /// guaranteed all pending commands will be
            /// executed before the end of the frame
            if(thread->flushed){
                FE_GPU_Thread::thread_counter+=1;
                thread->flushed = false;
                thread->run = false;
                thread->executed = false;
            }


            /// thread->flush signals the API manager (FE_GPU_API) to execute all pending commands
            /// on the next iteration of the main freaking rendering loop
            if(thread->run)
                thread->flushed = true;

            if(thread->executed)
                thread->run = true;
            thread->unlockMutex();

        }

    }
    FE_GPU_Thread::thread_counter = 0;
    //cout << "FINISHED" << endl;
    /// do platform-specific ... things to update the fu**ing frame
    switch(rend_type){
        case FE_OPENGL3_2: FE_GL3_API::update(this); break;
        case FE_OPENGLES3: FE_GL3_API::update(this); break;
        default: cout << "";
    }


}

void FE_GPU_API::destroy(){

    for(auto thread: threads){
        thread->destroy();
        delete thread;
    }

    switch(rend_type){
        case FE_OPENGL3_2: FE_GL3_API::destroy(this); break;
        case FE_OPENGLES3: FE_GL3_API::destroy(this); break;
        default: cout << "";
    }
}
