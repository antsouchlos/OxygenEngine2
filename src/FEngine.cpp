
#include "FEngine.h"

/* this function is called for every thread
 * it gets THREADDATA as an input pointer
 * handles threads and queued functions
 */
FEngine* FThreaddata::taskMgr = nullptr;


extern "C" int fluoric_update_thread(void* data){

    // fetch the task manager and the thread from the pointer

    // execute all functions which are bind to that thread
    FThreaddata* actual_data = static_cast<FThreaddata*>(data);
    FThreaddata::taskMgr->updateThread(actual_data->name);
    //cout << actual_data->name;
    delete actual_data;
    return 0;
}

FEngine::FEngine(){
    render_type = FE_OPENGL3_2;
    restart_renderer = false;
    done = false;
    createMutex();
}

void FEngine::Init(string titlea = "", int xa=800, int ya=600, bool fullscreen=false, FE_RENDERTYPE rend_type = FE_OPENGL3_2){

    //std::cout << "RUNNING" << endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    //cout << SDL_GetError() << endl;
    if(SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);

    //global_semaphore = SDL_CreateSemaphore(0);
	this->event_handler.init();
    this->info = new FE_GPU_Info();
    info->rend_type = render_type;
    this->api.init(render_type, info, titlea, xa, ya, false);
    restart_renderer = false;
    //this->keyboard = FKeyboard();
    this->mouse =  FMouse(this->keyboard.mutex);
    //this->shaderManager = new FGLShaderManager();
    this->title = titlea;
    done = false;
    this->x = xa;
    this->y = ya;
    this->framerate = 5;
    ///this->mutex = SDL_CreateMutex();
    this->synchronization_cond = SDL_CreateCond();
    /*this->thread_count = 0;
    this->threads_run = 0;
    this->threads_sync = false;
    this->start_threads = false;*/
    this->CreateNewThread("default", true);
    this->CreateNewThread("DefaultRenderer", true);
    this->AddTask("DefaultRenderer", updateRendererTaskA, "DefaultRenderer", this);
    // initializes the engineglClearColor(1.0, 1.0, 1.0, 1.0);
    //window = SDL_CreateWindow(this->title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->x, this->y, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    // SDL_EnableKeyRepeat(0,0);
    render_type = rend_type;

    //restartRenderer(FE_OPENGL3_2);
    //context = SDL_GL_CreateContext(window);

    //glClearColor(1.0f, 2.0f, 1.0f, 1.0f);

}

FEngine::~FEngine(){

}

void FEngine::restartRenderer(FE_RENDERTYPE typea){
    lockMutex();
    render_type = typea;
    restart_renderer = true;
    unlockMutex();
}
FKeyboard* FEngine::GetKeyboard(){
    return &this->keyboard;
}

FMouse* FEngine::GetMouse(){
    return &this->mouse;
}

void FEngine::Step(){
    // task that runs every frame
    //cout << "RUNNING" << endl;
    Uint32 ticks = SDL_GetTicks();

    /// update keyboard, mouse and joystick events
    //SDL_LockMutex(this->keyboard.mutex);
    //this->inputHandler();
    //SDL_UnlockMutex(this->keyboard.mutex);
	lockMutex();
	done = event_handler.update();
	unlockMutex();
	
    lockMutex();
    if(restart_renderer){
        //cout << "STARTING" << endl;
        //this->renderer.destroy();
        //this->renderer.initSDL(title, x, y, render_type);
        api.destroy();
        delete info;
        this->info = new FE_GPU_Info();
        info->rend_type = render_type;
        this->api.init(render_type, info, title, x, y, false);
    }
    restart_renderer = false;
    unlockMutex();

    api.update();

    /// be in sync with the framerate
    /// store it if it exceeds the desired rate
    unsigned int local_framerate = 0;

    lockMutex();
    local_framerate = framerate;
    unsigned int current_ticks = SDL_GetTicks()-ticks;
    unlockMutex();

    if( (local_framerate) > current_ticks ){
        SDL_Delay( local_framerate - (current_ticks) );
        this->current_framerate = 1000.0/(float)local_framerate ;
    }
    else{
        this->current_framerate = 1000.0/(float)(current_ticks);

    }

    /// add main thread to already
    /// finished-thread counter
    lockMutex();
    //cout << "main thread" << endl;
    completed_threads++;
    unlockMutex();

    if(areThreadsReady() || done == true){

        /// if this is the last motheyacking thread
        /// that slows down the game, signal all other threads
        /// to continue and reset the counter
        lockMutex();
        //cout << "" << endl;
        completed_threads = 0;
        SDL_CondBroadcast(synchronization_cond);
        unlockMutex();
    }
    else{

        /// if this is NOT dat last sh**
        /// wait indefinitely for a signal from the last thread
        lockMutex();
        SDL_CondWait(synchronization_cond, this->wmutex);
        unlockMutex();
    }
    cout << SDL_GetTicks() - ticks << endl;
}

