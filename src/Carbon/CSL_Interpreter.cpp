#include <Carbon/CSL_Interpreter.h>


using namespace std;


// This is a helper function for the interpreter
void OE_ReverseBitset(std::bitset<64>& b){
    std::bitset<64> temp;

    for (size_t i=0;i < 32; i++){
        temp[i] = b[31-i];
    }

    b = temp;
}

std::shared_ptr<OE_World> CSL_Interpreter::interpret(string sourceCode) {
    CSL_Parser parser(sourceCode);
    
    auto t = clock();
    
    CSL_Element_ptr rootElement = parser.parse();

    auto t1 = clock();
    cout << "CSL TEST PARSE: " << (float)(t1 -t)/CLOCKS_PER_SEC << endl;
  
	// TODO	
//	auto world = processWorld(std::move(rootElement));
	auto world = processWorld(std::move(rootElement));

    cout << "CSL TEST INTERPRET: " << (float)(clock()-t1)/CLOCKS_PER_SEC << endl;
    
//    cout << scenesList.to_str() << endl;
//    cout << objectsList.to_str() << endl;
//    cout << materialsList.to_str() << endl;
//    cout << tcmsList.to_str() << endl;
//    cout << texturesList.to_str() << endl;
//    cout << viewportsList.to_str() << endl;

    return world;
}

std::shared_ptr<OE_World> CSL_Interpreter::interpretFile(string pathToFile) {
	auto t = clock();


    string sourceCode = "";
	ifstream in(pathToFile);

	string line;
	while (getline(in, line))
		sourceCode += line;

	in.close();

    cout << "CSL TEST READ FILE: " << (float)(clock() -t)/CLOCKS_PER_SEC << endl;


	return interpret(sourceCode);
}

