#include <Carbon/CSL_Interpreter.h>

// Philipp might kill me for this, but I really think it looks better,
// so it's a chance I'm willing to take
namespace oe {
	using world		= OE_World;
	using scene		= OE_Scene;
	using camera	= OE_Camera;
	using light		= OE_Light;
	using mesh		= OE_Mesh32;
	using vgroup	= OE_VertexGroup;
	using texture	= OE_Texture;
	using material	= OE_Material;
	using tcm		= OE_TCM;
	using vpconfig	= OE_ViewportConfig;
}

namespace csl {
	int Interpreter::sv_to_int(std::string_view sv) {
		int result = 0;

		std::from_chars(sv.begin(), sv.end(), result);

		return result;
	}

	float Interpreter::sv_to_float(std::string_view sv) {
		float result = 0;

		int temp1;
		auto [p1, ec1] = std::from_chars(sv.begin(), sv.end(), temp1);

		int temp2;
		auto [p2, ec2] = std::from_chars(p1+1, sv.end(), temp2);

		result = temp1 + temp2 / static_cast<float>(std::pow(10, (p2-p1 - 1)));

		return result;
	}


	world_ptr Interpreter::process_world(const csl::element& world_e) {
		world_ptr result = std::make_shared<oe::world>();


		// Child Elements


		for (const auto& scene_e : world_e.elements.at("Scene")) {
			scene_ptr scene = process_scene(scene_e);
			// TODO: Why not just load the whole scene here? Why only add the id to world?
			result->scenes.insert(scene->id);
		}

		for (const auto& vpconfig_e : world_e.elements.at("ViewportConfig")) {
			vpconfig_ptr vpconfig = process_vpconfig(vpconfig_e);
			result->viewports.insert(vpconfig->id);
		}
		
		
		// Single Assignments


		// TODO: Dependency
		// TODO: I am really not sold on this
  		auto loaded_viewport = world_e.single_assignments.at("loaded_viewport");
		auto loaded_scene = world_e.single_assignments.at("loaded_scene");

		// TODO: Make this use std::string_view
		result->loaded_viewport = viewport_list_.name2id[std::string(loaded_viewport)];
		result->loaded_scene = scene_list_.name2id[std::string(loaded_scene)];


		return result;
	}

	scene_ptr Interpreter::process_scene(const csl::element& scene_e) {
		scene_ptr result = std::make_shared<oe::scene>();

		
		// Attributes
		

		// TODO: Is it smart to make this use string_view? Maybe a const char* would be better
		scene_list_.append(std::string(scene_e.attributes.at("name")), result);


		// Child Elements
		

		for (const auto& camera_e : scene_e.elements.at("Camera")) {
			camera_ptr c = process_camera(camera_e);
			result->objects.insert(c->id);
		}

		for (const auto& light_e : scene_e.elements.at("Light")) {
			light_ptr l = process_light(light_e);
			result->objects.insert(l->id);
		}

		for (const auto& mesh_e : scene_e.elements.at("Mesh")) {
			mesh_ptr m = process_mesh(mesh_e);
			result->objects.insert(m->id);
		}

		for (const auto& texture_e : scene_e.elements.at("Texture")) {
			texture_ptr t = process_texture(texture_e);
			result->objects.insert(t->id);
		}

		for (const auto& material_e : scene_e.elements.at("Material")) {
			material_ptr m = process_material(material_e);
			result->objects.insert(m->id);
		}

		for (const auto& tcm_e : scene_e.elements.at("TextureCombineMode")) {
			tcm_ptr t = process_tcm(tcm_e);
			result->objects.insert(t->id);
		
		}


		// TODO: It is utterly stupid to return the whole scene_ptr if the only thing used of it
		// ist the id
		return result;	
	}

	camera_ptr Interpreter::process_camera(const csl::element& camera_e) {
		camera_ptr result = std::make_shared<oe::camera>();


		// Attributes
		

		// TODO: Is it smart to make this use string_view? Maybe a const char* would be better
		object_list_.append(std::string(camera_e.attributes.at("name")), result);

		result->visible = !!sv_to_int(camera_e.attributes.at("visible"));

		// Assignments
		

		// TODO: WHY TF ARE ALL OF THESE PROTECTED AND NOT PUBLIC M8?
//		result->aspect_ratio	= sv_to_float(camera_e.single_assignments.at("aspect_ratio"));	
//		result->fov				= sv_to_float(camera_e.single_assignments.at("fov"));	
//		result->near			= sv_to_float(camera_e.single_assignments.at("near"));	
//		result->far				= sv_to_float(camera_e.single_assignments.at("far"));	
//		// TODO: Make this use std::string_view
//		auto parent = camera_e.single_assignments.at("parent");
//		result->parent			= object_list_.name2id(std::string(parent));
//		// TODO: This waw wrong in the previous interpreter
//		result->parent_type		= sv_to_int(camera_e.single_assignments.at("parent_type"));	


		// List Assignments


		// TODO: In order to make functions more uniform, change this and split it into
		// single assignments
		auto cs_v = camera_e.list_assignments.at("current_state");
		if (cs_v.size() != 10)
			throw semantic_error("The current_state member variable must have exactly"
								 "10 elements");

		result->current_state.pos_x = sv_to_float(cs_v[0]);
		result->current_state.pos_y = sv_to_float(cs_v[1]);
		result->current_state.pos_z = sv_to_float(cs_v[2]);
		result->current_state.rot_w = sv_to_float(cs_v[3]);
		result->current_state.rot_x = sv_to_float(cs_v[4]);
		result->current_state.rot_y = sv_to_float(cs_v[5]);
		result->current_state.rot_z = sv_to_float(cs_v[6]);
		result->current_state.sca_x = sv_to_float(cs_v[7]);
		result->current_state.sca_y = sv_to_float(cs_v[8]);
		result->current_state.sca_z = sv_to_float(cs_v[9]);
	}

	light_ptr Interpreter::process_light(const csl::element& light_e) {
		light_ptr result = std::make_shared<oe::light>();

	}

	mesh_ptr Interpreter::process_mesh(const csl::element& mesh_e) {
		mesh_ptr result = std::make_shared<oe::mesh>();
	
	}

	vgroup_ptr Interpreter::process_vgroup(const csl::element& vgroup_e) {
		vgroup_ptr result = std::make_shared<oe::vgroup>();
	
	}

	texture_ptr Interpreter::process_texture(const csl::element& texture_e) {
		texture_ptr result = std::make_shared<oe::texture>();
	
	}

	material_ptr Interpreter::process_material(const csl::element& material_e) {
		material_ptr result = std::make_shared<oe::material>();
	
	}

	tcm_ptr Interpreter::process_tcm(const csl::element& tcm_e) {
		tcm_ptr result = std::make_shared<oe::tcm>();
	
	}

	vpconfig_ptr Interpreter::process_vpconfig(const csl::element& vpconfig_e) {
		vpconfig_ptr result = std::make_shared<oe::vpconfig>();
	
	}
}