// update keyboard (now defunct)
void FEngine::inputHandler(){

    // do this for every keyboard key
    for (auto key : this->keyboard.keys)
            this->keyboard.keys[key.first].delta = this->keyboard.keys[key.first].pressed;

    // do this for every mouse key
    for (char key : {'l', 'r', 'm'})
        this->mouse.keys[key].delta = this->mouse.keys[key].pressed;

    // do this for every joystick
    for(auto &gamepad : this->controllers){

        // do this for every joystick button
        for(auto &button : gamepad.buttons){
            //cout << gamepad->buttons[button.first]->delta
            gamepad.buttons[button.first].delta = gamepad.buttons[button.first].pressed;
        }
    }

    this->mouse.wheel_state = 0;
    // message processing loop
    SDL_Event event;
    while ( SDL_PollEvent(&event) ){

       bool keypress = false;
       bool mousepress = false;
       bool joypress = false;
       unsigned int id = 0;

       switch (event.type){

            // exit if the window is closed
            case SDL_QUIT:
                cout << "ABOUT TO QUIT" << endl;
                done = true;
                break;

            // check for presses
            case SDL_KEYDOWN:

                // do this for every key in the keyboard list
                for (auto key : this->keyboard.keyList){

                    // check if the key is pressed
                    if (event.key.keysym.sym == key.first){

                        keypress = true;
                        // switch key state to transitional just-pressed state

                        this->keyboard.keys[key.second].pressed = true;
                        }
                    }
                    break;
            // check for releases
            case SDL_KEYUP:
                // do this for every key in the keyboard list
                for (auto key : this->keyboard.keyList){

                    // check if the key is pressed
                    if (event.key.keysym.sym == key.first && keypress==false) {

                        // switch key state to transitional just-pressed state
                        this->keyboard.keys[key.second].pressed = false;

                    }
                }
                break;
            // update mouse position
            case SDL_MOUSEMOTION:

                SDL_GetMouseState(&this->mouse.x, &this->mouse.y);
                break;
            // update mouse down events
            case SDL_MOUSEBUTTONDOWN:

                switch(event.button.button){

                    // left mouse pressed
                    case SDL_BUTTON_LEFT:
                        mousepress = true;
                        this->mouse.keys['l'].pressed = true;
                        break;
                    // middle mouse pressed
                    case SDL_BUTTON_MIDDLE:
                        if(!mousepress){
                            mousepress = true;
                            this->mouse.keys['m'].pressed = true;
                        }
                        break;

                    // right mouse pressed
                    case SDL_BUTTON_RIGHT:
                        if(!mousepress){

                            //cout << "right is pressed" << endl;
                            this->mouse.keys['r'].pressed = true;
                            mousepress = true;
                        }
                        break;

                }

            // update mouse up events
            case SDL_MOUSEBUTTONUP:
                if(!mousepress){
                    switch(event.button.button){

                        // left mouse released
                        case SDL_BUTTON_LEFT:
                            this->mouse.keys['l'].pressed = false;
                            break;
                        // middle mouse released1
                        case SDL_BUTTON_MIDDLE:
                            this->mouse.keys['m'].pressed = false;
                            break;
                        // right mouse released
                        case SDL_BUTTON_RIGHT:
                            this->mouse.keys['r'].pressed = false;
                            break;
                    }
                }
                break;
            // update mouse wheel events
            case SDL_MOUSEWHEEL:
                if(event.wheel.y == 1 || event.wheel.y == -1){
                    this->mouse.wheel_state = event.wheel.y;
                    //cout << event.wheel.y << endl;;
                }
                break;
            // update joystick axis input (if any)
            case SDL_JOYAXISMOTION:

                id = event.jaxis.which;
                if(id < this->controllers.size()){

                    // determine the axis and its value
                    int axis = event.jaxis.axis;
                    int value = event.jaxis.value;
                    this->controllers[event.jaxis.which].axis[axis] = value;
                }
                break;
            // update joystick keydown events
            case SDL_JOYBUTTONDOWN:

                id = event.jbutton.which;
                if(id < this->controllers.size()){

                    joypress = true;
                    this->controllers[event.jbutton.which].buttons[event.jbutton.button].pressed = true;
                }
                break;
            // update joystick keyup events
            case SDL_JOYBUTTONUP:
                id = event.jbutton.which;
                if(id < this->controllers.size() && !joypress){

                    this->controllers[event.jbutton.which].buttons[event.jbutton.button].pressed = false;
                }
                break;
        }
    }
    //cout << "WORKS" << endl;
    SDL_GetRelativeMouseState(&this->mouse.dx, &this->mouse.dy);
    //cout << this->keyboard.keys["d"]->delta << " " <<this->keyboard.keys["d"]->pressed << endl;

}

