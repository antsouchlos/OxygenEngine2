#include "FE_World.h"

FE_World::FE_World()
{
     createMutex();
}

FE_World::~FE_World()
{

}

FE_World* FE_World::loadScene(string name){
    lockMutex();
    this->loaded_scene = name;
    unlockMutex();
    return this;
}


bool FE_World::update(){

    return true;
};

bool FE_World::init(){
    return true;
};

bool FE_World::destroy(){
    for(auto scene: this->scenes){
        scene->destroy();
        delete scene;
    }
    scenes.clear();
    return true;
};

string FE_World::getName(){
    return name;
};
/*
FE_Scene* FE_World::loadSceneFromFile(string, string){

};
*/
FE_Scene* FE_World::createScene(string namea){
    lockMutex();

    FE_Scene* output = new FE_Scene();
    output->name = namea;
    scenes.push_back(output);

    unlockMutex();
    return output;
};

/*
FE_Scene* FE_World::createSceneFromFile(string namea, string filename){


};
*/
FE_Scene*  FE_World::getScene(string namea){

    for (auto scene: this->scenes)
    if  (scene->name == name)
        return scene;
    else
        continue;
    return nullptr;
};

vector<string> FE_World::getSceneNames(){
    vector<string> output;
    for(auto scene : this->scenes) output.push_back(scene->name);
    return output;
};

FE_Scene* FE_World::renameScene(string a_name, string prev_name){
    lockMutex();
    this->getScene(a_name)->name = prev_name;
    unlockMutex();
    return this->getScene(a_name);
};

bool FE_World::removeScene(string a_name){
    lockMutex();
    for(unsigned int i=0; i< scenes.size();i++)
    if(scenes[i]->name == a_name){
        delete scenes[i];
        scenes.erase(scenes.begin()+i);
    }

    unlockMutex();
    return true;
};
