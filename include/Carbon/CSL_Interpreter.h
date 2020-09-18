#ifndef CSL_INTERPRETER_H_
#define CSL_INTERPRETER_H_

#include "FE_Libs.h"

//all needed types are included in CSL_Writer.h
#include "CSL_Writer.h"
#include "CSL_Parser.h"
#include "Exceptions.h"

using namespace std;

class CSL_Interpreter {
public:
	FE_World *interpret(string sourceCode);
	FE_World *interpretFile(string pathToFile);
private:

	//parser to parse the code before interpreting
	CSL_Parser *parser;

	//return value of "interpret()"
	FE_World *world;

	//a pointer to the CNode currently beeing processed
	CNode *curNode;

	//void process();

	//a function for each type
	//the functions are called recursively (processWorld()->processScene()->processCamera->...)
	void processWorld(FE_World *world);
	void processScene(FE_Scene *scene);
        void processCamera(FE_Camera *camera);
        void processLight(FE_Light *light);
        void processInteraction(FE_Interaction *interaction);
        void processMesh(FE_Mesh *mesh);
        void processVertex(FE_Vertex *vertex);
        void processUVMap(FE_UVMap *uvmap);
        void processVertexGroup(FE_VertexGroup *vgroup);
        void processAnimation(FE_Animation *animation);
        void processTransformation(FE_Transformation *transformation);
        void processPolygon(FE_Polygon *polygon);
        void processPhysics(FE_Physics *physics);
        void processTexture(FE_Texture *texture);
        void processMaterial(FE_Material *material);
        void processTCM(FE_TCM *tcm);

};

#endif
