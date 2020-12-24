#ifndef CSL_INTERPRETER_H_
#define CSL_INTERPRETER_H_

#include <algorithm>
#include <sstream>

#include <types/OE_World.h>

#include <Carbon/CSL_Exceptions.h>
#include <Carbon/CSL_Parser.h>


// TODO
/*extern char const CSL_IteratorElementString[] = "element";
extern char const CSL_IteratorAttributeString[] = "attribute";
extern char const CSL_IteratorVariableString[] = "variable";

using UnknownMemberElementError = UnknownMemberError<CSL_IteratorElementString>;
using UnknownMemberAttributeError = UnknownMemberError<CSL_IteratorAttributeString>;
using UnknownMemberVariableError = UnknownMemberError<CSL_IteratorVariableString>;*/

class UnknownMemberElementError {
	public:
		UnknownMemberElementError(std::string_view &s) {};
};

using UnknownMemberAttributeError = UnknownMemberElementError;
using UnknownMemberVariableError = UnknownMemberElementError;


// This is a helper function for the interpreter
void OE_ReverseBitset(std::bitset<64>&);

class CSL_Interpreter {
public:
	std::shared_ptr<OE_World> interpret(std::string sourceCode);
	std::shared_ptr<OE_World> interpretFile(std::string pathToFile);
    
	OE_SharedIndexMap<OE_Scene>          scenesList;
	OE_SharedIndexMap<OE_Object>         objectsList;
	OE_SharedIndexMap<OE_Material>       materialsList;
	OE_SharedIndexMap<OE_Texture>        texturesList;
	OE_SharedIndexMap<OE_TCM>            tcmsList;
	OE_SharedIndexMap<OE_ViewportConfig> viewportsList;
private:
	//a pointer to the CNode currently beeing processed
	std::unique_ptr<CSL_Element> curElement;

	//a function for each type
	//the functions are called recursively (processWorld()->processScene()->processCamera->...)
	std::shared_ptr<OE_World>          processWorld(CSL_Element_ptr world_e);
	std::shared_ptr<OE_Scene>          processScene();
    std::shared_ptr<OE_Camera>         processCamera();
    std::shared_ptr<OE_Light>          processLight();
    //void processInteraction(OE_Interaction *interaction);
    std::shared_ptr<OE_Mesh32>         processMesh();
    //void processVertex(OE_Vertex *vertex);
    void processUVMapData(OE_UVMapData &uvmap, const std::size_t&);
    OE_VertexGroup *    processVertexGroup();
    //void processAnimation(OE_Animation *animation);
    //void processTransformation(OE_Transformation *transformation);
    void processTriangle(OE_Mesh32 *mesh, const std::size_t& num_of_uvs);
    //void processPhysics(OE_Physics *physics);
    std::shared_ptr<OE_Texture>  processTexture();
    std::shared_ptr<OE_Material> processMaterial();
    std::shared_ptr<OE_TCM>      processTCM(CSL_Element_ptr tcm_e);
//    void processTCM_Texture(OE_TCM_Texture &tcm_texture);
    std::shared_ptr<OE_ViewportConfig> processViewportConfig(CSL_Element_ptr vconf_e);
    
};

#endif
