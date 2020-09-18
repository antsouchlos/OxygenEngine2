#ifndef FE_POLYGON_H
#define FE_POLYGON_H

#include "FE_Libs.h"
#include "FMath.h"

using namespace std;
class FE_Polygon : public FE_THREAD_SAFETY_OBJECT
{
    friend class FE_Mesh;

    friend class CSL_Writer;
    friend class CSL_Interpreter;

    public:
        FE_Polygon(int, int, int, int);
        ~FE_Polygon();

        FE_Polygon* setIndex(int);
        int getIndex();

        FE_Polygon* setNormal(FE_Vec4);
        FE_Vec4 getNormal();

        FE_Polygon* setVertices(int, int, int);
        vector<size_t> getVertices();
    protected:
        size_t n;
        FE_Vec4 normal;
        int fake_normal;
        vector<size_t> vertexNums;
    private:
};

#endif // FE_POLYGON_H
