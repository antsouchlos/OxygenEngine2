#ifndef CSL_INTERPRETER_H_
#define CSL_INTERPRETER_H_

#include <types/OE_World.h>

#include <Carbon/CSL_Exceptions.h>
#include <Carbon/CSL_Parser.h>

#include <charconv>
#include <memory>
#include <string>
#include <string_view>


namespace csl {
	using world_ptr		= std::shared_ptr<OE_World>;
	using scene_ptr		= std::shared_ptr<OE_Scene>;
	using camera_ptr	= std::shared_ptr<OE_Camera>;
	using light_ptr		= std::shared_ptr<OE_Light>;
	using mesh_ptr		= std::shared_ptr<OE_Mesh32>;
	using vgroup_ptr	= std::shared_ptr<OE_VertexGroup>;
	using texture_ptr	= std::shared_ptr<OE_Texture>;
	using material_ptr	= std::shared_ptr<OE_Material>;
	using tcm_ptr		= std::shared_ptr<OE_TCM>;
	using vpconfig_ptr	= std::shared_ptr<OE_ViewportConfig>;


	class Interpreter {
		public:
			static int		sv_to_int(std::string_view sv);
			static float	sv_to_float(std::string_view sv);


			std::shared_ptr<OE_World> interpret(std::string& input);
			std::shared_ptr<OE_World> interpret_file(std::string& path_to_file);

		private:
			OE_SharedIndexMap<OE_Scene>          scene_list_;
			OE_SharedIndexMap<OE_Object>         object_list_;
			OE_SharedIndexMap<OE_Material>       material_lList_;
			OE_SharedIndexMap<OE_Texture>        texture_list_;
			OE_SharedIndexMap<OE_TCM>            tcms_list_;
			OE_SharedIndexMap<OE_ViewportConfig> viewport_list_;

			world_ptr		process_world(const csl::element&);
			scene_ptr		process_scene(const csl::element&);
			camera_ptr		process_camera(const csl::element&);
			light_ptr		process_light(const csl::element&);
			mesh_ptr		process_mesh(const csl::element&);
			vgroup_ptr		process_vgroup(const csl::element&);
			texture_ptr		process_texture(const csl::element&);
			material_ptr	process_material(const csl::element&);
			tcm_ptr			process_tcm(const csl::element&);
			vpconfig_ptr	process_vpconfig(const csl::element&);
	};
}

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
