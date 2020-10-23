#include <Carbon/CSL_Interpreter.h>

using namespace std;

// This is a helper function for the interpreter
void OE_ReverseBitset(std::bitset<64>& bitseta){
    std::bitset<64> temp;
    for (size_t i=0;i < 32; i++){
        temp[i] = bitseta[31-i];
    }
    bitseta = temp;
}


CSL_Interpreter::~CSL_Interpreter(){
    if (this->parser != nullptr)
        delete this->parser;
    //if (this->curNode != nullptr)
    //    delete this->curNode; // IS THIS ALSO NECESSARY!?
}

OE_World *CSL_Interpreter::interpret(string sourceCode) {
    //parse "sourcecode"
    if (parser != nullptr)
        delete parser;
    parser = new CSL_Parser();
    
    auto t = clock();
    
    curNode = parser->parse(sourceCode);
    auto t1 = clock();
    cout << "CSL TEST PARSE: " << (float)(t1 -t)/CLOCKS_PER_SEC << endl;
    //interpret the Abstract Syntax Tree obtained by parsing "sourceCode"
    OE_World *world = processWorld();
    cout << "CSL TEST INTERPRET: " << (float)(clock()-t1)/CLOCKS_PER_SEC << endl;
    return world;
}

OE_World *CSL_Interpreter::interpretFile(string pathToFile) {
	string sourceCode = "";
	string line;
    
    auto t = clock();
	//read all lines in "pathToFile"
	ifstream in(pathToFile);
	while (getline(in, line))
		sourceCode += line;
	in.close();
    cout << "CSL TEST READ FILE: " << (float)(clock() -t)/CLOCKS_PER_SEC << endl;
	return interpret(sourceCode);
}

OE_World* CSL_Interpreter::processWorld() {
    
    
    OE_World* world = new OE_World();
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;
        
        CSL_Node *saveNode = curNode;
        curNode = child;
        if (type == "tag") {
            if (id == "Scene") {
                auto scene = processScene();
                world->scenes[scene->id] = scene;
            } 
            else if (id == "ViewportConfig") {
                auto vconf = processViewportConfig();
                world->viewports[vconf->id] = vconf;
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
                world->loaded_viewport = OE_ViewportConfig::name2id[child->args[0]]; 
            } 
            else if (id == "loaded_scene") {
                world->loaded_scene = OE_Scene::name2id[child->args[0]];
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
    }
    return world;
}

OE_Scene* CSL_Interpreter::processScene() {
    OE_Scene* scene = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        curNode = child;
        if (type == "tag") {
            assert (scene != nullptr);
            if (id == "Camera") {
                OE_Camera* camera = processCamera();
                scene->objects[camera->id] = camera;
            } 
            else if (id == "Light") {
                OE_Light *light = processLight();
                scene->objects[light->id] = light;
            } 
            else if (id == "Mesh") {
                OE_Mesh32 *mesh = processMesh();
                scene->objects[mesh->id] = mesh;
            } 
            else if (id == "Texture") {
                OE_Texture *texture = processTexture();
                scene->textures[texture->id] = texture;
            } 
            else if (id == "Material") {
                OE_Material *material = processMaterial();
                scene->materials[material->id] = material;
            } 
            else if (id == "TextureCombineMode") {
                OE_TCM *tcm = processTCM();
                scene->texture_CMs[tcm->id] = tcm;
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
                scene = new OE_Scene(child->args[0]);
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID\"" + id + "\" in \"Scene\"");
            }
        }
        curNode = saveNode;
    }
    return scene;
}

OE_Camera * CSL_Interpreter::processCamera() {
    
    OE_Camera* camera = nullptr;
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
                camera->parent = OE_Object::name2id[child->args[0]];
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
                camera = new OE_Camera(child->args[0]);
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

OE_Light * CSL_Interpreter::processLight() {
    OE_Light* light = nullptr;
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
                light->parent = OE_Object::name2id[child->args[0]];
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
                    light->objects.push_back(OE_Object::name2id[x]);
                } 
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Light\"");
            }
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                light = new OE_Light(child->args[0]);
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
}