std::shared_ptr<OE_World> CSL_Interpreter::processWorld(CSL_Element_ptr world_e) {
	std::shared_ptr<OE_World> world = std::make_shared<OE_World>();

	if (world_e->name != "World"sv) {
		// TODO
		//throw InterpreterException("Element root must be \"World\"");
	}

	for (auto &e : world_e->elements) {
		if (e->name == "Scene"sv) {
//			auto scene = processScene();
//			world->scenes.insert(scene->id);
		} else if (e->name == "ViewportConfig"sv) {
//			auto vconf = processViewportConfig();
//			world->viewports.insert(vconf->id);
		} else {
			// TODO
			// throw CSL_UnknownElementException("");
		}
	}

	// TODO: Are attributes even necessary?
	for (auto &a : world_e->attributes) {
	
	}

	for (auto &a : world_e->assignments) {
	
	}

/*	for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;
        
        CSL_Node *saveNode = curNode;
        curNode = child;
        if (type == "tag") {
            if (id == "Scene") {
                auto scene = processScene();
                world->scenes.insert(scene->id);
            } 
            else if (id == "ViewportConfig") {
                auto vconf = processViewportConfig();
                world->viewports.insert(vconf->id);
            }
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"" + id + "\" is not an accepted tag-ID in \"World\"");
            }
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
             if (id == "loaded_viewport") {
                world->loaded_viewport = this->viewportsList.name2id[child->args[0]]; 
            } 
            else if (id == "loaded_scene") {
                world->loaded_scene = this->scenesList.name2id[child->args[0]];
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"World\"");
            }

        } 
        else if (type == "listassignment") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"World\"");
        } 
        else if (type == "tagassignment") {
                        throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variables in \"World\"");
        }
        curNode = saveNode;
    }*/
    return world;
}
/*
std::shared_ptr<OE_Scene> CSL_Interpreter::processScene() {
    std::shared_ptr<OE_Scene> scene = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        curNode = child;
        if (type == "tag") {
            assert (scene != nullptr);
            if (id == "Camera") {
                auto camera = processCamera();
                scene->objects.insert(camera->id);
            } 
            else if (id == "Light") {
                auto light = processLight();
                scene->objects.insert(light->id);
            } 
            else if (id == "Mesh") {
                auto mesh = processMesh();
                scene->objects.insert(mesh->id);
            } 
            else if (id == "Texture") {
                auto texture = processTexture();
                scene->textures.insert(texture->id);
            } 
            else if (id == "Material") {
                auto material = processMaterial();
                scene->materials.insert(material->id);
            } 
            else if (id == "TextureCombineMode") {
                auto tcm = processTCM();
                scene->texture_CMs.insert(tcm->id);
            } 
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"" + id + "\" is not an accepted tag-ID in \"Scene\"");
            }
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
            assert (scene != nullptr);
            throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Scene\"");
        } 
        else if (type == "listassignment") {
            assert (scene != nullptr);
            throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Scene\"");
        }
        else if (type == "tagassignment") {
            if (id == "name") {
                scene = std::make_shared<OE_Scene>(child->args[0]);
                this->scenesList.append(child->args[0], scene);
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID\"" + id + "\" in \"Scene\"");
            }
        }
        curNode = saveNode;
    }
    return scene;
}

std::shared_ptr<OE_Camera> CSL_Interpreter::processCamera() {
    
    std::shared_ptr<OE_Camera> camera = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        curNode = child;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Camera\"");
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
            assert (camera != nullptr);
            if (id == "aspect_ratio") {
                camera->aspect_ratio = stof(child->args[0]);
            } 
            else if (id == "fov") {
                camera->fov = stof(child->args[0]);
            } 
            else if (id == "near") {
                camera->near = stof(child->args[0]);
            } 
            else if (id == "far") {
                camera->far = stof(child->args[0]);
            }
            else if (id == "parent") {
                camera->parent = this->objectsList.name2id[child->args[0]];
            }
            else if (id == "parent_type") {
                camera->far = stoi(child->args[0]);
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Camera\"");
            }
        } 
        else if (type == "listassignment") {
            assert (camera != nullptr);
            if (id == "current_state") {
                assert (child->args.size() == 10);
                camera->current_state.pos_x = stof(child->args[0]);
                camera->current_state.pos_y = stof(child->args[1]);
                camera->current_state.pos_z = stof(child->args[2]);
                camera->current_state.rot_w = stof(child->args[3]);
                camera->current_state.rot_x = stof(child->args[4]);
                camera->current_state.rot_y = stof(child->args[5]);
                camera->current_state.rot_z = stof(child->args[6]);
                camera->current_state.sca_x = stof(child->args[7]);
                camera->current_state.sca_y = stof(child->args[8]);
                camera->current_state.sca_z = stof(child->args[9]);
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Camera\"");
            }
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                camera = std::make_shared<OE_Camera>(child->args[0]);
                this->objectsList.append(child->args[0], camera);
            }
            else if (id == "visible"){
                assert(camera != nullptr);
                camera->visible = !!stoi(child->args[0]);
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID\"" + id + "\" in \"Camera\"");
            }
        }
        curNode = saveNode;
    }
    return camera;
}

std::shared_ptr<OE_Light> CSL_Interpreter::processLight() {
    std::shared_ptr<OE_Light> light = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        curNode = child;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Light\"");
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
            assert (light != nullptr);
            if (id == "light_type") {
                light->light_type = stoi(child->args[0]);
            } 
            else if (id == "range") {
                light->range = stof(child->args[0]);
            }
            else if (id == "intensity") {
                light->range = stof(child->args[0]);
            }
            else if (id == "fov") {
                light->fov = stof(child->args[0]);
            }
            else if (id == "parent") {
                light->parent = this->objectsList.name2id[child->args[0]];
            }
            else if (id == "parent_type") {
                light->parent_type = stoi(child->args[0]);
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Light\"");
            }
        }
        else if (type == "listassignment") {
            assert (light != nullptr);
            if (id == "current_state") {
                assert (child->args.size() == 10);
                light->current_state.pos_x = stof(child->args[0]);
                light->current_state.pos_y = stof(child->args[1]);
                light->current_state.pos_z = stof(child->args[2]);
                light->current_state.rot_w = stof(child->args[3]);
                light->current_state.rot_x = stof(child->args[4]);
                light->current_state.rot_y = stof(child->args[5]);
                light->current_state.rot_z = stof(child->args[6]);
                light->current_state.sca_x = stof(child->args[7]);
                light->current_state.sca_y = stof(child->args[8]);
                light->current_state.sca_z = stof(child->args[9]);
            } 
            else if (id == "color"){
                light->color.r = stof(child->args[0]);
                light->color.g = stof(child->args[1]);
                light->color.b = stof(child->args[2]);
            }
            else if (id == "objects"){
                for (auto& x: child->args){
                    light->objects.push_back(this->objectsList.name2id[x]);
                } 
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Light\"");
            }
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                light = std::make_shared<OE_Light>(child->args[0]);
                this->objectsList.append(child->args[0], light);
            }
            else if (id == "visible"){
                assert(light != nullptr);
                light->visible = !!stoi(child->args[0]);
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID\"" + id + "\" in \"Light\"");
            }
        }
        curNode = saveNode;
    }
    return light;
}*/

