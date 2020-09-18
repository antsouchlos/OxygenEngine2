#include "FWorldManager.h"

FWorldManager::FWorldManager()
{
    loaded_world_now = false;
    loaded_world = 0;
}

FWorldManager::~FWorldManager()
{
    for(auto world : worlds){
        world->destroy();
        delete world;
    }
}
bool FWorldManager::init(){


    return true;
};
bool FWorldManager::destroy(){
    return true;
};

bool FWorldManager::storeWorld(string filename, bool erase_contents){

    return true;
}

FE_World* FWorldManager::loadWorld(string filename){


    ifstream in(filename.c_str());

    if(in.fail())
        throw FileNotFoundException(filename);

    // 'process' file
    string str((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    //vector<FE_World*> temporary_worlds;
    //reader.sortToTypes(temporary_worlds, str);
    //temporary_worlds.reserve(1);
    //temporary_worlds[0] = interpreter.interpret(str);
    //for(auto world : temporary_worlds)
        worlds.push_back(interpreter.interpret(str));
        writeToLog("worlds: " + to_string(worlds.size() ) );
        //cout << "WHATEVER IS GOING ON"<< worlds.size() << endl;
    //lockMutex();



    /* loaded_world = 0;
     //cout << "loading" << endl;
    //string name = worlds[0]->name;
    //cout << worlds[0] << endl;
    FE_World* world = getWorld();
    //cout << "loading world" << endl;
    //cout << world->getSceneNames()[0] << endl;

    world->loaded_scene = world->scenes[0]->name;
    loaded_world_now = true;
    //cout << loaded_world->loaded_scene << endl;
    //cout << "asd" << endl;*/
    //unlockMutex();
    in.close();

    //str.clear();

    return nullptr;
};

FE_World* FWorldManager::setActive(string a_name){

    FE_World* output = nullptr;

    for(size_t i=0; i < worlds.size(); i++){
    //cout << "LOADED_WORLD" << endl;
    if(worlds[i]->name == a_name){

        loaded_world = i;
        loaded_world_now = true;
		cout << "LOADED_WORLD" << endl;
        worlds[i]->loaded_scene =  worlds[i]->scenes[0]->name;
        output = worlds[i];
    }
	}
    if(output == nullptr) throw WorldNotFoundException(a_name);
    return output;
}
FE_World* FWorldManager::getWorld(){

    FE_World* output = nullptr;
    lockMutex();

    // check if world exists
    if(worlds.size() !=0)
    for(size_t i = 0; i <= loaded_world; i++){
        if(i == loaded_world)
            output = worlds[i];
    }


    if(output == nullptr) throw WorldNotFoundException("");
    unlockMutex();
    return output;
};

FE_Mesh* FWorldManager::getObject(string scene, string obj){
    return this->getWorld()->getScene(scene)->getObject(obj);
};

bool FWorldManager::deleteWorld(){

    this->getWorld()->destroy();
    delete getWorld();

    for(unsigned int i=0; i< worlds.size(); i++){
    if(worlds[i] == getWorld())
        worlds.erase(worlds.begin()+i);
    }

    loaded_world_now = false;
    return true;
};

bool FWorldManager::update(){
    this->getWorld()->update();
    return true;
};

// implement mutexes
FE_THREAD_SAFETY_OBJECT::FE_THREAD_SAFETY_OBJECT(){this->wmutex = nullptr; /*this->wmutex = SDL_CreateMutex();*/ changed = false;}
FE_THREAD_SAFETY_OBJECT::~FE_THREAD_SAFETY_OBJECT(){SDL_DestroyMutex(wmutex);}

void FE_THREAD_SAFETY_OBJECT::createMutex(){

    this->wmutex = nullptr;
    //cout << "runs at full speed" << endl;
}

void FE_THREAD_SAFETY_OBJECT::lockMutex(){
    if(this->wmutex == nullptr) wmutex =SDL_CreateMutex();
    SDL_LockMutex(wmutex);
}

void FE_THREAD_SAFETY_OBJECT::unlockMutex(){
    SDL_UnlockMutex(wmutex);
}
