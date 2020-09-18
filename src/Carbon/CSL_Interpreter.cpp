#include "CSL_Interpreter.h"

FE_World *CSL_Interpreter::interpret(string sourceCode) {
	//parse "sourcecode"
	parser = new CSL_Parser();
	curNode = parser->parse(sourceCode);

	//interpret the Abstract Syntax Tree obtained by parsing "sourceCode"
	FE_World *world = new FE_World();
	processWorld(world);

	return world;
}

FE_World *CSL_Interpreter::interpretFile(string pathToFile) {
	string sourceCode = "";
	string line;

	//read all lines in "pathToFile"
	ifstream in(pathToFile);
	while (getline(in, line))
		sourceCode += line;
	in.close();

	return interpret(sourceCode);
}

void CSL_Interpreter::processWorld(FE_World* world) {
	for (auto& child : curNode->children) {
		string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
		curNode = child;
		if (type == "tag") {
			if (id == "Scene") {
				FE_Scene *scene = new FE_Scene();
				processScene(scene);
				world->scenes.push_back(scene);
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"" + id + "\" is not an accepted tag-ID in \"World\"");
			}
		} else if (type == "closedtag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
		} else if (type == "assignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variables in \"World\"");
		} else if (type == "listassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"World\"");
		} else if (type == "tagassignment") {
                        if (id == "name") {
                                world->name = child->args[0];
                        } else if (id == "loaded_scene") {
				world->name = child->args[0];
                        } else {
                                throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"World\"");
                        }
		}
		curNode = saveNode;
	}
}

