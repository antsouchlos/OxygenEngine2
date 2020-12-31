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



	world_ptr Interpreter::process_world(const element& world_e) {
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



	scene_ptr Interpreter::process_scene(const element& scene_e) {
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



	camera_ptr Interpreter::process_camera(const element& camera_e) {
		camera_ptr result = std::make_shared<oe::camera>();


		// Attributes
		

		// TODO: Is it smart to make this use string_view? Maybe a const char* would be better
		object_list_.append(std::string(camera_e.attributes.at("name")), result);

		result->visible = !!sv_to_int(camera_e.attributes.at("visible"));


		// Single Assignments
		

		result->aspect_ratio	= sv_to_float(camera_e.single_assignments.at("aspect_ratio"));	
		result->fov				= sv_to_float(camera_e.single_assignments.at("fov"));	
		result->near			= sv_to_float(camera_e.single_assignments.at("near"));	
		result->far				= sv_to_float(camera_e.single_assignments.at("far"));	
		// TODO: Make this use std::string_view
		auto parent = camera_e.single_assignments.at("parent");
		// TODO: Dependency
		result->parent			= object_list_.name2id(std::string(parent));
		// TODO: This was wrong in the previous interpreter
		result->parent_type		= sv_to_int(camera_e.single_assignments.at("parent_type"));	


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
	


	light_ptr Interpreter::process_light(const element& light_e) {
		light_ptr result = std::make_shared<oe::light>();


		// Attributes


		// TODO: Is it smart to make this use string_view? Maybe a const char* would be better
		// TODO: Actually set the name of the light
		object_list_.append(std::string(light_e.attributes.at("name")), result);

		result->visible = !!sv_to_int(light_e.attributes.at("visible"));


		// Single Assignments


		result->light_type		= sv_to_int(light_e.single_assignments.at("light_type"));
		result->range			= sv_to_float(light_e.single_assignments.at("range"));
		result->intensity		= sv_to_float(light_e.single_assignments.at("intensity"));
		result->fov				= sv_to_float(light_e.single_assignments.at("fov"));

		auto parent = light_e.single_assignments.at("parent");
		// TODO: Dependency
		result->parent			= object_list_.name2id(std::string(parent));
		result->parent_type		= sv_to_int(light_e.single_assignments.at("parent_type"));	


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

		auto color_v = light_e.list_assignments.at("color");
		if (color_v.size() != 3)
			throw semantic_error("The color member variable must have exactly 3 elements");

		result->color.r = sv_to_float(color_v[0]);
		result->color.g = sv_to_float(color_v[1]);
		result->color.b = sv_to_float(color_v[2]);

		for (const auto& o : light_e.list_assignments.at("objects")) {
			// TODO: Is emplace_back an option?
			// TODO: Dependency
			// TODO: Make this not use std::string
			result->objects.push_back(object_list_.name2id[std::string(o)]);
		}
	}



	mesh_ptr Interpreter::process_mesh(const element& mesh_e) {
		mesh_ptr result = std::make_shared<oe::mesh>();

	
		// Attributes


		// TODO: Uniform naming convetion
		result->data.isDynamic	= sv_to_int(mesh_e.attributes.at("isDynamic"));
		result->data.num_of_uvs	= sv_to_int(mesh_e.attributes.at("num_uvs"));
		result->visible			= !!sv_to_int(mesh_e.attributes.at("visible"));


		// Single Assignments


		auto num_of_vertices = sv_to_int(mesh_e.single_assignments.at("num_of_vertices"));
		result->data.vertices.positions.reserve(num_of_vertices);

		auto num_of_normals = sv_to_int(mesh_e.single_assignments.at("num_of_normals"));
		result->data.vertices.normals.reserve(num_of_normals);
		
		auto num_of_triangles = sv_to_int(mesh_e.single_assignments.at("num_of_triangles"));
		result->data.triangles.reserve(num_of_triangles);

		auto parent = mesh_e.single_assignments.at("parent");
		// TODO: Dependency
		result->parent			= object_list_.name2id(std::string(parent));
		result->parent_type		= sv_to_int(mesh_e.single_assignments.at("parent_type"));	


		// List Assignments


		auto cs_v = mesh_e.list_assignments.at("current_state");
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
	
		// TODO: Uniform naming convention
		for (const auto& t : mesh_e.list_assignments.at("textureCM_IDs")) {
			// TODO: std::string
			// TODO: WHY TF.
			result->textureCM_IDs.push_back(oe::world::tcmsList.name2id[std::string(t)]);
		}

		std::size_t n_vertices;
		for (const auto& v : mesh_e.list_assignments.at("vertices")) {
			result->data.vertices.positions.push_back(sv_to_float(v));
		}
	
		for (const auto& n : mesh_e.list_assignments.at("normals")) {
			result->data.vertices.normals.push_back(sv_to_float(n));
		}


		// Child Elements


		for (const auto& vgroup_e : mesh_e.elements.at("VertexGroup")) {
			vgroup_ptr v = process_vgroup(vgroup_e);
			// TODO: Make this a std::shared_ptr
			result->data.triangle_groups[v->id] = v.get();
		}

		for (const auto& triangle_e : mesh_e.elements.at("Triangle")) {
			oe::triangle t = process_triangle(result, triangle_e, result->data.num_of_uvs);
			result->data.triangles.push_back(t);
		}

		std::size_t num_of_uvmaps = 0;
		for (const auto& uvmap_data_e : mesh_e.elements.at("UVMapData")) {
			oe::uvmap_data u = process_uvmap_data(uvmap_data_e, num_of_uvmaps++);
			result->data.vertices.uvmaps.push_back(u);
		}


		// Perform operations depending on parsed values
		

		std::size_t max_uv_num = 0;
		for (const auto& n : result->data.vertices.uvmaps) {
			max_uv_num = std::max(max_uv_num, n.elements.size());
		}
	
		if ( (num_of_vertices<=65536) && (num_of_normals<=65536) && (max_uv_num<=65536)
				&& (num_of_triangles*3<=65536) && (num_of_uvmaps<=2) ) {

			// TODO: Make this use std::shared_ptr
			result->data.initUnorderedIB(result.get());
		} else if (num_of_uvmaps == 0) {
			// TODO: Make this use std::shared_ptr
			result->data.initUnorderedIB(result.get());
		} else {
			// TODO: Make this use std::shared_ptr
			result->data.initOrderedIB(result.get());
		}
		
		// TODO: Naming. N. A. M. I. N. G. (Why is there an s in the second function
		// and no s in the first one?)
		result->data.genVertexBufferInternally();
		result->data.genIndexBuffersInternally();


		return result;
	}



	vgroup_ptr Interpreter::process_vgroup(const element& vgroup_e) {
		vgroup_ptr result = nullptr;


		// Attributes


		// TODO: std::string
		auto name = vgroup_e.attributes.at("name");
		result = std::make_shared<oe::vgroup>(std::string(name));

		result->visible = !!sv_to_int(vgroup_e.attributes.at("visible"));


		// Single Assignments


		// TODO: WHY?
		result->bone_id = 0;

		// TODO: Is this an id or a name? Change the identifier in the file accordingly
		auto material_id = vgroup_e.single_assignments.at("material_id");
		// TODO: Dependency
		// TODO: std::string
		result->material_id = material_list_.name2id(std::string(material_id));


		// List Assignments


		for (const auto& p : vgroup_e.list_assignments.at("polygons")) {
			result->polygons.push_back(sv_to_int(p));
		}
	}



	texture_ptr Interpreter::process_texture(const element& texture_e) {
		texture_ptr result = std::make_shared<oe::texture>();
	

		// Single Assignments


		// TODO: std::string
		result->path = std::string(texture_e.attributes.at("path"));
		
		auto camera = texture_e.single_assignments.at("camera");
		// TODO: Dependency
		// TODO: std::string
		result->camera = object_list_.name2id(std::string(camera));
	}



	material_ptr Interpreter::process_material(const element& material_e) {
		material_ptr result = std::make_shared<oe::material>();


		// Attributes

		
		// TODO: Remove name from the csl format
		

		// Single Assignments


		// TODO: FOR THE LOVE OF GOD AND ALL THAT IS HOLY, AT LEAST MAKE THE USE OF
		// SINGLE- OR LIST ASSIGNMENTS UNIFORM. HOLY FUCK.
		
		result->alpha			= sv_to_float(material_e.single_assignments.at("alpha"));
		result->dif_r			= sv_to_float(material_e.single_assignments.at("dif_r"));
		result->dif_g			= sv_to_float(material_e.single_assignments.at("dif_g"));
		result->dif_b			= sv_to_float(material_e.single_assignments.at("dif_b"));
		result->dif_a			= sv_to_float(material_e.single_assignments.at("dif_a"));
		result->scol_r			= sv_to_float(material_e.single_assignments.at("scol_r"));
		result->scol_g			= sv_to_float(material_e.single_assignments.at("scol_g"));
		result->scol_b			= sv_to_float(material_e.single_assignments.at("scol_b"));
		result->illuminosity	= sv_to_float(material_e.single_assignments.at("illuminosity"));
		
		result->translucency
							= sv_to_float(material_e.single_assignments.at("translucency"));
		result->specular_intensity
							= sv_to_float(material_e.single_assignments.at("specular_intensity"));
		result->specular_hardness
							= sv_to_float(material_e.single_assignments.at("specular_hardness"));


		// List Assignments


		// TODO: Make naming uniform (textureCM_IDs -> tcms)
		for (const auto& t : material_e.list_assignments.at("textureCM_IDs")) {
			// TODO: Dependency
			// TODO: std::string
			result->textureCM_IDs.push_back(tcm_list_.name2id[std::string(t)]);
		}
	}



	tcm_ptr Interpreter::process_tcm(const element& tcm_e) {
		tcm_ptr result = std::make_shared<oe::tcm>();


		// Single Assignments


		// TODO: Make the code uniform: Either put every rgba thiny inside a list-assignment
		// or not
		result->r				= sv_to_float(tcm_e.single_assignments.at("r"));
		result->g				= sv_to_float(tcm_e.single_assignments.at("g"));
		result->b				= sv_to_float(tcm_e.single_assignments.at("b"));
		result->a				= sv_to_float(tcm_e.single_assignments.at("a"));
		result->combine_mode	= sv_to_int(tcm_e.single_assignments.at("combine_mode"));
		result->texture_array	= !!sv_to_int(tcm_e.single_assignments.at("texture_array"));


		// Child Elements
		

		for (const auto& tcm_texture_e : tcm_e.elements.at("TCM_Texture")) {
			// TODO: Is emplace_back an option?
			result->textures.push_back(process_tcm_texture(tcm_texture_e));
		}
	
	}



	vpconfig_ptr Interpreter::process_vpconfig(const element& vpconfig_e) {
		vpconfig_ptr result = std::make_shared<oe::vpconfig>();


		// List Assignments


		for (const auto& s : vpconfig_e.list_assignments.at("split_screen_positions")) {
			result->split_screen_positions.push_back(sv_to_float(s));
		}

		for (const auto& c : vpconfig_e.list_assignments.at("camera_modes")) {
			result->camera_modes.push_back(sv_to_int(c));
		}
		
		for (const auto& l : vpconfig_e.list_assignments.at("layer_combine_modes")) {
			result->layer_combine_modes.push_back(sv_to_int(l));
		}
		
		for (const auto& c : vpconfig_e.list_assignments.at("cameras")) {
			// TODO: Dependency
			// TODO: std::string
			result->cameras.push_back(object_list_.name2id[std::string(c)]);
		}	
	}



	oe::tcm_texture Interpreter::process_tcm_texture(const element& tcm_texture_e) {
		oe::tcm_texture result;


		// Single Assignments


		result.mode			= sv_to_int(tcm_texture_e.single_assignments.at("mode"));
		result.uvmap		= sv_to_int(tcm_texture_e.single_assignments.at("uvmap"));
		
		// TODO: Uniform naming convention
		result.textureMulFactor
							= sv_to_int(tcm_texture_e.single_assignments.at("textureMulFactor"));
		
		// TODO: Uniform naming convention
		auto texture_id		= tcm_texture_e.single_assignments.at("textureID");
		// TODO: Dependency
		// TODO: std::string
		result.textureID	= texture_list_.name2id(std::string(texture_id));


		return result;
	}



	oe::uvmap_data Interpreter::process_uvmap_data(const element& uvmap_data_e, std::size_t num) {
		// TODO: std::string
		oe::uvmap_data result(num, std::string(uvmap_data_e.attributes.at("name")));


		// Single Assignments


		// TODO: Make this an attribute
		std::size_t num_of_uvs = sv_to_int(uvmap_data_e.single_assignments.at("num_of_uvs"));
		result.elements.reserve(num_of_uvs);


		// List Assignments


		for (const auto& e : uvmap_data_e.list_assignments.at("elements")) {
			result.elements.push_back(sv_to_float(e));
		}


		return result;
	}

	oe::triangle Interpreter::process_triangle(mesh_ptr mesh, const element& triangle_e,
									std::size_t num_of_uvs) {

		oe::triangle result;

		
		// List Assignments
		
		// TODO: Proper naming
		std::size_t n = 2 + num_of_uvs;
		

		if (triangle_e.list_assignments.at("v1").size() != n)
			// TODO
			throw semantic_error("2 + num_of_uvs != triangle_e.list_assignments.at(\"v1\").size()");

		for (const auto& v : triangle_e.list_assignments.at("v1")) {
			// TODO: Smart pointers
			result.v1 = new uint32_t[n];
			for (std::size_t i = 0; i < n; i++) {
				result.v1[i] = sv_to_int(v);
			}

			//TODO: Smart pointers
			uint32_t* actual_indices = mesh->data.addTriangle(result.v1);
			if (actual_indices != result.v1) {
				delete[] result.v1;
				result.v1 = actual_indices;
			}
		}
		
		if (triangle_e.list_assignments.at("v2").size() != n)
			// TODO
			throw semantic_error("2 + num_of_uvs != triangle_e.list_assignments.at(\"v2\").size()");

		for (const auto& v : triangle_e.list_assignments.at("v2")) {
			// TODO: Smart pointers
			result.v2 = new uint32_t[n];
			for (std::size_t i = 0; i < n; i++) {
				result.v2[i] = sv_to_int(v);
			}

			//TODO: Smart pointers
			uint32_t* actual_indices = mesh->data.addTriangle(result.v2);
			if (actual_indices != result.v2) {
				delete[] result.v2;
				result.v2 = actual_indices;
			}
		}
		
		if (triangle_e.list_assignments.at("v3").size() != n)
			// TODO
			throw semantic_error("2 + num_of_uvs != triangle_e.list_assignments.at(\"v3\").size()");

		for (const auto& v : triangle_e.list_assignments.at("v3")) {
			// TODO: Smart pointers
			result.v3 = new uint32_t[n];
			for (std::size_t i = 0; i < n; i++) {
				result.v3[i] = sv_to_int(v);
			}

			//TODO: Smart pointers
			uint32_t* actual_indices = mesh->data.addTriangle(result.v3);
			if (actual_indices != result.v3) {
				delete[] result.v3;
				result.v3 = actual_indices;
			}
		}


		return result;
	}
}