OE_Mesh32* CSL_Interpreter::processMesh() {
    
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
    
    OE_Mesh32* mesh = nullptr;
    
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        curNode = child;
        
        
        
        if (type == "tag") {
            assert (mesh != nullptr);
            if (id == "VertexGroup") {
                OE_VertexGroup *vgroup = processVertexGroup();
                mesh->data.triangle_groups[vgroup->id] = vgroup;
            } 
            else if (id == "Triangle") {
                //-----------------------------WTF IS THE CONSTRUCTOR FOR!?--------------------------------------
                //------------------------------DON'T USE A CONSTRUCTR IN TYPES, BECAUSE THEN I CAN'T ACCESS THE TYPES---------------------------------
                //ANSWER: well ok
                // ANSWER 2020: Rewrote the interpreter to use CUSTOM CONSTRUCTORS
                // and references to pointers :))))))))))))
                triangles_reserved = true;
                assert(uvs_set == true);
                assert(uvs_set == true);
                assert(map_chosen == true);
                processTriangle(mesh, mesh->data.num_of_uvs);
                
            }
            else if (id == "UVMapData"){
                
                OE_UVMapData uvmap;
                processUVMapData(uvmap, num_of_uvmaps);
                mesh->data.vertices.uvmaps.push_back(uvmap);
                num_of_uvmaps++;
            }
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"" + id + "\" is not an accepted tag-ID in \"Mesh\"");
            }
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
            assert (mesh != nullptr);
            if (id == "parent") {
                mesh->parent = OE_Object::name2id[child->args[0]];
            }
            else if (id == "parent_type") {
                mesh->parent_type = stoi(child->args[0]);
            }
            else if (id == "num_of_vertices"){
                if(!vertices_reserved){
                    mesh->data.vertices.positions.reserve(stoi(child->args[0]));
                    vnum = stoi(child->args[0]);
                }
                vertices_reserved = true;
            } 
            else if (id == "num_of_normals"){
                if(!normals_reserved){
                    mesh->data.vertices.normals.reserve(stoi(child->args[0]));
                    nnum = stoi(child->args[0]);
                }
                normals_reserved = true;
            }
            else if (id == "num_of_triangles"){
                if(!triangles_reserved){
                    mesh->data.triangles.reserve(stoi(child->args[0]));
                    num_of_triangles = stoi(child->args[0]);
                }
                triangles_reserved = true;
            }
            else{
                throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variables in \"Mesh\"");
            }
        } 
        else if (type == "listassignment") {
			assert (mesh != nullptr);
            if (id == "current_state") {
                assert (child->args.size() == 10);
                mesh->current_state.pos_x = stof(child->args[0]);
                mesh->current_state.pos_y = stof(child->args[1]);
                mesh->current_state.pos_z = stof(child->args[2]);
                mesh->current_state.rot_w = stof(child->args[3]);
                mesh->current_state.rot_x = stof(child->args[4]);
                mesh->current_state.rot_y = stof(child->args[5]);
                mesh->current_state.rot_z = stof(child->args[6]);
                mesh->current_state.sca_x = stof(child->args[7]);
                mesh->current_state.sca_y = stof(child->args[8]);
                mesh->current_state.sca_z = stof(child->args[9]);
            } 
            else if (id == "textureCM_IDs"){
                for (const auto& x: child->args){
                    mesh->textureCM_IDs.push_back(OE_TCM::name2id[x]); 
                } 
            }
            else if (id == "vertices") {
                vertices_reserved = true;
                for (const auto& x: child->args){
                    mesh->data.vertices.positions.push_back(stof(x));
                } 
            } 
            else if (id == "normals") {
                normals_reserved = true;
                for (const auto& x: child->args){
                    mesh->data.vertices.normals.push_back(stof(x));
                } 
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Mesh\"");
            }
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                mesh = new OE_Mesh32(child->args[0]);
            } 
            else if (id == "visible"){
                assert(mesh != nullptr);
                mesh->visible = !!stoi(child->args[0]);
            }
            else if (id == "isDynamic"){
                assert(mesh != nullptr);
                mesh->data.isDynamic = !!stoi(child->args[0]);
            }
            else if (id == "num_uvs"){
                assert(mesh != nullptr);
                mesh->data.num_of_uvs = stoi(child->args[0]);
                uvs_set = true;
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Mesh\"");
            }
        }
        if (!map_chosen && uvs_set && vertices_reserved && normals_reserved && (num_of_uvmaps == mesh->data.num_of_uvs) && triangles_reserved){
            
            size_t uvnum = 0;
            for (auto x : mesh->data.vertices.uvmaps){
                uvnum = max(uvnum, x.elements.size());
            }
            size_t tnum = num_of_triangles;
            // THIS here is a very big hack using polymorphism of C++
            // Now i can dynamically choose on runtime if i want ordered or unordered maps
            // Unordered map is O(1) fast on few vertices/triangles/etc ! (less than 65536 of each and less than 21845 triangles)
            // Unordered map is also just as fast when no uv maps are present
            // But ordered map O(logn) is faster on larger objects especially with several uv maps 
            
            if (vnum <= 65536 && nnum <= 65536 && uvnum <= 65536 && tnum*3 <= 65536 && num_of_uvmaps <= 2){
                mesh->data.initUnorderedIB(mesh);
            } 
            else if (num_of_uvmaps == 0){
                mesh->data.initUnorderedIB(mesh);
            }
            else{
                mesh->data.initOrderedIB(mesh);
            }
            
            map_chosen = true;
        }
        
        curNode = saveNode;
    }
    mesh->data.genVertexBufferInternally();
    mesh->data.genIndexBuffersInternally();
    return mesh;
}