std::shared_ptr<OE_Mesh32> CSL_Interpreter::processMesh(CSL_Element_ptr mesh_e) {
	// TODO: Like seriously, WTF
    bool vertices_reserved = false; 
    bool normals_reserved = false;
    bool triangles_reserved = false;
    
    bool uvs_set = false;    //true if num_of_uvs is set
    bool map_chosen = false;
    //bool uvs_done = false;  // true if all OE_UVMapDatas have being created and parsed
    
    size_t vnum = 0;
    size_t nnum = 0;
    size_t num_of_uvmaps = 0;
    size_t num_of_triangles = 0;


	std::shared_ptr<OE_Mesh32> mesh = nullptr;

	for (auto &att : mesh_e->attributes) {
		if ( std::holds_alternative<CSL_Assignment_ptr>(att) ) {
			auto att_ptr = std::get<CSL_Assignment_ptr>(std::move(att));

			if (att_ptr->name == "name"sv) {
				// TODO: Make this use std::string_view
				mesh = std::make_shared<OE_Mesh32>(std::string(att_ptr->value));
				this->objectsList.append(std::string(att_ptr->value), mesh);
			} else if (att_ptr->name == "visible"sv) {
				mesh->visible = !!csl::parseInt(att_ptr->value);
			} else if (att_ptr->name == "isDynamic"sv) {
				mesh->data.isDynamic = !!csl::parseInt(att_ptr->value);
			} else if (att_ptr->name == "num_uvs"sv) {
				mesh->data.num_of_uvs = csl::parseInt(att_ptr->value);
			} else {
				throw UnknownMemberAttributeError(att_ptr->name);
			}
		} else {
			// TODO: Make this error messages the same everywhere
			throw SemanticError("The Mesh element does not have list attributes");
		}
	}

	for (auto &as : mesh_e->assignments) {
		assert(mesh != nullptr); // TODO

		if ( std::holds_alternative<CSL_Assignment_ptr>(as) ) {
			auto as_ptr = std::get<CSL_Assignment_ptr>(std::move(as));

			if (as_ptr->name == "parent"sv) {
				// TODO: Make this use std::string_view
				mesh->parent = this->objectsList.name2id[std::string(as_ptr->value)];
			} else if (as_ptr->name == "parent_type"sv) {
				mesh->parent_type = csl::parseInt(as_ptr->value);
			} else if (as_ptr->name == "num_of_vertices"sv) {
				// TODO: Jesus christ, is this really necessary? I reaaaaaly don't like having
				// such dependencies between data
				if (!vertices_reserved) {
					vnum = csl::parseInt(as_ptr->value);
					mesh->data.vertices.positions.reserve(vnum);
				}

				vertices_reserved = true;
			} else if (as_ptr->name == "num_of_normals"sv) {
				if (!normals_reserved) {
					nnum = csl::parseInt(as_ptr->value);
					mesh->data.vertices.normals.reserve(nnum);
				}

				normals_reserved = true;
			} else if (as_ptr->name == "num_of_triangles"sv) {
				if (!triangles_reserved) {
					// TODO: Make the naming consistent
					num_of_triangles = csl::parseInt(as_ptr->value);
					mesh->data.triangles.reserve(num_of_triangles);
				}

				triangles_reserved = true;
			} else {
				throw UnknownMemberVariableError(as_ptr->name);
			}
		} else {
			auto as_ptr = std::get<CSL_ListAssignment_ptr>(std::move(as));

			if (as_ptr->name == "current_state"sv) {
				// TODO
				// THE FUCK? WHY? DON'T USE LISTS FOR THIS, IT INTRODUCES A BUNCH OF PROBLEMS,
				// NOT THE LEAST OF WHICH BEING WHAT IF IT DOESN'T HAVE ENOUGH ELEMENTS?
                assert (as_ptr->values.size() == 10);
                mesh->current_state.pos_x = as_ptr->values[0];
                mesh->current_state.pos_y = as_ptr->values[1];
                mesh->current_state.pos_z = as_ptr->values[2];
                mesh->current_state.rot_w = as_ptr->values[3];
                mesh->current_state.rot_x = as_ptr->values[4];
                mesh->current_state.rot_y = as_ptr->values[5];
                mesh->current_state.rot_z = as_ptr->values[6];
                mesh->current_state.sca_x = as_ptr->values[7];
                mesh->current_state.sca_y = as_ptr->values[8];
                mesh->current_state.sca_z = as_ptr->values[9];
			} else if (as_ptr->name == "textureCM_IDs"sv) {
				for (const auto &v : as_ptr->values) {
					// TODO: Jesus fuck, is it that difficult to be consistend in how things
					// are defined?
					// TODO: Fuck, is this a list of strings?
					//mesh->textureCM_IDs.push_back(OE_World::tcmsList.name2id[v]);
				}
			} else if (as_ptr->name == "vertices"sv) {
				for (const auto &v : as_ptr->values) {
					mesh->data.vertices.positions.push_back(v);
				}
			} else if (as_ptr->name == "normals"sv) {
				normals_reserved = true; // TODO: Do this properly

				for (const auto &v : as_ptr->values) {
					mesh->data.vertices.normals.push_back(v);
				}
			} else {
				throw UnknownMemberVariableError(as_ptr->name);
			}
		}
	}

	for (auto &e : mesh_e->elements) {
		assert(mesh != nullptr); // TODO (Also check for all below functions)

		if (e->name == "VertexGroup"sv) {
			// TODO: Is using emplace_back instead of reserving data and then accessing the
			// data directly through indexes an option? (To not lose performance while somehow
			// keeping this clusterfuck organized)
			std::shared_ptr<OE_VertexGroup> vGroup = processVertexGroup(std::move(e));
			// TODO: Make mesh->data.triangle_groups[vGroup->id] be a std::shared_ptr
			mesh->data.triangle_groups[vGroup->id] = vGroup.get();
		} else if (e->name == "UVMapData"sv) {
			// TODO: Why is this an argument to a function and not the return value?
			OE_UVMapData uvMap;
			// TODO
			//processUVMapData(uvMap, num_of_uvmaps);
			mesh->data.vertices.uvmaps.push_back(uvMap);
			num_of_uvmaps++;
		} else if (e->name == "Triangle"sv) {
			triangles_reserved = true;
			assert(uvs_set == true);
			assert(map_chosen == true);
			processTriangle(mesh.get(), mesh->data.num_of_uvs);
		} else {
			throw UnknownMemberElementError(e->name);
		}
	}

	// TODO: Can this be left here? It used to be called for every CSL_Node child
	if ( !map_chosen && uvs_set && vertices_reserved && normals_reserved &&
			(num_of_uvmaps == mesh->data.num_of_uvs) && triangles_reserved) {

		std::size_t uvNum = 0;
		for (const auto &u : mesh->data.vertices.uvmaps) {
			uvNum = std::max(uvNum, u.elements.size());
		}

		std::size_t tNum = num_of_triangles; // TODO: This is really not necessary

            if (	(vnum <= 65536) && (nnum <= 65536)
				&& (uvNum <= 65536) && (tNum*3 <= 65536)
				&& (num_of_uvmaps <= 2) ){

                mesh->data.initUnorderedIB(mesh.get());
            } else if (num_of_uvmaps == 0) {
                mesh->data.initUnorderedIB(mesh.get());
            } else {
                mesh->data.initOrderedIB(mesh.get());
            }
            
            map_chosen = true;
	}

	mesh->data.genVertexBufferInternally();
	mesh->data.genIndexBuffersInternally();
	return mesh;
}

