#ifndef FE_VERTEX_H
#define FE_VERTEX_H

#include "FE_Libs.h"
#include "FMath.h"
using namespace std;

struct FE_UV{
    float u; float v;
};

class FE_UVMap: public FE_THREAD_SAFETY_OBJECT{
    friend class FE_Vertex;

    friend class CSL_Interpreter;
    friend class CSL_Writer;

public:
    FE_UVMap();
    ~FE_UVMap();

    FE_UVMap* setU(float);
    FE_UVMap* setV(float);
    FE_UV getUV();


protected:
    string name;

    float u = 0;
    float v = 0;

};

class FE_Vertex : public FE_THREAD_SAFETY_OBJECT
{
    friend class FE_Mesh;
    friend class FE_Renderer;
    friend class FE_MeshRenderData;

    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_Vertex(unsigned int);
        FE_Vertex();
        ~FE_Vertex();

        bool init();
        bool destroy();

        size_t getIndex();
        FE_Vertex* setIndex(size_t);

        FE_Vec4 getPos();
        FE_Vertex* setPos(FE_Vec4);
        FE_Vertex* setX(float);
        FE_Vertex* setY(float);
        FE_Vertex* setZ(float);

        FE_Vec4 getNormals();
        FE_Vertex* setNormals(FE_Vec4);
        FE_Vertex* setNormalX(float);
        FE_Vertex* setNormalY(float);
        FE_Vertex* setNormalZ(float);

        FE_UVMap* createUVMap(string);
        FE_UVMap* getUVMap(string);
        FE_Vertex* renameUVMap(string, string);
        FE_Vertex* removeUVMap(string);
        vector<string> getUVMapNames();

        bool equivalents(FE_Vertex*);
    protected:
        size_t n;  // there is a good reason i use size_t and not (unsigned/signed) int...
		size_t id; //size_t goddamn it, not string

        float x = 0;
        float y = 0;
        float z = 0;

        float normalx = 0;
        float normaly = 0;
        float normalz = 0;

    vector<FE_UVMap*> uvmaps;
    private:
};

#endif // FE_VERTEX_H
