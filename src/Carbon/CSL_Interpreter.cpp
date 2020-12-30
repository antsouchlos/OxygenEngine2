#include <Carbon/CSL_Interpreter.h>


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

		// TODO: Dependency
		result->loaded_viewport = viewport_list_.name2id[std::string(loaded_viewport)];
		result->loaded_scene = scene_list_.name2id[std::string(loaded_scene)];


		return result;
	}

	scene_ptr Interpreter::process_scene(const csl::element& scene_e) {
		scene_ptr result = std::make_shared<oe::scene>();

		
		// Attributes
		

		// TODO: Is it smart to make this use string_view? Maybe a const char* would be better
		// TODO: Actually set the name of the scene
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
		// TODO: Actually set the name of the camera
		object_list_.append(std::string(camera_e.attributes.at("name")), result);

		result->visible = !!sv_to_int(camera_e.attributes.at("visible"));


		// Single Assignments
		

		// TODO: WHY TF ARE ALL OF THESE PROTECTED AND NOT PUBLIC M8?
//		result->aspect_ratio	= sv_to_float(camera_e.single_assignments.at("aspect_ratio"));	
//		result->fov				= sv_to_float(camera_e.single_assignments.at("fov"));	
//		result->near			= sv_to_float(camera_e.single_assignments.at("near"));	
//		result->far				= sv_to_float(camera_e.single_assignments.at("far"));	
//		// TODO: Make this use std::string_view
//		auto parent = camera_e.single_assignments.at("parent");
//		// TODO: Dependency
//		result->parent			= object_list_.name2id(std::string(parent));
//		// TODO: This waw wrong in the previous interpreter
//		result->parent_type		= sv_to_int(camera_e.single_assignments.at("parent_type"));	


		// List Assignments


		// TODO: Make functions more readable: make a function for current_state
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


		// Attributes


		// TODO: Is it smart to make this use string_view? Maybe a const char* would be better
		// TODO: Actually set the name of the light
		object_list_.append(std::string(light_e.attributes.at("name")), result);

		result->visible = !!sv_to_int(light_e.attributes.at("visible"));


		// Single Assignments


		// TODO: WHY TF ARE ALL OF THESE PROTECTED AND NOT PUBLIC M8?
//		result->light_type		= sv_to_int(light_e.single_assignments.at("light_type"));
//		result->range			= sv_to_float(light_e.single_assignments.at("range"));
//		result->intensity		= sv_to_float(light_e.single_assignments.at("intensity"));
//		result->fov				= sv_to_float(light_e.single_assignments.at("fov"));
//
//		auto parent = light_e.single_assignments.at("parent");
//		// TODO: Dependency
//		result->parent			= object_list_.name2id(std::string(parent));
//		result->parent_type		= sv_to_int(light_e.single_assignments.at("parent_type"));	


		// List Assignments


		auto cs_v = light_e.list_assignments.at("current_state");
		if (cs_v.size() != 10)
			throw semantic_error("The current_state member variable must have exactly"
								 "10 elements");
		
		// TODO: Make functions more readable: make a function for current_state
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

		// TODO: WHY TF ARE ALL OF THESE PROTECTED AND NOT PUBLIC M8?
//		auto color_v = light_e.list_assignments.at("color");
//		if (color_v.size() != 3)
//			throw semantic_error("The color member variable must have exactly 3 elements");
//
//		result->color.r = sv_to_float(color_v[0]);
//		result->color.g = sv_to_float(color_v[1]);
//		result->color.b = sv_to_float(color_v[2]);
//
//		for (const auto& o : light_e.list_assignments.at("objects")) {
//			// TODO: Is emplace_back an option?
//			// TODO: Dependency
//			// TODO: Make this not use std::string
//			result->objects.push_back(object_list_.name2id[std::string(o)]);
//		}
	}

	mesh_ptr Interpreter::process_mesh(const csl::element& mesh_e) {
		mesh_ptr result = std::make_shared<oe::mesh>();
	
	}

	vgroup_ptr Interpreter::process_vgroup(const csl::element& vgroup_e) {
		vgroup_ptr result = std::make_shared<oe::vgroup>();


		// Attributes


		// TODO: Set vgroup name
		// result->name = vgroup_e.attributes.at("name");

		// TODO: WHY TF ARE ALL OF THESE PROTECTED AND NOT PUBLIC M8?
//		result->visible = !!sv_to_int(vgroup_e.attributes.at("visible"));
//
//
//		// Single Assignments
//
//
//		// TODO: WHY?
//		result->bone_id = 0;
//
//		// TODO: Is this an id or a name? Change the identifier in the file accordingly
//		auto material_id = vgroup_e.single_assignments.at("material_id");
//		// TODO: Dependency
//		// TODO: std::string
//		result->material_id = material_list_.name2id(std::string(material_id));
//
//
//		// List Assignments
//
//
//		for (const auto& p : vgroup_e.list_assignments.at("polygons")) {
//			result->polygons.push_back(sv_to_int(p));
//		}
	}

	texture_ptr Interpreter::process_texture(const csl::element& texture_e) {
		texture_ptr result = std::make_shared<oe::texture>();
	

		// Attributes


		// TODO: Set vgroup name
		// result->name = vgroup_e.attributes.at("name");
		

		// Single Assignments


		// TODO: WHY TF ARE ALL OF THESE PROTECTED AND NOT PUBLIC M8?
//		// TODO: std::string
//		result->path = std::string(texture_e.attributes.at("path"));
//		
//		auto camera = texture_e.single_assignments.at("camera");
//		// TODO: Dependency
//		// TODO: std::string
//		result->camera = object_list_.name2id(std::string(camera));
	}

	material_ptr Interpreter::process_material(const csl::element& material_e) {
		material_ptr result = std::make_shared<oe::material>();


		// Attributes

		
		// TODO: Remove name from the csl format
		

		// Single Assignments


		// TODO: FOR THE LOVE OF GOD AND ALL THAT IS HOLY, AT LEAST MAKE THE USE OF
		// SINGLE- OR LIST ASSIGNMENTS UNIFORM. HOLY FUCK.
		
		// TODO: WHY TF ARE ALL OF THESE PROTECTED AND NOT PUBLIC M8?
//		result->alpha			= sv_to_float(material_e.single_assignments.at("alpha"));
//		result->dif_r			= sv_to_float(material_e.single_assignments.at("dif_r"));
//		result->dif_g			= sv_to_float(material_e.single_assignments.at("dif_g"));
//		result->dif_b			= sv_to_float(material_e.single_assignments.at("dif_b"));
//		result->dif_a			= sv_to_float(material_e.single_assignments.at("dif_a"));
//		result->scol_r			= sv_to_float(material_e.single_assignments.at("scol_r"));
//		result->scol_g			= sv_to_float(material_e.single_assignments.at("scol_g"));
//		result->scol_b			= sv_to_float(material_e.single_assignments.at("scol_b"));
//		result->illuminosity	= sv_to_float(material_e.single_assignments.at("illuminosity"));
//		
//		result->translucency
//							= sv_to_float(material_e.single_assignments.at("translucency"));
//		result->specular_intensity
//							= sv_to_float(material_e.single_assignments.at("specular_intensity"));
//		result->specular_hardness
//							= sv_to_float(material_e.single_assignments.at("specular_hardness"));
//
//
//		// List Assignments
//
//
//		// TODO: Make naming uniform (textureCM_IDs -> tcms)
//		for (const auto& t : material_e.list_assignments.at("textureCM_IDs")) {
//			// TODO: Dependency
//			// TODO: std::string
//			result->textureCM_IDs.push_back(tcm_list_.name2id[std::string(t)]);
//		}
	}

	tcm_ptr Interpreter::process_tcm(const csl::element& tcm_e) {
		tcm_ptr result = std::make_shared<oe::tcm>();
	
	}

	vpconfig_ptr Interpreter::process_vpconfig(const csl::element& vpconfig_e) {
		vpconfig_ptr result = std::make_shared<oe::vpconfig>();
	
	}
}