// TODO
//void CSL_Interpreter::processUVMapData(OE_UVMapData &uvmap, const size_t& uvmapID) {
//
//    bool uvs_set = false;
//    bool hasInit = false;
//    
//    for (auto& child : curNode->children) {
//        string type = child->type;
//        string id = child->ID;
//
//        CSL_Node *saveNode = curNode;
//        if (type == "tag") {
//            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"UVMapData\"");
//        } 
//        else if (type == "closedtag") {
//            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
//        } 
//        else if (type == "assignment") {
//            assert(hasInit == true);
//            if (id == "num_of_uvs") {
//                if (!uvs_set){
//                    uvmap.elements.reserve(stoi(child->args[0]));
//                    uvs_set = true;
//                }
//            } 
//            else {
//                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"UVMapData\"");
//            }
//        } 
//        else if (type == "listassignment") {
//            assert(hasInit == true);
//            if (id == "elements") {
//                uvs_set = true;
//                for (const auto& x: child->args){
//                    uvmap.elements.push_back(stof(x));
//                } 
//            } 
//            else {
//                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"UVMapData\"");
//            }
//        } 
//        else if (type == "tagassignment") {
//            if (id == "name") {
//                uvmap = OE_UVMapData(uvmapID, child->args[0]);
//                hasInit = true;
//            } 
//            else {
//                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"UVMapData\"");
//            }
//        }
//        curNode = saveNode;
//    }
//}

