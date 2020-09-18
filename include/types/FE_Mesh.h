#ifndef FE_MESH_H
#define FE_MESH_H

#include "FE_Object.h"
#include "FE_Vertex.h"
#include "FE_Polygon.h"
#include "FE_VertexGroup.h"
#include "FE_Animation.h"
#include "FE_Interaction.h"
using namespace std;
//class FE_Vertex{public: int index;};
//class FE_VertexGroup{public: std::string name;};
//class FE_Polygon{public: int index; FE_Polygon(int index, int v1, int v2, int v3){}};
//class FE_Physics{public: int i;};
//class FE_Animation{public: std::string name;};

class FE_Mesh : public FE_Object
{
    friend class FE_Renderer;
    friend class FE_MeshRenderData;

    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_Mesh();
        ~FE_Mesh();
        bool init();
        bool destroy();

        bool hasMaterial(string);
        bool hasPolygonMaterial(string, size_t);

        FE_Vertex* createVertex();
        FE_Vertex* getVertex(size_t);
        FE_Vertex* getLastVertex(int);
        FE_Mesh* removeVertex(int);
        size_t       getNumVertices();


        FE_Polygon* createPolygon(int, int, int);
        FE_Polygon* getPolygon(int);
        FE_Polygon* getLastPolygon(int);
        FE_Mesh*  removePolygon(int);
        size_t         getNumPolygons();


        FE_VertexGroup* createVertexGroup(string);
        FE_VertexGroup* getVertexGroup(string);
        FE_Mesh* renameVertexGroup(string, string);
        FE_Mesh* removeVertexGroup(string);

        FE_Animation* createAnimation(string);
        FE_Animation* getAnimation(string);
        FE_Mesh* renameAnimation(string, string);
        FE_Mesh* removeAnimation(string);

        FE_Physics* getPhysics();
        FE_Physics* getPhysicsProperties();

        // boiler plate functions inherited from FE_Object
        FE_Mesh* setData(void*);
        FE_Mesh* setPos(FE_Vec4);
        FE_Mesh* setRotEuler(FE_Vec4);
        FE_Mesh* setRot(FE_Vec4);
        FE_Mesh* setScale(FE_Vec4);

        FE_Mesh* setX(float);
        FE_Mesh* setY(float);
        FE_Mesh* setZ(float);
        FE_Mesh* setRotX(float);
        FE_Mesh* setRotY(float);
        FE_Mesh* setRotZ(float);
        FE_Mesh* setScaleX(float);
        FE_Mesh* setScaleY(float);
        FE_Mesh* setScaleZ(float);
    protected:
        vector<FE_Vertex*> vertices;
        vector<FE_VertexGroup*> vertex_groups;
        vector<FE_Animation*> animations;
        // 1.2 {
        vector<FE_Polygon*> polygons;
        // 1.2}
        FE_Physics *physics;
        vector<string> lights;
    private:
};

#endif // FE_MESH_H
