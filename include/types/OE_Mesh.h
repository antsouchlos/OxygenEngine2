#ifndef OE_MESH_H
#define OE_MESH_H

#include <types/OE_Object.h>
#include <types/OE_PolygonStorage.h>
#include <types/OE_TCM.h>

/* OE_Mesh32 represents a mesh with 32-bit index, which means more than 65535 unique vertex combinations are allowed.
 */

class OE_Mesh32 : public OE_Object{
    
    friend class CSL_Interpreter;
    
    public:
    
        OE_Mesh32(uint32_t num_of_vertices, uint32_t num_of_normals,
						uint32_t num_of_triangles, uint32_t num_of_uvs, uint32_t max_uv_num);
		~OE_Mesh32();
        
        // This takes into account rotation as well
        void calculateProperBoundingBox();
        
        std::string getType() const;
        std::string to_str() const;
        
    //protected:
        
		//TODO: std::make_shared in processMesh
		std::shared_ptr<OE_PolygonStorage32>    data;
        std::vector<std::size_t>                textureCM_IDs;
        //std::size_t                             num_of_triangles;
        //std::size_t                             num_of_vertices;
        //std::size_t                             num_of_normals
        void*                                   physics_data;
};



#endif