std::shared_ptr<OE_VertexGroup> CSL_Interpreter::processVertexGroup(CSL_Element_ptr vGroup_e) {
	std::shared_ptr<OE_VertexGroup> vGroup = nullptr;

	for (auto &att : vGroup_e->attributes) {
		if ( std::holds_alternative<CSL_Assignment_ptr>(att) ) {
			auto att_ptr = std::get<CSL_Assignment_ptr>(std::move(att));

			if (att_ptr->name == "name"sv) {
				// TODO: Make this use std::string_view
				vGroup = std::make_shared<OE_VertexGroup>(std::string(att_ptr->value));
			} else if (att_ptr->name == "visible") {
				assert(vGroup != nullptr); // TODO
				vGroup->visible = !!csl::parseInt(att_ptr->value);
			} else {
				throw UnknownMemberAttributeError(att_ptr->name);
			}
		} else {
			throw SemanticError("The name attribute of the VertexGroup element is not a list");
		}
	}

	for (auto &as : vGroup_e->assignments) {
		assert(vGroup != nullptr); // TODO

		if ( std::holds_alternative<CSL_Assignment_ptr>(as) ) {
			auto as_ptr = std::get<CSL_Assignment_ptr>(std::move(as));

			if (as_ptr->name == "material_id"sv) {
				// TODO: Make this use std::string_view
				vGroup->material_id = this->materialsList.name2id[std::string(as_ptr->value)];
			} else if (as_ptr->name == "bone_id") {
				// TODO: Is this really correct?
				vGroup->bone_id = 0;
			} else {
				throw UnknownMemberVariableError(as_ptr->name); // TODO: Differentiate between
																// List and non-List member
																// variables
			}
		} else {
			auto as_ptr = std::get<CSL_ListAssignment_ptr>(std::move(as));

			if (as_ptr->name == "polygons"sv) {
				for (const auto &v : as_ptr->values) {
					vGroup->polygons.push_back(v); // TODO: Save int types in int, not float
				}
			} else {
				throw UnknownMemberVariableError(as_ptr->name);
			}
		}
	}

	if (vGroup_e->elements.size() > 0) {
		throw SemanticError("The VertexGroup element does not have child elements");
	}

	return vGroup;
}


