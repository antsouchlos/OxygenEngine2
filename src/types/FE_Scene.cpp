#include "FE_Scene.h"

FE_Scene::FE_Scene()
{
     createMutex();
}

FE_Scene::~FE_Scene()
{
    //dtor
}

void FE_Scene::destroy(){
    for(auto x : materials){
        delete x;
    }
    materials.clear();

    for(auto x : lights){
        x->destroy();
        delete x;
    }
    lights.clear();

    for(auto x : objects){
        x->destroy();
        delete x;
    }
    objects.clear();

    for(auto x : textures){
        delete x;
    }
    textures.clear();

    for(auto x : interactions){
        delete x;
    }
    interactions.clear();

    for(auto x : textureCombineModes){
        x->destroy();
        delete x;
    }
    textureCombineModes.clear();

    for(auto x : cameras){
        x->destroy();
        delete x;
    }
    cameras.clear();
    groups.clear();
}

bool FE_Scene::update(){
    return true;
}
void FE_Scene::init(){

}

//DONE
string FE_Scene::getName(){

    lockMutex();
    string output = name;
    unlockMutex();
    return output;
}

//DONE
float FE_Scene::getGravity(){

    lockMutex();
    float output = gravity;
    unlockMutex();
    return output;
}

//DONE
FE_Scene* FE_Scene::setGravity(float value){

    lockMutex();
    gravity = value;
    unlockMutex();
    return this;
}

//DONE
string FE_Scene::getGravityAxis(){

    lockMutex();
    string output = gravityAxis;
    unlockMutex();
    return output;
}

//DONE
FE_Scene* FE_Scene::setGravityAxis(string axis){

    lockMutex();
    this->gravityAxis = axis;
    unlockMutex();
    return this;
}

// CAMERAS //////////////////////////////