void CSL_Interpreter::processUVMapData(OE_UVMapData &uvmap, const size_t& uvmapID) {

    bool uvs_set = false;
    bool hasInit = false;
    
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"UVMapData\"");
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
            assert(hasInit == true);
            if (id == "num_of_uvs") {
                if (!uvs_set){
                    uvmap.elements.reserve(stoi(child->args[0]));
                    uvs_set = true;
                }
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"UVMapData\"");
            }
        } 
        else if (type == "listassignment") {
            assert(hasInit == true);
            if (id == "elements") {
                uvs_set = true;
                for (const auto& x: child->args){
                    uvmap.elements.push_back(stof(x));
                } 
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"UVMapData\"");
            }
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                uvmap = OE_UVMapData(uvmapID, child->args[0]);
                hasInit = true;
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"UVMapData\"");
            }
        }
        curNode = saveNode;
    }
}

OE_VertexGroup * CSL_Interpreter::processVertexGroup() {
    
    OE_VertexGroup* vgroup = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"VertexGroup\"");
        }
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
            assert(vgroup != nullptr);
            if (id == "material_id") {
                vgroup->material_id = OE_Material::name2id[child->args[0]];
            } 
            else if (id == "bone_id") {
                vgroup->bone_id = 0;
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"VertexGroup\"");
            }
        } 
        else if (type == "listassignment") {
            assert(vgroup != nullptr);
            if (id == "polygons") {
                for (auto& polygon : child->args)
                    vgroup->polygons.push_back(stoi(polygon));
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"VertexGroup\"");
            }
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                vgroup = new OE_VertexGroup(child->args[0]);
            }
            else if (id == "visible"){
                assert(vgroup != nullptr);
                vgroup->visible = !!stoi(child->args[0]);
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"VertexGroup\"");
            }
        }
		curNode = saveNode;
    }
    return vgroup;
}