// TODO
//void CSL_Interpreter::processTriangle(OE_Mesh32 *mesh, const size_t &num_of_uvs) {
//    
//    OE_Triangle32 triangle = OE_Triangle32();
//    for (auto& child : curNode->children) {
//        string type = child->type;
//        string id = child->ID;
//
//        CSL_Node *saveNode = curNode;
//        if (type == "tag") {
//            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Triangle\"");
//        } 
//        else if (type == "closedtag") {
//            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
//        } 
//        else if (type == "assignment") {
//            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variables in \"Triangle\"");
//        } 
//        else if (type == "listassignment") {
//            if (id == "v1") {
//                
//                //IMPORTANT
//                assert(child->args.size() == 2 + num_of_uvs);
//                
//                triangle.v1 = new uint32_t[2+num_of_uvs];
//                for (size_t i=0; i<2 + num_of_uvs; i++){
//                    triangle.v1[i] = stoi(child->args[i]);
//                }
//                //check if triangle indices already exists to save space
//                uint32_t* actual_indices = mesh->data.addTriangle(triangle.v1);
//                if(actual_indices != triangle.v1){
//                    delete[] triangle.v1;
//                    triangle.v1 = actual_indices;
//                }
//            }
//            else if (id == "v2") {
//                
//                //IMPORTANT
//                assert(child->args.size() == 2 + num_of_uvs);
//                
//                triangle.v2 = new uint32_t[2+num_of_uvs];
//                for (size_t i=0; i<2 + num_of_uvs; i++){
//                    triangle.v2[i] = stoi(child->args[i]);
//                }
//                //check if triangle indices already exists to save space
//                uint32_t* actual_indices = mesh->data.addTriangle(triangle.v2);
//                if(actual_indices != triangle.v2){
//                    delete[] triangle.v2;
//                    triangle.v2 = actual_indices;
//                }
//            }
//            else if (id == "v3") {
//                
//                //IMPORTANT
//                assert(child->args.size() == 2 + num_of_uvs);
//                
//                triangle.v3 = new uint32_t[2+num_of_uvs];
//                for (size_t i=0; i<2 + num_of_uvs; i++){
//                    triangle.v3[i] = stoi(child->args[i]);
//                }
//                //check if triangle indices already exists to save space
//                uint32_t* actual_indices = mesh->data.addTriangle(triangle.v3);
//                if(actual_indices != triangle.v3){
//                    //cout << "MALAKIES" << endl;
//                    //printArray(actual_indices, 2+num_of_uvs);
//                    delete[] triangle.v3;
//                    triangle.v3 = actual_indices;
//                }
//            }
//            else {
//                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Triangle\"");
//            }
//        } 
//        else if (type == "tagassignment") {
//            throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Triangle\"");
//        }
//        curNode = saveNode;
//    }
//    mesh->data.triangles.push_back(triangle);
//}



std::shared_ptr<OE_Texture> CSL_Interpreter::processTexture(CSL_Element_ptr texture_e) {
	std::shared_ptr<OE_Texture> texture = nullptr;

	for (auto &att : texture_e->attributes) {
		if ( std::holds_alternative<CSL_Assignment_ptr>(att) ) {
			auto att_ptr = std::get<CSL_Assignment_ptr>(std::move(att));

			if (att_ptr->name == "name"sv) {
				// TODO: Make OE_Texture use std::string_view istead of std::string
				texture = std::make_shared<OE_Texture>(std::string(att_ptr->value));
				this->texturesList.append(std::string(att_ptr->value), texture);
			} else {
				throw UnknownMemberAttributeError(att_ptr->value);
			}
		} else {
			throw SemanticError("The name attribute of the Texture element is not a list");
		}
	}

	for (auto &as : texture_e->assignments) {
		assert(texture != nullptr); // TODO

		if ( std::holds_alternative<CSL_Assignment_ptr>(as) ) {
			auto as_ptr = std::get<CSL_Assignment_ptr>(std::move(as));

			if (as_ptr->name == "path"sv) {
				texture->path = as_ptr->value;
			} else if (as_ptr->name == "camera") {
				// TODO: Make this use std::string_view
				texture->camera = this->objectsList.name2id[std::string(as_ptr->value)];
			} else {
				throw UnknownMemberVariableError(as_ptr->name);
			}
		} else {
			// TODO: Check this message is the same everywhere
			throw SemanticError("The Texture element does not have list member variables");
		}
	}

	if (texture_e->elements.size() > 0) {
		throw SemanticError("The Texture element does not have child elements");
	}

	return texture;
}

