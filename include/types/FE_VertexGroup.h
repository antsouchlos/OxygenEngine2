#ifndef FE_VERTEXGROUP_H
#define FE_VERTEXGROUP_H

#include "FE_Libs.h"

using namespace std;
class FE_VertexGroup : public FE_THREAD_SAFETY_OBJECT
{
    friend class FE_Mesh;
    friend class FE_MeshRenderData;

    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_VertexGroup();
        ~FE_VertexGroup();
        string getType();
        FE_VertexGroup* setType(string val);

        string getTypeName();
        FE_VertexGroup* setTypeName(string val);

        FE_VertexGroup* assignPolygon(size_t);
        FE_VertexGroup* removePolygon(size_t);
        size_t getNumPolygons();
    protected:
        string name;
        string material;
        string bone_name;
        vector<size_t> polygons;
};

#endif // FE_VERTEXGROUP_H