//DONE
FE_Camera* FE_Scene::createCamera (string a_name){

    lockMutex();

    FE_Camera* output = new FE_Camera();
    output->name = a_name;
    cameras.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Camera* FE_Scene::getCamera    (string a_name){
    lockMutex();

    FE_Camera* output = nullptr;

    for(auto obj : cameras)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;
}
//DONE
FE_Camera* FE_Scene::renameCamera (string a_name, string prev_name){

    lockMutex();
    this->getCamera(a_name)->name = prev_name;
    unlockMutex();
    return this->getCamera(a_name);
}
//DONE
FE_Scene* FE_Scene::removeCamera       (string a_name){
    lockMutex();
    for(unsigned int i=0; i< cameras.size();i++)
    if(cameras[i]->name == a_name){
        delete cameras[i];
        cameras.erase(cameras.begin()+i);
    }

    unlockMutex();
    return this;
}
//DONE
vector<string> FE_Scene::getCameraNames(){
    lockMutex();
    vector<string> output;
    for(auto x : cameras) output.push_back(x->name);
    unlockMutex();
    return output;
}

//LIGHTS ////////////////////////////////

//DONE
FE_Light* FE_Scene::createLight (string a_name){
    lockMutex();

    FE_Light* output = new FE_Light();
    output->name = a_name;
    lights.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Light* FE_Scene::getLight    (string a_name){
    lockMutex();

    FE_Light* output = nullptr;

    for(auto obj : lights)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;

}
//DONE
FE_Light* FE_Scene::renameLight (string a_name, string prev_name){

    lockMutex();
    this->getLight(a_name)->name = prev_name;
    unlockMutex();
    return this->getLight(a_name);
}
//DONE
FE_Scene* FE_Scene::removeLight      (string a_name){
    lockMutex();
    for(unsigned int i=0; i< lights.size();i++)
    if(lights[i]->name == a_name){
        delete lights[i];
        lights.erase(lights.begin()+i);
    }

    unlockMutex();
    return this;
}
//DONE
vector<string> FE_Scene::getLightNames(){
    lockMutex();
    vector<string> output;
    for(auto x : lights) output.push_back(x->name);
    unlockMutex();
    return output;
}

// OBJECTS ////////////////////////////////

//DONE
FE_Mesh* FE_Scene::createObject (string a_name){
    lockMutex();

    FE_Mesh* output = new FE_Mesh();
    output->name = a_name;
    objects.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Mesh* FE_Scene::getObject    (string a_name){
    lockMutex();

    FE_Mesh* output = nullptr;

    for(auto obj : objects)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;
}
//DONE
FE_Mesh* FE_Scene::renameObject (string a_name, string prev_name){

    lockMutex();
    this->getObject(a_name)->name = prev_name;
    unlockMutex();
    return this->getObject(a_name);
}
//DONE
FE_Scene* FE_Scene::removeObject       (string a_name){
    lockMutex();
    for(unsigned int i=0; i< objects.size();i++)
    if(objects[i]->name == a_name){
        delete objects[i];
        objects.erase(objects.begin()+i);
    }

    unlockMutex();
    return this;
}
//DONE
vector<string> FE_Scene::getObjectNames(){

    lockMutex();
    vector<string> output;
    for(auto x : objects) output.push_back(x->name);
    unlockMutex();
    return output;
}

//MATERIAL ////////////////////////////////
//DONE
FE_Material* FE_Scene::createMaterial (string a_name){
    lockMutex();

    FE_Material* output = new FE_Material();
    output->name = a_name;
    materials.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Material* FE_Scene::getMaterial(string a_name){
    lockMutex();

    FE_Material* output = nullptr;

    for(auto obj : materials)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;
}
//DONE
FE_Material* FE_Scene::renameMaterial (string a_name, string prev_name){
    lockMutex();
    this->getMaterial(a_name)->name = prev_name;
    unlockMutex();
    return this->getMaterial(a_name);
}
//DONE
FE_Scene* FE_Scene::removeMaterial         (string a_name){
    lockMutex();
    for(unsigned int i=0; i< materials.size();i++)
    if(materials[i]->name == a_name){
        delete materials[i];
        materials.erase(materials.begin()+i);
    }

    unlockMutex();
    return this;
}
//DONE
vector<string> FE_Scene::getMaterialNames(){
    lockMutex();
    vector<string> output;
    for(auto x : materials) output.push_back(x->name);
    unlockMutex();
    return output;
}

//TEXTURES ////////////////////////////////
//DONE
FE_Texture* FE_Scene::createTexture (string a_name){
    lockMutex();

    FE_Texture* output = new FE_Texture();
    output->name = a_name;
    textures.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Texture* FE_Scene::getTexture(string a_name){
    lockMutex();

    FE_Texture* output = nullptr;

    for(auto obj : textures)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;
}
//DONE
FE_Texture* FE_Scene::renameTexture (string a_name, string prev_name){
    lockMutex();
    this->getTexture(a_name)->name = prev_name;
    unlockMutex();
    return this->getTexture(a_name);
}
//DONE
FE_Scene* FE_Scene::removeTexture        (string a_name){
    lockMutex();
    for(unsigned int i=0; i< textures.size();i++)
    if(textures[i]->name == a_name){
        delete textures[i];
        textures.erase(textures.begin()+i);
    }

    unlockMutex();
    return this;
}
//DONE
vector<string> FE_Scene::getTextureNames(){
    lockMutex();
    vector<string> output;
    for(auto x : textures) output.push_back(x->name);
    unlockMutex();
    return output;
}

//TCM ////////////////////////////////
//DONE
FE_TCM* FE_Scene::createTCM (string a_name){
    lockMutex();

    FE_TCM* output = new FE_TCM();
    output->name = a_name;
    textureCombineModes.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_TCM* FE_Scene::getTCM(string a_name){
    lockMutex();

    FE_TCM* output = nullptr;

    for(auto obj : textureCombineModes)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;
}
//DONE
FE_TCM* FE_Scene::renameTCM (string a_name, string prev_name){
    lockMutex();
    this->getTCM(a_name)->name = prev_name;
    unlockMutex();
    return this->getTCM(a_name);
}
//DONE
FE_Scene* FE_Scene::removeTCM    (string a_name){
    lockMutex();
    for(unsigned int i=0; i< textureCombineModes.size();i++)
    if(textureCombineModes[i]->name == a_name){
        delete textureCombineModes[i];
        textureCombineModes.erase(textureCombineModes.begin()+i);
    }

    unlockMutex();
    return this;
}
//DONE
vector<string> FE_Scene::getTCMNames(){
    lockMutex();
    vector<string> output;
    for(auto x : textureCombineModes) output.push_back(x->name);
    unlockMutex();
    return output;
}

//INTERACTION ////////////////////////////////
//DONE
FE_Interaction* FE_Scene::createInteraction (string a_name){
    lockMutex();

    FE_Interaction* output = new FE_Interaction();
    output->name = a_name;
    interactions.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Interaction* FE_Scene::getInteraction(string a_name){
    lockMutex();

    FE_Interaction* output = nullptr;

    for(auto obj : interactions)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;
}
//DONE
FE_Interaction* FE_Scene::renameInteraction         (string a_name, string prev_name){
    lockMutex();
    this->getInteraction(a_name)->name = prev_name;
    unlockMutex();
    return this->getInteraction(a_name);
}
//DONE
FE_Scene* FE_Scene::removeInteraction            (string a_name){
    lockMutex();
    for(unsigned int i=0; i< interactions.size();i++)
    if(interactions[i]->name == a_name){
        delete interactions[i];
        interactions.erase(interactions.begin()+i);
    }

    unlockMutex();
    return this;
}
//DONE
vector<string> FE_Scene::getInteractionNames(){
    lockMutex();
    vector<string> output;
    for(auto x : interactions) output.push_back(x->name);
    unlockMutex();
    return output;
}