std::shared_ptr<OE_Material> CSL_Interpreter::processMaterial(CSL_Element_ptr material_e) {
	std::shared_ptr<OE_Material> material = nullptr;

	for (auto &att : material_e->attributes) {
		if ( std::holds_alternative<CSL_Assignment_ptr>(att) ) {
			auto att_ptr = std::get<CSL_Assignment_ptr>(std::move(att));

			if (att_ptr->name == "name"sv) {
				// TODO: Make OE_Material use std::string_view
				material = std::make_shared<OE_Material>(std::string(att_ptr->name));
				this->materialsList.append(std::string(att_ptr->name), material);
			} else {
				throw UnknownMemberAttributeError(att_ptr->name);
			}
		} else {
			throw SemanticError("The name attribute of the Material element is not a list");
		}
	}

	for (auto &as : material_e->assignments) {
		assert(material != nullptr);	// TODO: Exception. Also check if this has been done
										// in the functions further down
		if ( std::holds_alternative<CSL_Assignment_ptr>(as) ) {
			auto as_ptr = std::get<CSL_Assignment_ptr>(std::move(as));

			if (as_ptr->name == "translucency"sv) {
				material->translucency = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "alpha"sv) {
				material->alpha = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "specular_intensity"sv) {
				material->specular_intensity = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "specular_hardness"sv) {
				material->specular_hardness = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "dif_r"sv) {
				material->dif_r = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "dif_g"sv) {
				material->dif_g = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "dif_b"sv) {
				material->dif_b = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "dif_a"sv) {
				material->dif_a = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "scol_r"sv) {
				material->scol_r = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "scol_g"sv) {
				material->scol_g = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "scol_b"sv) {
				material->scol_b = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "illuminosity"sv) {
				material->illuminosity = csl::parseFloat(as_ptr->value);
			} else {
				throw UnknownMemberVariableError(as_ptr->name);
			}
		} else {
			auto as_ptr = std::get<CSL_ListAssignment_ptr>(std::move(as));

			if (as_ptr->name == "textureCM_IDs"sv) {
				for (const auto &v : as_ptr->values) {
					// TODO: Fuck, is this another list of strings?
					// mateiral->textureCM_IDs.push_back(this->tcmList.name2id[v]);
				}
			} else {
				throw UnknownMemberVariableError(as_ptr->name);
			}
		}
	}

	if (material_e->elements.size() != 0) {
		throw SemanticError("The Material element does not have child elements");
	}

	return material;
}


//
// Preset for adding new types
//
//std::shared_ptr<OE_<++>> CSL_Interpreter::process<++>(CSL_Element_ptr <++>_e) {
//	std::shared_ptr<OE_<++>> <++> = nullptr;
//
//	for (auto &att : <++>->attributes) {
//		if ( std::hold_alternative<CSL_Assignment_ptr>(att) ) {
//			auto att_ptr = std::get<CSL_Assignment_ptr>(std::move(att));
//
//			if (att_ptr->name == "name"sv) {
//				<++>
//			} else {
//				throw UnknownMemberAttributeError(att_ptr->name);
//			}
//		} else {
//			throw SemanticError("The name attribute of the <++> element is not a list");
//		}
//	}
//
//	for (auto &as : <++>->assignments) {
//		assert(<++> != nullptr); // TODO
//
//		if ( std::holds_alternative<CSL_Assignment_ptr>(as) ) {
//			auto as_ptr = std::get<CSL_Assignment_ptr>(std::move(as));
//
//			if (as_ptr->name == "<++>"sv) {
//				<++>
//			} else <++>{
//				throw UnknownMemberVariableError(as_ptr->name);
//			}
//		} else {
//			auto as_ptr = std::get<CSL_ListAssignment_ptr>(std::move(as));
//
//			if (as_ptr->name == "<++>"sv) {
//				for (const auto &v : as_ptr->values) {
//
//				}
//			} else <++>{
//				throw UnknownMemberVariableError(as_ptr->name);
//			}
//		}
//	}
//
//	for (auto &e : <++>->elements) {
//		assert(<++> != nullptr); // TODO (Also check for all below functions)
//		if (e->name == "<++>"sv) {
//		
//		} else <++>{
//			throw UnknownMemberElementError(e->name);
//		}
//	}
//
//	return <++>;
//}

std::shared_ptr<OE_TCM> CSL_Interpreter::processTCM(CSL_Element_ptr tcm_e) {
	std::shared_ptr<OE_TCM> tcm = nullptr;

	for (auto &att : tcm_e->attributes) {
		if ( std::holds_alternative<CSL_Assignment_ptr>(att) ) {
			auto att_ptr = std::get<CSL_Assignment_ptr>(std::move(att));

			if (att_ptr->name == "name"sv) {
				//TODO: Make OE_TCM use a std::string_view instead of std::string
				tcm = std::make_shared<OE_TCM>(std::string(att_ptr->value));
				//TODO: Make tcmList use a std::string_view instead of std::string
				this->tcmsList.append(std::string(att_ptr->value), tcm);
			} else {
				throw UnknownMemberAttributeError(att_ptr->name);
			}
		} else {
			throw SemanticError("The name attribute of the TCM element is not a list");
		}
	}

	for (auto &as : tcm_e->assignments) {
		if ( std::holds_alternative<CSL_Assignment_ptr>(as) ) {
			auto as_ptr = std::get<CSL_Assignment_ptr>(std::move(as));

			if (as_ptr->name == "combine_mode"sv) {
				tcm->combine_mode = csl::parseInt(as_ptr->value);
			} else if (as_ptr->name == "r"sv) {
				tcm->r = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "g"sv) {
				tcm->g = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "b"sv) {
				tcm->b = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "a"sv) {
				tcm->a = csl::parseFloat(as_ptr->value);
			} else if (as_ptr->name == "texture_array"sv) {
				// TODO: What does the double negation do?
				tcm->texture_array = !!csl::parseInt(as_ptr->value);
			} else {
				throw UnknownMemberVariableError(as_ptr->name);
			}
		} else {
			throw SemanticError("The TCM element does not contain list variables");
		}
	}

	if (tcm_e->elements.size() > 0) {
		throw SemanticError("The TCM element does not have child elements");
	}

	return tcm;
}

