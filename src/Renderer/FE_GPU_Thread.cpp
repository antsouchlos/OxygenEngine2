#include "FE_GPU_Thread.h"

size_t FE_GPU_Thread::thread_counter = 0;

FE_GPU_Thread::FE_GPU_Thread()
{
    run = false;
    flushed = false;
}

FE_GPU_Thread::~FE_GPU_Thread()
{
    //dtor
}

void FE_GPU_Thread::init(FE_RENDERTYPE typea){
    this->rend_type = typea;
    switch(rend_type){
        case FE_OPENGL3_2: FE_GL3_Thread::init(this); break;
        case FE_OPENGLES3: FE_GL3_Thread::init(this); break;
        default: cout << "";
    }
}

void FE_GPU_Thread::destroy(){
    for(auto cmd_queue: cmd_queues){
        cmd_queue->destroy();
        delete cmd_queue;
    }

    switch(rend_type){
        case FE_OPENGL3_2: FE_GL3_Thread::destroy(this); break;
        case FE_OPENGLES3: FE_GL3_Thread::destroy(this); break;
        default: cout << "";
    }
}

void FE_GPU_Thread::update(){

    //cout << "HERE IT COMES" << endl;
    //flushed = false;
    lockMutex();
    vector<string> _FE_local_pending_cmd_queues = pending_cmd_queues;
    //cout << "size" << _FE_local_pending_cmd_queues.size() << endl;
    for(auto cmd_name: _FE_local_pending_cmd_queues){

        for(auto cmd : cmd_queues)
        if(cmd->name == cmd_name){
            cmd->update();
            //cout << cmd->name << endl;
        }
        /// never forget to run ->submit( INSERT NAME HERE ) on your fu**ing
        /// command queues , else they will NEVER execute
        /*if(cmd_queue[x]->flushed){
            cmd_queue[x]->update();

            /// delete all heap-allocated sh**
            delete cmd_queue[x];
            cmd_queue.erase(cmd_queue.begin()+x);
            x--;
        }*/


    }
    /*if(cmd_queue.size() == 0){
        lockMutex();
        flushed = true;
        unlockMutex();
    }*/
    pending_cmd_queues.clear();
    unlockMutex();
}


void FE_GPU_Thread::flush(){

    lockMutex();
    this->executed = true;
    unlockMutex();
}

FE_GPU_CommandQueue* FE_GPU_Thread::createCommandQueue(string a_name){
    for(auto cmd: cmd_queues)
    if(cmd->name == a_name){
        return cmd;
    }
    this->cmd_queues.push_back(new FE_GPU_CommandQueue(this));
    cmd_queues.back()->name = a_name;
    return this->cmd_queues[this->cmd_queues.size()-1];
}

void FE_GPU_Thread::deleteCommandQueue(string a_name){
    for(size_t x=0; x < cmd_queues.size(); x++)
    if(cmd_queues[x]->name == a_name){
        cmd_queues[x]->destroy();
        cmd_queues.erase(cmd_queues.begin() + x);
        break;
    }
}

void FE_GPU_Thread::submit(string a_name){
    for(auto cmd: cmd_queues){
        if(cmd->name == a_name){
            for(auto command: cmd->commands)
                command->flush(this);
        }
    }
    lockMutex();
    pending_cmd_queues.push_back(a_name);
    unlockMutex();
}