void FEngine::CreateNewThread(string thread_name, bool synchro){

    FThreadstruct defaultThread = FThreadstruct();
    defaultThread.synchronize = synchro;
    defaultThread.changed = true;

    this->threads[thread_name] = defaultThread;

    lockMutex();

    FThreaddata::taskMgr = this;
    FThreaddata* data = new FThreaddata();
    data->name = thread_name;
    if(synchro)
        this->threadIDs[thread_name] = SDL_CreateThread(fluoric_update_thread, thread_name.c_str(), (void*)data);
    else
        this->unsync_threadIDs[thread_name] = SDL_CreateThread(fluoric_update_thread, thread_name.c_str(), (void*)data);
    unlockMutex();
    //cout << SDL_GetError() << endl;
    //cout << "RUNS" << endl;
}

//renderer task disabled easily
int FEngine::updateRendererTask(){

    FE_GPU_Thread* thread = this->api.createThread("DefaultRenderer");

    if(loaded_world_now)
        this->renderer.update(thread, this->worlds[loaded_world]->getScene(worlds[loaded_world]->loaded_scene), 0.5);
    /// TEST
    //cout << "thread NR  ----------------" << endl;
    /*FE_GPU_CommandQueue* cmd_queue = thread->createCommandQueue("default one");
    cmd_queue->createVertexBuffer("asdasd");
    //cout << "dat shit is ruining it" << endl;
    cmd_queue->setVertexBufferMemory("asdasd", 9, FE_GPU_DYNAMIC);
    cmd_queue->setVertexBufferData("asdasd", vector<GLfloat>({0.5, 0.0, 0.0, 0.0, 0.5, 0.0, -0.5, 0.0, 0.0}), 0);

    cmd_queue->createVertexLayout("dat shit");

    FE_GPU_VertexLayoutInput layout;
    vector<FE_GPU_VertexLayoutInput> inputs;
    layout.name = "asdasd";
    layout.amount = 3;
    layout.stride = 0;
    layout.offset = 0;
    inputs.push_back(layout);

    cmd_queue->setVertexLayoutFormat("dat shit", inputs);
    cmd_queue->setVLIndexBufferMemory("dat shit", 3, FE_GPU_DYNAMIC);
    cmd_queue->setVLIndexBufferData("dat shit", vector<GLuint>({0, 1, 2}), 0);

    cmd_queue->createUniform("motheryacker");
    cmd_queue->setUniformMemory("motheryacker", 4, FE_GPU_STREAM);
    cmd_queue->setUniformData("motheryacker", vector<GLfloat>({1.0, 1.0, 1.0, 1.0}), 0);
    cmd_queue->setUniformState("motheryacker", 1, 0, 0);
    //cmd_queue->setUniformState("motheryacker", 2, 0, 0);

    cmd_queue->createProgram("lel");
    FE_GPU_Shader vs;
    vs.type = FE_GPU_VERTEX_SHADER;
    vs.data = string(Shader(

    in vec3 fe_position;
    out vec3 normals;

    layout(std140) uniform FE_SceneData{
        vec4 perspective;
    };

    void main(){
        vec4 final_position = perspective*vec4(fe_position, 1.0);
        normals = fe_position;
        gl_Position = final_position;
    }));
    cmd_queue->setProgramVS("lel", vs);

    FE_GPU_Shader fs;
    fs.type = FE_GPU_PIXEL_SHADER;
    fs.data = string(Shader( precision mediump float;

    in vec3 normals;

    out vec4 LFragColor;
    void main(){
        LFragColor = vec4(normals, 1.0);
    }));
    //writeToLog("\n" + vs.data);
    //writeToLog("\n" + fs.data);
    cmd_queue->setProgramFS("lel", fs);
    cmd_queue->setupProgram("lel");
    //cmd_queue->submit();

    //auto cmd_queue1 = thread->createCommandQueue();
    cmd_queue->setProgramUniformSlot("lel", "FE_SceneData", 1);
    cmd_queue->drawInstances("lel", "dat shit", 0, 2, 3);
    cmd_queue->optimize();
     //SDL_Delay(1000);
    thread->submit("default one");*/
    thread->flush();

    /// ENDTEST
    return this->GetTaskInfo("DefaultRenderer", "DefaultRenderer").CONTINUE();
}