// TODO
//void CSL_Interpreter::processTCM_Texture(OE_TCM_Texture &tcm_tex) {
//    for (auto& child : curNode->children) {
//        string type = child->type;
//        string id = child->ID;
//
//        CSL_Node *saveNode = curNode;
//        if (type == "tag") {
//            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"TCM\"");
//        }
//        else if (type == "closedtag") {
//            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
//        }
//        else if (type == "assignment") {
//            if (id == "textureID") {
//                tcm_tex.textureID = this->texturesList.name2id(child->args[0]);
//            }
//            else if (id == "mode"){
//                tcm_tex.mode = stoi(child->args[0]);
//            }
//            else if (id == "textureMulFactor"){
//                tcm_tex.textureMulFactor = stoi(child->args[0]);
//            }
//            else if (id == "uvmap"){
//                tcm_tex.uvmap = stoi(child->args[0]);
//            }
//            else {
//                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"TCM\"");
//            }
//        }
//        else if (type == "listassignment") {
//            throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"TCM\"");
//        }
//        else if (type == "tagassignment") {
//            throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"TCM\"");
//        }
//        curNode = saveNode;
//    }
//}

std::shared_ptr<OE_ViewportConfig>
CSL_Interpreter::processViewportConfig(CSL_Element_ptr vconf_e) {
	// TODO: Why was this a nullpointer?	
    //std::shared_ptr<OE_ViewportConfig> vconf = nullptr;

	std::shared_ptr<OE_ViewportConfig> vconf = std::make_shared<OE_ViewportConfig>();

	for (auto &att : vconf_e->attributes) {
		if ( std::holds_alternative<CSL_Assignment_ptr>(att) ) {
			auto att_ptr = std::get<CSL_Assignment_ptr>(std::move(att));

			if (att_ptr->name == "name"sv) {
				//vconf = std::make_shared<OE_ViewportConfig>(child->args[0]);
				//this->viewportsList.append(child->args[0], vconf);
			} else {
				throw UnknownMemberAttributeError(att_ptr->name);
			}
		} else {
			throw SemanticError("The name attribute of the ViewportConfigElement is not a list");
		}
	}

	for (auto &as : vconf_e->assignments) {
		if ( std::holds_alternative<CSL_ListAssignment_ptr>(as) ) {
			auto as_ptr = std::get<CSL_ListAssignment_ptr>(std::move(as));

			if (as_ptr->name == "cameras"sv) {
				for (const auto &v : as_ptr->values) {
					// TODO: Fuck, is this a list-assignment with strings?
					//vconf->cameras.push_back(this->objectsList.name2id[v]);
				}
			} else if (as_ptr->name == "split_screen_positions"sv) {
				for (const auto &v : as_ptr->values) {
					vconf->split_screen_positions.push_back(v);
				}
			} else if (as_ptr->name == "camera_modes"sv) {
                for (const auto &v : as_ptr->values){
                    vconf->camera_modes.push_back(v);
                } 
			} else if (as_ptr->name == "layer_combine_modes"sv) {
				for (const auto &v : as_ptr->values) {
					// TODO: For ints use ints and not floats
					vconf->layer_combine_modes.push_back(static_cast<int>(v));
				}
			} else {
				throw UnknownMemberVariableError(as_ptr->name);
			}
		} else {
			throw SemanticError("The ViewportConfig element only contains list variables");
		}
	}

	if (vconf_e->elements.size() != 0) {
		throw SemanticError("The ViewportConfig element does not have child elements");
	}
    
	return vconf;
}
