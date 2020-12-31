#include <types/OE_Mesh.h>
#include <types/OE_World.h>
#include <OE_Math.h>

using namespace std;

OE_Mesh32::OE_Mesh32(){
    
   this->physics_data     = nullptr;
   this->data = std::make_shared<OE_PolygonStorage32>();
}

OE_Mesh32::OE_Mesh32(const string& name) : OE_Object(name){
    
    this->physics_data     = nullptr;
	this->data = std::make_shared<OE_PolygonStorage32>();
}

OE_Mesh32::~OE_Mesh32(){
    
}

// This takes into account rotation as well

void OE_Mesh32::calculateProperBoundingBox(){
    
    
    //auto t = clock();
    
    OE_Mat3x3 rot_matrix = OE_Mat3x3(this->GetModelMatrix());
    
    assert((this->data->vertices.positions.size() >= 3) && (this->data->vertices.positions.size() % 3 == 0));
    
    // find min/max position on each axis
    // apply the inverse rotation to find the proper bounding box again
    OE_Vec3 rotated_initial_pos = rot_matrix * OE_Vec3(this->data->vertices.positions[0], this->data->vertices.positions[1], this->data->vertices.positions[2]);
    
    this->data->vertices.max_x = rotated_initial_pos[0];
    this->data->vertices.max_y = rotated_initial_pos[1];
    this->data->vertices.max_z = rotated_initial_pos[2];
    
    this->data->vertices.min_x = rotated_initial_pos[0];
    this->data->vertices.min_y = rotated_initial_pos[1];
    this->data->vertices.min_z = rotated_initial_pos[2];
    
    for (size_t i=0; i < this->data->vertices.positions.size(); i+=3){
            
            OE_Vec3 rotated_pos = rot_matrix * OE_Vec3(this->data->vertices.positions[i], this->data->vertices.positions[i+1], this->data->vertices.positions[i+2]);
            
            if (rotated_pos[0] < this->data->vertices.min_x){
                this->data->vertices.min_x = rotated_pos[0];
                
            }
            if (rotated_pos[0] > this->data->vertices.max_x){
                this->data->vertices.max_x = rotated_pos[0];
               
            }
       
            if (rotated_pos[1] < this->data->vertices.min_y){
                this->data->vertices.min_y = rotated_pos[1];
                
            }
            if (rotated_pos[1] > this->data->vertices.max_y){
                this->data->vertices.max_y = rotated_pos[1];
               
            }
       
            if (rotated_pos[2] < this->data->vertices.min_z){
                this->data->vertices.min_z = rotated_pos[2];
                
            }
            if (rotated_pos[2] > this->data->vertices.max_z){
                this->data->vertices.max_z = rotated_pos[2];
               
            }
    }
    
    this->data->vertices.calculatedBoundingBox = true;
    //cout << "BOUNDING BOX UPDATE IN " << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
}


string OE_Mesh32::getType() const{
    return "MESH32";
}

std::string OE_Mesh32::to_str() const{
    
    string output = "";
    //vector<string> output_list;
    //output_list.reserve(9+ this->textureCM_IDs.size() + this->data->vertices.uvmaps.size() + this->data->triangles.size() + this->data.triangle_groups.size()+1);
    
    output.append(outputTypeTag("Mesh", {{"name", "\"" + OE_World::objectsList.id2name[this->id] + "\""}, {"visible", convert((int)visible)}, {"num_uvs", convert(data->vertices.uvmaps.size())}, {"isDynamic", convert((int)this->data->isDynamic)}}));
    CSL_WriterBase::indent = CSL_WriterBase::indent + 1;
    output.append("\n");
    
    output.append(outputList("current_state", this->current_state.to_arr()));
    output.append("\n");
    
    output.append(outputVar("parent", "\"" + OE_World::objectsList.id2name[this->parent] + "\""));
    output.append("\n");
    
    output.append(outputVar("parent_type", convert(this->parent_type)));
    output.append("\n");
    
    output.append(outputVar("num_of_vertices", convert(this->data->vertices.positions.size())));
    output.append("\n");
    
    output.append(outputVar("num_of_normals", convert(this->data->vertices.normals.size())));
    output.append("\n");
    
    output.append(outputVar("num_of_triangles", convert(this->data->triangles.size())));
    output.append("\n");
    
    output.append(outputList("vertices", this->data->vertices.positions));
    output.append("\n");
    
    output.append(outputList("normals", this->data->vertices.normals));
    vector<string> tcm_strs;
    for(const auto &x: this->textureCM_IDs){
        tcm_strs.push_back("\"" + OE_World::tcmsList.id2name[x] + "\"");
    }
    if(tcm_strs.size() != 0){
        output.append(outputList("textureCM_IDs", tcm_strs));
        output.append("\n");
    }
    
    for (const auto& x: this->data->vertices.uvmaps){
        output.append(x.to_str());
        output.append("\n");
    }
    for (const auto& x: this->data->triangles){
        output.append(x.to_str(2+this->data->num_of_uvs));
        output.append("\n");
    }
    for (const auto& x: this->data->triangle_groups){
        output.append(x.second->to_str());
        output.append("\n");
    }
    output = output.substr(0, output.size()-1);
    
    CSL_WriterBase::indent = CSL_WriterBase::indent - 1;
    output.append(outputClosingTag("Mesh"));
    //return CSL_Join("\n", output_list);
    return output;
}