void CSL_Interpreter::processScene(FE_Scene *scene) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
		curNode = child;
                if (type == "tag") {
			if (id == "Camera") {
				FE_Camera *camera = new FE_Camera();
				processCamera(camera);
				scene->cameras.push_back(camera);
			} else if (id == "Light") {
				FE_Light *light = new FE_Light();
				processLight(light);
				scene->lights.push_back(light);
			} else if (id == "Mesh") {
				FE_Mesh *mesh = new FE_Mesh();
				processMesh(mesh);
				scene->objects.push_back(mesh);
			} else if (id == "Texture") {
				FE_Texture *texture = new FE_Texture();
				processTexture(texture);
				scene->textures.push_back(texture);
			} else if (id == "Material") {
				FE_Material *material = new FE_Material();
				processMaterial(material);
				scene->materials.push_back(material);
			} else if (id == "TCM") {
				FE_TCM *tcm = new FE_TCM();
				processTCM(tcm);
				scene->textureCombineModes.push_back(tcm);
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"" + id + "\" is not an accepted tag-ID in \"Scene\"");
			}
		} else if (type == "closedtag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "gravity") {
				scene->gravity = stof(child->args[0]);
			} else if (id == "gravityAxis") {
				scene->gravityAxis = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Scene\"");
			}
                } else if (type == "listassignment") {
			if (id == "groups") {
				scene->groups = child->args;
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Scene\"");
			}
                } else if (type == "tagassignment") {
			if (id == "name") {
				scene->name = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID\"" + id + "\" in \"Scene\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processCamera(FE_Camera *camera) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
		curNode = child;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Camera\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "aspect_ratio") {
				camera->aspect_ratio = stof(child->args[0]);
			} else if (id == "field_of_view") {
				camera->field_of_view = stof(child->args[0]);
			} else if (id == "near") {
				camera->near = stof(child->args[0]);
			} else if (id == "far") {
				camera->far = stof(child->args[0]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Camera\"");
			}
                } else if (type == "listassignment") {
			if (id == "pos") {
				camera->pos.x = stof(child->args[0]);
				camera->pos.y = stof(child->args[1]);
				camera->pos.z = stof(child->args[2]);
				camera->pos.w = stof(child->args[3]);
			} else if (id == "rot") {
				camera->rot.x = stof(child->args[0]);
				camera->rot.y = stof(child->args[1]);
				camera->rot.z = stof(child->args[2]);
				camera->rot.w = stof(child->args[3]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Camera\"");
			}
                } else if (type == "tagassignment") {
			if (id == "name") {
				camera->name = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID\"" + id + "\" in \"Camera\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processLight(FE_Light *light) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
		curNode = child;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Light\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "light_type") {
				light->light_type = child->args[0];
			} else if (id == "value") {
				light->value = stof(child->args[0]);
			} else if (id == "fov") {
				light->fov = stof(child->args[0]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Light\"");
			}
                } else if (type == "listassignment") {
                        if (id == "pos") {
                                light->pos.x = stof(child->args[0]);
                                light->pos.y = stof(child->args[1]);
                                light->pos.z = stof(child->args[2]);
                                light->pos.w = stof(child->args[3]);
                        } else if (id == "rot") {
                                light->rot.x = stof(child->args[0]);
                                light->rot.y = stof(child->args[1]);
                                light->rot.z = stof(child->args[2]);
                                light->rot.w = stof(child->args[3]);
			} else if (id == "color") {
				light->color.x = stof(child->args[0]);
				light->color.y = stof(child->args[1]);
				light->color.z = stof(child->args[2]);
				light->color.w = stof(child->args[3]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Light\"");
			}
                } else if (type == "tagassignment") {
			if (id == "name") {
				light->name = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID\"" + id + "\" in \"Light\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processInteraction(FE_Interaction *interaction) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
		curNode = child;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Interaction\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "interaction") {
				interaction->interaction = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Interaction\"");
			}
                } else if (type == "listassignment") {
			if (id == "objectNames") {
				interaction->objectNames = child->args;
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Interaction\"");
			}
                } else if (type == "tagassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-vriables in \"Interaction\"");
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processMesh(FE_Mesh *mesh) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
		curNode = child;
                if (type == "tag") {
			if (id == "Vertex") {
			    /// einai sigouro oti o parser diavazei ta vertices me tin seira pou ta grafei? an oxi gamithike o renderer!
				FE_Vertex *vertex = new FE_Vertex();
				processVertex(vertex);
				mesh->vertices.push_back(vertex);
			} else if (id == "VertexGroup") {
				FE_VertexGroup *vgroup = new FE_VertexGroup();
				processVertexGroup(vgroup);
				mesh->vertex_groups.push_back(vgroup);
			} else if (id == "Animation") {
				FE_Animation *animation = new FE_Animation();
				processAnimation(animation);
				mesh->animations.push_back(animation);
			} else if (id == "Polygon") {
				//-----------------------------WTF IS THE CONSTRUCTOR FOR!?--------------------------------------
				//------------------------------DON'T USE A CONSTRUCTR IN TYPES, BECAUSE THEN I CAN'T ACCESS THE TYPES---------------------------------
				//ANSWER: well ok
				/*FE_Polygon *polygon = new FE_Polygon();
				processPolygon(polygon);
				mesh->polygons.push_back(polygon);*/
			} else if (id == "Physics") {
				FE_Physics *physics = new FE_Physics();
				processPhysics(physics);
				mesh->physics = physics;
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"" + id + "\" is not an accepted tag-ID in \"Mesh\"");
			}
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variables in \"Mesh\"");
                } else if (type == "listassignment") {
			if (id == "pos") {
                                mesh->pos.x = stof(child->args[0]);
                                mesh->pos.y = stof(child->args[1]);
                                mesh->pos.z = stof(child->args[2]);
                                mesh->pos.w = stof(child->args[3]);
                        } else if (id == "rot") {
                                mesh->rot.x = stof(child->args[0]);
                                mesh->rot.y = stof(child->args[1]);
                                mesh->rot.z = stof(child->args[2]);
                                mesh->rot.w = stof(child->args[3]);
                        } else if (id == "sca") {
                                mesh->sca.x = stof(child->args[0]);
                                mesh->sca.y = stof(child->args[1]);
                                mesh->sca.z = stof(child->args[2]);
                                mesh->sca.w = stof(child->args[3]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Mesh\"");
			}
                } else if (type == "tagassignment") {
			if (id == "name") {
				mesh->name = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Mesh\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processVertex(FE_Vertex *vertex) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
		curNode = child;
                if (type == "tag") {
			if (id == "UVMap") {
				FE_UVMap *uvmap = new FE_UVMap();
				processUVMap(uvmap);
				vertex->uvmaps.push_back(uvmap);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-block with the ID \"" + id + "\" in \"Vertex\"");
			}
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "x") {
				vertex->x = stof(child->args[0]);
			} else if (id == "y") {
				vertex->y = stof(child->args[0]);
			} else if (id == "z") {
				vertex->z = stof(child->args[0]);
			} else if (id == "normal_x") {
				vertex->normalx = stof(child->args[0]);
			} else if (id == "normal_y") {
				vertex->normaly = stof(child->args[0]);
			} else if (id == "normal_z") {
				vertex->normalz = stof(child->args[0]);
			} else if (id == "id") {
				vertex->n = stoi(child->args[0]); 
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Vertex\"");
			}
                } else if (type == "listassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"Vertex\"");
                } else if (type == "tagassignment") {
			if (id == "n") {
				//vertex->n = stoi(child->args[0]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Vertex\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processUVMap(FE_UVMap *uvmap) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"UVMap\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "u") {
				uvmap->u = stof(child->args[0]);
			} else if (id == "v") {
				uvmap->v = stof(child->args[0]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"UVMap\"");
			}
                } else if (type == "listassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"UVMap\"");
                } else if (type == "tagassignment") {
			if (id == "name") {
				uvmap->name = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"UVMap\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processVertexGroup(FE_VertexGroup *vgroup) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"VertexGroup\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "material") {
				vgroup->material = child->args[0];
			} else if (id == "bone_name") {
				vgroup->bone_name = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"VertexGroup\"");
			}
                } else if (type == "listassignment") {
			if (id == "polygons") {
				for (auto& polygon : child->args)
					vgroup->polygons.push_back(stoi(polygon));
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"VertexGroup\"");
			}
                } else if (type == "tagassignment") {
			if (id == "name") {
				vgroup->name = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"VertexGroup\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processAnimation(FE_Animation *animation) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			if (id == "Transformation") {
				FE_Transformation *transformation = new FE_Transformation();
				processTransformation(transformation);
				animation->transforms.push_back(transformation);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-block with the ID \"" + id + "\" in \"Animation\"");
			}
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "frameNum") {
				animation->frameNum = stoi(child->args[0]);
			} else if (id == "inverse_kinematics") {
				animation->inverse_kinematics = stoi(child->args[0]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Animation\"");
			}
                } else if (type == "listassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"Animation\"");
                } else if (type == "tagassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variables in \"Animation\"");
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processTransformation(FE_Transformation *transformation) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Transformation\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"Transformation\"");
                } else if (type == "listassignment") {
                        if (id == "pos") {
                                transformation->pos.x = stof(child->args[0]);
                                transformation->pos.y = stof(child->args[1]);
                                transformation->pos.z = stof(child->args[2]);
                                transformation->pos.w = stof(child->args[3]);
                        } else if (id == "rot") {
                                transformation->rot.x = stof(child->args[0]);
                                transformation->rot.y = stof(child->args[1]);
                                transformation->rot.z = stof(child->args[2]);
                                transformation->rot.w = stof(child->args[3]);
                        } else if (id == "sca") {
                                transformation->sca.x = stof(child->args[0]);
                                transformation->sca.y = stof(child->args[1]);
                                transformation->sca.z = stof(child->args[2]);
                                transformation->sca.w = stof(child->args[3]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Transformation\"");
			}
                } else if (type == "tagassignment") {
			if (id == "bonename") {
				transformation->bonename = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Transformation\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processPolygon(FE_Polygon *polygon) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Polygon\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variables in \"Polygon\"");
                } else if (type == "listassignment") {
                        if (id == "normal") {
                                polygon->normal.x = stof(child->args[0]);
                                polygon->normal.y = stof(child->args[1]);
                                polygon->normal.z = stof(child->args[2]);
                                polygon->normal.w = stof(child->args[3]);
			} else if (id == "vertexNums") {
				for (auto& num : child->args)
					polygon->vertexNums.push_back(stoi(num));
                        } else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Polygon\"");
			}
                } else if (type == "tagassignment") {
			if (id == "n") {
				polygon->n = stoi(child->args[0]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Polygon\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processPhysics(FE_Physics *physics) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Physics\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "bounciness") {
				physics->bounciness = child->args[0];
			} else if (id == "vehicle") {
				physics->vehicle = stoi(child->args[0]);
			} else if (id == "tire") {
				physics->tire = stoi(child->args[0]);
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Physics\"");
			}
                } else if (type == "listassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"Physics\"");
                } else if (type == "tagassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variables in \"Physics\"");
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processTexture(FE_Texture *texture) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Texture\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "path") {
				texture->path = child->args[0];
			} else if (id == "type") {
				texture->type = child->args[0];
			} else if (id == "stencil_name") {
				texture->stencil_name = child->args[0];
			} else if (id == "uv_map") {
				texture->uv_map = child->args[0];
			} else if (id == "stencil_num") {
				texture->stencil_num = stoi(child->args[0]);
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Texture\"");
			}
                } else if (type == "listassignment") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"Texture\"");
                } else if (type == "tagassignment") {
			if (id == "name") {
				texture->name = child->args[0];
			} else {
				throw UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Texture\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processMaterial(FE_Material *material) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Material\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
                        if (id == "shadeless") {
                                material->shadeless = stoi(child->args[0]);
                        } else if (id == "alpha") {
                                material->alpha = stof(child->args[0]);
                        } else if (id == "specular_intensity") {
                                material->specular_intensity = stof(child->args[0]);
                        } else if (id == "specular_hardness") {
                                material->specular_hardness = stof(child->args[0]);
			} else if (id == "cm_resolution") {
				material->cm_resolution = stof(child->args[0]);
			} else if (id == "cube_map") {
				material->cube_map = stof(child->args[0]);
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Material\"");
			}
                } else if (type == "listassignment") {
			if (id == "dif") {
				material->dif_r = stof(child->args[0]);
				material->dif_g = stof(child->args[1]);
				material->dif_b = stof(child->args[2]);
				material->dif_a = stof(child->args[3]);
			} else if (id == "scol") {
				material->scol_r = stof(child->args[0]);
				material->scol_g = stof(child->args[1]);
				material->scol_b = stof(child->args[2]);
				material->scol_a = stof(child->args[3]);
			} else if (id == "translucency") {
                                material->translucency.x = stof(child->args[0]);
                                material->translucency.y = stof(child->args[1]);
                                material->translucency.z = stof(child->args[2]);
                                material->translucency.w = stof(child->args[3]);
			} else if (id == "illuminosity") {
                                material->illuminosity.x = stof(child->args[0]);
                                material->illuminosity.y = stof(child->args[1]);
                                material->illuminosity.z = stof(child->args[2]);
                                material->illuminosity.w = stof(child->args[3]);
			} else if (id == "textureNames") {
				material->textureNames = child->args;
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Material\"");
			}
                } else if (type == "tagassignment") {
			if (id == "name") {
				material->name = child->args[0];
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Material\"");
			}
                }
		curNode = saveNode;
        }
}

void CSL_Interpreter::processTCM(FE_TCM *tcm) {
        for (auto& child : curNode->children) {
                string type = child->type;
		string id = child->ID;

		CNode *saveNode = curNode;
                if (type == "tag") {
			throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"TCM\"");
                } else if (type == "closedtag") {
                        throw UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
                } else if (type == "assignment") {
			if (id == "combine_mode") {
				tcm->combine_mode = child->args[0];
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"TCM\"");
			}
                } else if (type == "listassignment") {
			if (id == "value") {
				tcm->value.x = stof(child->args[0]);
				tcm->value.y = stof(child->args[1]);
				tcm->value.z = stof(child->args[2]);
				tcm->value.w = stof(child->args[3]);
			} else if (id == "textureNames") {
				tcm->textureNames = child->args;
			} else if (id == "tex_indices") {
				tcm->tex_indices = child->args;
			} else if (id == "modes") {
				for (auto& mode : child->args)
					tcm->modes.push_back(stoi(mode));
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"TCM\"");
			}
                } else if (type == "tagassignment") {
			if (id == "name") {
				tcm->name = child->args[0];
			} else {
				throw UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"TCM\"");
			}
                }
		curNode = saveNode;
        }
}

