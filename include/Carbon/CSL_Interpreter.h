#ifndef CSL_INTERPRETER_H_
#define CSL_INTERPRETER_H_

#include <types/OE_World.h>

#include <Carbon/CSL_Parser.h>
#include <Carbon/CSL_Exceptions.h>

// This is a helper function for the interpreter
void OE_ReverseBitset(std::bitset<64>&);

class CSL_Interpreter {
public:
	std::shared_ptr<OE_World> interpret(std::string sourceCode);
	std::shared_ptr<OE_World> interpretFile(std::string pathToFile);
    
    ~CSL_Interpreter();
    
private:

	//parser to parse the code before interpreting
	CSL_Parser *parser{nullptr};

	//return value of "interpret()"
	//OE_World *world; UNNECESSARY

	//a pointer to the CNode currently beeing processed
	CSL_Node *curNode{nullptr};

	//a function for each type
	//the functions are called recursively (processWorld()->processScene()->processCamera->...)
	std::shared_ptr<OE_World>          processWorld();
	OE_Scene*          processScene();
    OE_Camera*          processCamera();
    OE_Light*           processLight();
    //void processInteraction(OE_Interaction *interaction);
    OE_Mesh32 *         processMesh();
    //void processVertex(OE_Vertex *vertex);
    void processUVMapData(OE_UVMapData &uvmap, const std::size_t&);
    OE_VertexGroup *    processVertexGroup();
    //void processAnimation(OE_Animation *animation);
    //void processTransformation(OE_Transformation *transformation);
    void processTriangle(OE_Mesh32 *mesh, const std::size_t& num_of_uvs);
    //void processPhysics(OE_Physics *physics);
    OE_Texture * processTexture();
    OE_Material * processMaterial();
    OE_TCM * processTCM();
    void processTCM_Texture(OE_TCM_Texture &tcm_texture);
    OE_ViewportConfig * processViewportConfig();

};

#endif
