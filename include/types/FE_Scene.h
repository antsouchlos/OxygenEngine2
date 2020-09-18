#ifndef FE_SCENE_H
#define FE_SCENE_H
#include "FE_Libs.h"
#include "FE_Mesh.h"
#include "FE_Light.h"
#include "FE_Camera.h"
#include "FE_Texture.h"
#include "FE_TCM.h"
#include "FE_Material.h"

using namespace std;


//void* glShaderSource = nullptr;

//class FE_GL_SceneInfo;
//class FE_Camera{public: string name;};
//class FE_Texture{public: string name;};
//class FE_Mesh{public: string name;};
//class FE_Light{public: string name;};
//class FE_TCM{public: string name;};
//class FE_Interaction{public: string name;};
//class FE_Material{public: string name;};

class FE_Scene :  public FE_THREAD_SAFETY_OBJECT
{
    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        friend class FE_World;
        friend class FWorldManager;

        FE_Scene();
        ~FE_Scene();

        bool update();
        void init();
        void destroy();

        string getName();

        float getGravity();
        FE_Scene* setGravity(float);

        string getGravityAxis();
        FE_Scene* setGravityAxis(string);

        FE_Camera* createCamera (string);
        FE_Camera* getCamera    (string);
        FE_Camera* renameCamera (string, string);
        FE_Scene* removeCamera       (string);
        vector<string> getCameraNames();

        FE_Light* createLight (string);
        FE_Light* getLight    (string);
        FE_Light* renameLight (string, string);
        FE_Scene* removeLight      (string);
        vector<string> getLightNames();

        FE_Mesh* createObject (string);
        FE_Mesh* getObject    (string);
        FE_Mesh* renameObject (string, string);
        FE_Scene* removeObject       (string);
        vector<string> getObjectNames();

        FE_Material* createMaterial (string);
        FE_Material* getMaterial    (string);
        FE_Material* renameMaterial (string, string);
        FE_Scene* removeMaterial         (string);
        vector<string> getMaterialNames();

        FE_Texture* createTexture (string);
        FE_Texture* getTexture    (string);
        FE_Texture* renameTexture (string, string);
        FE_Scene* removeTexture        (string);
        vector<string> getTextureNames();

        FE_TCM* createTCM (string);
        FE_TCM* getTCM    (string);
        FE_TCM* renameTCM (string, string);
        FE_Scene* removeTCM    (string);
        vector<string> getTCMNames();

        FE_Interaction* createInteraction (string);
        FE_Interaction* getInteraction    (string);
        FE_Interaction* renameInteraction         (string, string);
        FE_Scene* removeInteraction            (string);
        vector<string> getInteractionNames();

    protected:
        string name;

        float gravity;
        string gravityAxis;

        vector<string> groups;

        vector<FE_Camera*> cameras;
        vector<FE_Light*> lights;

        vector<FE_Interaction*> interactions;
        vector<FE_Mesh*> objects;
        vector<FE_Texture*> textures;
        vector<FE_Material*> materials;
        vector<FE_TCM*> textureCombineModes;

        //FE_GL_SceneInfo* rendererInfo;
    private:
};

#endif // FE_SCENE_H