void CSL_Interpreter::processTriangle(OE_Mesh32 *mesh, const size_t &num_of_uvs) {
    
    OE_Triangle32 triangle = OE_Triangle32();
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Triangle\"");
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variables in \"Triangle\"");
        } 
        else if (type == "listassignment") {
            if (id == "v1") {
                
                //IMPORTANT
                assert(child->args.size() == 2 + num_of_uvs);
                
                triangle.v1 = new uint32_t[2+num_of_uvs];
                for (size_t i=0; i<2 + num_of_uvs; i++){
                    triangle.v1[i] = stoi(child->args[i]);
                }
                //check if triangle indices already exists to save space
                uint32_t* actual_indices = mesh->data.addTriangle(triangle.v1);
                if(actual_indices != triangle.v1){
                    delete[] triangle.v1;
                    triangle.v1 = actual_indices;
                }
            }
            else if (id == "v2") {
                
                //IMPORTANT
                assert(child->args.size() == 2 + num_of_uvs);
                
                triangle.v2 = new uint32_t[2+num_of_uvs];
                for (size_t i=0; i<2 + num_of_uvs; i++){
                    triangle.v2[i] = stoi(child->args[i]);
                }
                //check if triangle indices already exists to save space
                uint32_t* actual_indices = mesh->data.addTriangle(triangle.v2);
                if(actual_indices != triangle.v2){
                    delete[] triangle.v2;
                    triangle.v2 = actual_indices;
                }
            }
            else if (id == "v3") {
                
                //IMPORTANT
                assert(child->args.size() == 2 + num_of_uvs);
                
                triangle.v3 = new uint32_t[2+num_of_uvs];
                for (size_t i=0; i<2 + num_of_uvs; i++){
                    triangle.v3[i] = stoi(child->args[i]);
                }
                //check if triangle indices already exists to save space
                uint32_t* actual_indices = mesh->data.addTriangle(triangle.v3);
                if(actual_indices != triangle.v3){
                    //cout << "MALAKIES" << endl;
                    //printArray(actual_indices, 2+num_of_uvs);
                    delete[] triangle.v3;
                    triangle.v3 = actual_indices;
                }
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Triangle\"");
            }
        } 
        else if (type == "tagassignment") {
            throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Triangle\"");
        }
        curNode = saveNode;
    }
    mesh->data.triangles.push_back(triangle);
}


OE_Texture * CSL_Interpreter::processTexture() {
    
    OE_Texture* texture = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Texture\"");
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        }
        else if (type == "assignment") {
            assert(texture != nullptr);
            if (id == "path") {
                texture->path = child->args[0];
            } 
            else if (id == "camera") {
                texture->camera = OE_Camera::name2id[child->args[0]];
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Texture\"");
            }
        } 
        else if (type == "listassignment") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variables in \"Texture\"");
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                texture = new OE_Texture(child->args[0]);
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Texture\"");
            }
        }
        curNode = saveNode;
    }
    return texture;
}

OE_Material * CSL_Interpreter::processMaterial() {
    
    OE_Material* material = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"Material\"");
        }
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        }
        else if (type == "assignment") {
            assert(material != nullptr);
            if (id == "translucency") {
                material->translucency = stof(child->args[0]);
            } 
            else if (id == "alpha") {
                material->alpha = stof(child->args[0]);
            }
            else if (id == "specular_intensity") {
                material->specular_intensity = stof(child->args[0]);
            }
            else if (id == "specular_hardness") {
                material->specular_hardness = stof(child->args[0]);
            } 
            else if (id == "dif_r") {
                material->dif_r = stof(child->args[0]);
            } 
            else if (id == "dif_g") {
                material->dif_g = stof(child->args[0]);
            } 
            else if (id == "dif_b") {
                material->dif_b = stof(child->args[0]);
            } 
            else if (id == "dif_a") {
                material->dif_a = stof(child->args[0]);
            } 
            else if (id == "scol_r") {
                material->scol_r = stof(child->args[0]);
            } 
            else if (id == "scol_g") {
                material->scol_g = stof(child->args[0]);
            } 
            else if (id == "scol_b") {
                material->scol_b = stof(child->args[0]);
            } 
            else if (id == "illuminosity") {
                material->illuminosity = stof(child->args[0]);
            } 
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"Material\"");
            }
        } 
        else if (type == "listassignment") {
            assert(material != nullptr);
            if  (id == "textureCM_IDs") {
                for (const auto& x : child->args)
                    material->textureCM_IDs.push_back(OE_TCM::name2id[x]);
            } 
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"Material\"");
            }
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                material = new OE_Material(child->args[0]);
            } 
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"Material\"");
            }
        }
        curNode = saveNode;
    }
    return material;
}