void FEngine::AddControllers(){


    unsigned int id = SDL_NumJoysticks();
    this->controllers.clear();
    for(unsigned int i=0; i<id; i++ ){
        this->controllers.push_back(FGamepad(i, this->keyboard.mutex, true));

    }
     SDL_JoystickEventState(SDL_ENABLE);

}

FGamepad* FEngine::GetController(unsigned int id){
    if(id< this->controllers.size())
        return &this->controllers[id];
    else{
        return nullptr;
    }
}

void FEngine::Destroy(){
    // clean up
    //cout << "DESTROYING" << endl;
    lockMutex();
    completed_threads++;
    unlockMutex();
    int thread_output;
    for (auto thread : this->threadIDs){

        SDL_WaitThread(thread.second, &thread_output);

    }
    // destroy all sdl related
    thread_output++;

    for(auto joystick : this->controllers){
        SDL_JoystickClose(joystick.joystick);
    }
    this->controllers.clear();
    //delete keyboard;
    //delete shaderManager;
    //this->shaderManager->destroy();

    SDL_DestroyCond(synchronization_cond);
    this->api.destroy();
    //delete renderer;
    SDL_Quit();
}

void FEngine::Start(){

    done = false;

    // starts and maintains the game engine
    while (!(done)){
        this->Step();
    }

    // if an error occurs or the program ends cleanup
    this->Destroy();
}
int updateRendererTaskA(void* data, FTask task){
    FEngine* engine = static_cast<FEngine*>(data);
    //cout << "DOING IT" << endl;
    engine->updateRendererTask();
    return 0;
}
