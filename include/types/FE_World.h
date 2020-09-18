#ifndef FE_WORLD_H
#define FE_WORLD_H
#include "FE_Scene.h"

using namespace std;



class FE_World : public FE_THREAD_SAFETY_OBJECT
{
    friend class FEngine;
    friend class FWorldManager;

    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_World();
        ~FE_World();

        FE_World* loadScene(string);

        bool update();

        bool init();
        bool destroy();
        string getName();
        //FE_Scene* loadSceneFromFile(string, string);
        FE_Scene* createScene(string);
        //FE_Scene* createSceneFromFile(string, string);

        FE_Scene*      getScene(string);
        vector<string> getSceneNames();

        FE_Scene* renameScene(string, string);
        bool      removeScene(string);

    protected:
        string name;
        vector<FE_Scene*> scenes;
        string loaded_scene;
    private:
};

#endif // FE_WORLD_H