OE_TCM * CSL_Interpreter::processTCM() {
    OE_TCM* tcm = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        if (type == "tag") {
            assert(tcm != nullptr);
            if (id == "TCM_Texture"){
                OE_TCM_Texture tcm_tex;
                processTCM_Texture(tcm_tex);
                tcm->textures.push_back(tcm_tex);
            }
            else{
                throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"TCM\"");
            }
        }
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        }
        else if (type == "assignment") {
            assert(tcm != nullptr);
            if (id == "combine_mode") {
                tcm->combine_mode = stoi(child->args[0]);
            }
            else if (id == "r"){
                tcm->r = stof(child->args[0]);
            }
            else if (id == "g"){
                tcm->g = stof(child->args[0]);
            }
            else if (id == "b"){
                tcm->b = stof(child->args[0]);
            }
            else if (id == "a"){
                tcm->a = stof(child->args[0]);
            }
            else if (id == "texture_array"){
                tcm->texture_array = !!stoi(child->args[0]);
            }
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"TCM\"");
            }
        }
        else if (type == "listassignment") {
            throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"TCM\"");
        }
        else if (type == "tagassignment") {
            if (id == "name") {
                tcm = new OE_TCM(child->args[0]);
            }
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"TCM\"");
            }
        }
        curNode = saveNode;
    }
    return tcm;
}

void CSL_Interpreter::processTCM_Texture(OE_TCM_Texture &tcm_tex) {
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"TCM\"");
        }
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        }
        else if (type == "assignment") {
            if (id == "textureID") {
                tcm_tex.textureID = OE_Texture::name2id(child->args[0]);
            }
            else if (id == "mode"){
                tcm_tex.mode = stoi(child->args[0]);
            }
            else if (id == "textureMulFactor"){
                tcm_tex.textureMulFactor = stoi(child->args[0]);
            }
            else if (id == "uvmap"){
                tcm_tex.uvmap = stoi(child->args[0]);
            }
            else {
                throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"TCM\"");
            }
        }
        else if (type == "listassignment") {
            throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No list-variable with the ID \"" + id + "\" in \"TCM\"");
        }
        else if (type == "tagassignment") {
            throw CSL_UnknownIDException("UnknownIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"TCM\"");
        }
        curNode = saveNode;
    }
}

OE_ViewportConfig* CSL_Interpreter::processViewportConfig() {
    
    OE_ViewportConfig* vconf = nullptr;
    for (auto& child : curNode->children) {
        string type = child->type;
        string id = child->ID;

        CSL_Node *saveNode = curNode;
        if (type == "tag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-blocks in \"UVMapData\"");
        } 
        else if (type == "closedtag") {
            throw CSL_UnexpectedTypeException("UnexpectedTypeException at " + to_string(child->line) + ":" + to_string(child->col) + ": \"closedtag\" is not beeing used by the current version of the Carbon_Scripting_Language");
        } 
        else if (type == "assignment") {
            throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"UVMapData\"");
        }
        else if (type == "listassignment") {
            assert(vconf != nullptr);
            if (id == "cameras") {
                for (const auto& x: child->args){
                    vconf->cameras.push_back(OE_Camera::name2id[x]);
                } 
            } 
            else if (id == "split_screen_positions"){
                for (const auto& x: child->args){
                    vconf->split_screen_positions.push_back(stof(x));
                } 
            }
            else if (id == "camera_modes"){
                for (const auto& x: child->args){
                    vconf->camera_modes.push_back(stoi(x));
                } 
            }
            else if (id == "layer_combine_modes"){
                for (const auto& x: child->args){
                    vconf->layer_combine_modes.push_back(stoi(x));
                } 
            }
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No regular-variable with the ID \"" + id + "\" in \"UVMapData\"");
            }
        } 
        else if (type == "tagassignment") {
            if (id == "name") {
                vconf = new OE_ViewportConfig( child->args[0]);
            } 
            else {
                throw CSL_UnknownIDException("UnexpectedIDException at " + to_string(child->line) + ":" + to_string(child->col) + ": No tag-variable with the ID \"" + id + "\" in \"UVMapData\"");
            }
        }
        curNode = saveNode;
    }
    return vconf;
}
