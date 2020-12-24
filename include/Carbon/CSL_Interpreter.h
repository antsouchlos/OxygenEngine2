#ifndef CSL_INTERPRETER_H_
#define CSL_INTERPRETER_H_

#include <types/OE_World.h>

#include <Carbon/CSL_Exceptions.h>
#include <Carbon/CSL_Parser.h>


//// This is a helper function for the interpreter
//void OE_ReverseBitset(std::bitset<64>&);
//
//class CSL_Interpreter {
//public:
//	std::shared_ptr<OE_World> interpret(std::string sourceCode);
//	std::shared_ptr<OE_World> interpretFile(std::string pathToFile);
//    
//	OE_SharedIndexMap<OE_Scene>          scenesList;
//	OE_SharedIndexMap<OE_Object>         objectsList;
//	OE_SharedIndexMap<OE_Material>       materialsList;
//	OE_SharedIndexMap<OE_Texture>        texturesList;
//	OE_SharedIndexMap<OE_TCM>            tcmsList;
//	OE_SharedIndexMap<OE_ViewportConfig> viewportsList;
//private:
//	//a pointer to the CNode currently beeing processed
//	std::unique_ptr<CSL_Element> curElement;
//
//	//a function for each type
//	//the functions are called recursively (processWorld()->processScene()->processCamera->...)
//	std::shared_ptr<OE_World>          processWorld(CSL_Element_ptr world_e);
//	std::shared_ptr<OE_Scene>          processScene(CSL_Element_ptr scene_e);
//    std::shared_ptr<OE_Camera>         processCamera(CSL_Element_ptr camera_e);
//    std::shared_ptr<OE_Light>          processLight(CSL_Element_ptr light_e);
//    //void processInteraction(OE_Interaction *interaction);
//    std::shared_ptr<OE_Mesh32>         processMesh(CSL_Element_ptr mesh_e);
//    //void processVertex(OE_Vertex *vertex);
//    void processUVMapData(OE_UVMapData &uvmap, const std::size_t&);
//	std::shared_ptr<OE_VertexGroup>		processVertexGroup(CSL_Element_ptr vGroup_e);
//    //void processAnimation(OE_Animation *animation);
//    //void processTransformation(OE_Transformation *transformation);
//    void processTriangle(OE_Mesh32 *mesh, const std::size_t& num_of_uvs);
//    //void processPhysics(OE_Physics *physics);
//    std::shared_ptr<OE_Texture>  processTexture(CSL_Element_ptr texture_e);
//    std::shared_ptr<OE_Material> processMaterial(CSL_Element_ptr material_e);
//    std::shared_ptr<OE_TCM>      processTCM(CSL_Element_ptr tcm_e);
////    void processTCM_Texture(OE_TCM_Texture &tcm_texture);
//    std::shared_ptr<OE_ViewportConfig> processViewportConfig(CSL_Element_ptr vconf_e);
//    
//};

#endif
