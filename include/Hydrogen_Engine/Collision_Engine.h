#ifndef COLISSION_ENGINE_H_
#define COLISSION_ENGINE_H_

#include "HE_Types.h"
#include "FE_World.h"

class Collision_Engine {
public:
	~Collision_Engine();
private:
	AABB generateAABB(FE_Mesh *mesh);
};

#endif
