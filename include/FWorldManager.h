#include "FE_Renderer.h"
class FWorldManager : public FE_THREAD_SAFETY_OBJECT
{
    public:
        FWorldManager();
        virtual ~FWorldManager();

        virtual bool init();
        virtual bool destroy();

        bool       storeWorld(string, bool);
        FE_World*  loadWorld(string);
        FE_World*  setActive(string);
        FE_World*  getWorld();
        FE_Mesh*   getObject(string, string);
        bool       deleteWorld();
        bool       update();

    protected:
        //FE_Reader reader;
	CSL_Interpreter interpreter;
        FE_Renderer renderer;
        vector<FE_World*> worlds;
        size_t loaded_world;
        bool loaded_world_now;
    private:
};
