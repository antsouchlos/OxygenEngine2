#include "FE_Mesh.h"

FE_Mesh::FE_Mesh()
{
    createMutex();
}

FE_Mesh::~FE_Mesh()
{
    //ctor
}

bool FE_Mesh::init()
{
    return true;
}

bool FE_Mesh::destroy()
{
    for(auto x : this->vertex_groups){
        delete x;
    }
    this->vertex_groups.clear();

    for(auto x : this->vertices){
        x->destroy();
        delete x;
    }
    this->vertices.clear();

    for(auto x : this->polygons){
        delete x;
    }
    this->polygons.clear();

    for(auto x : this->animations){
        x->destroy();
        delete x;
    }
    this->animations.clear();
    return true;
}
//DONE
bool FE_Mesh::hasMaterial(string info){

    bool output = false;

    for(auto x : this->vertex_groups)
    if(x->name == info){

        output = true;
        break;
    }
    return output;
}

bool FE_Mesh::hasPolygonMaterial(string info, size_t n){

    bool output = false;

    for(auto x : this->vertex_groups)
    if(x->name == info){

        for(auto o : x->polygons )
        if (o == n){
            output = true;
            break;
        }


    }
    return output;
}
//DONE
FE_Mesh* FE_Mesh::setData(void* info){
    data = info;
    return this;
}

// VERTICES
//DONE
FE_Vertex* FE_Mesh::createVertex(){
    lockMutex();

    FE_Vertex* output = new FE_Vertex(vertices.size());
    vertices.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Vertex* FE_Mesh::getVertex(size_t a_name){
    lockMutex();

    FE_Vertex* output = nullptr;
    for(size_t i = 0; i < vertices.size(); i++)
    if(vertices[i]->n == a_name){
        output = vertices[i];
    }

    unlockMutex();
    return output;
}
//DONE
FE_Vertex* FE_Mesh::getLastVertex(int offset){
    return this->vertices[vertices.size()-offset-1];
}
//DONE
FE_Mesh* FE_Mesh::removeVertex(int a_name){
    lockMutex();
    delete vertices[a_name];
    vertices.erase(vertices.begin() + a_name);
    unlockMutex();
    return this;
}
//DONE
size_t   FE_Mesh::getNumVertices(){
    return this->vertices.size();
}

// POLYGONS
//DONE
FE_Polygon* FE_Mesh::createPolygon(int v1, int v2, int v3){
    lockMutex();

    FE_Polygon* output = new FE_Polygon(polygons.size(), v1, v2, v3);
    polygons.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Polygon* FE_Mesh::getPolygon(int a_name){
   lockMutex();

    FE_Polygon* output = nullptr;
    for(unsigned int i = 0; i < polygons.size(); i++)
    if(polygons[i]->n == a_name){
        output = polygons[i];
    }

    unlockMutex();
    return output;
}
//DONE
FE_Polygon* FE_Mesh::getLastPolygon(int offset){
    return this->polygons[polygons.size()-offset-1];
}
//DONE
FE_Mesh*  FE_Mesh::removePolygon(int a_name){
    lockMutex();
    delete polygons[a_name];
    polygons.erase(polygons.begin() + a_name);
    unlockMutex();
    return this;
}
//DONE
size_t  FE_Mesh::getNumPolygons(){
    return this->polygons.size();
}

// VERTEX GROUPS
//DONE
FE_VertexGroup* FE_Mesh::createVertexGroup(string a_name){
    lockMutex();

    FE_VertexGroup* output = new FE_VertexGroup();
    output->name = a_name;
    vertex_groups.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_VertexGroup* FE_Mesh::getVertexGroup(string a_name){
    lockMutex();

    FE_VertexGroup* output = nullptr;

    for(auto obj : vertex_groups)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;
}
//DONE
FE_Mesh* FE_Mesh::renameVertexGroup(string a_name, string prev_name){
    lockMutex();
    this->getVertexGroup(a_name)->name = prev_name;
    unlockMutex();
    return this;
}
//DONE
FE_Mesh* FE_Mesh::removeVertexGroup(string a_name){
    lockMutex();
    for(unsigned int i=0; i< vertex_groups.size();i++)
    if(vertex_groups[i]->name == a_name){
        delete vertex_groups[i];
        vertex_groups.erase(vertex_groups.begin()+i);
    }

    unlockMutex();
    return this;
}


// ANIMATIONS
//DONE
FE_Animation* FE_Mesh::createAnimation(string a_name){
    lockMutex();

    FE_Animation* output = new FE_Animation();
    output->name = a_name;
    animations.push_back(output);

    unlockMutex();
    return output;
}
//DONE
FE_Animation* FE_Mesh::getAnimation(string a_name){
    lockMutex();

    FE_Animation* output = nullptr;

    for(auto obj : animations)
    if(obj->name == a_name)
        output = obj;

    unlockMutex();
    return output;
}
//DONE
FE_Mesh* FE_Mesh::renameAnimation(string a_name, string prev_name){
    lockMutex();
    this->getAnimation(a_name)->name = prev_name;
    unlockMutex();
    return this;
}
//DONE
FE_Mesh* FE_Mesh::removeAnimation(string a_name){
    lockMutex();
    for(unsigned int i=0; i< animations.size();i++)
    if(animations[i]->name == a_name){
        delete animations[i];
        animations.erase(animations.begin()+i);
    }

    unlockMutex();
    return this;
}


// PHYSICS
//DONE
FE_Physics* FE_Mesh::getPhysics(){
    return this->physics;
}
//DONE
FE_Physics* FE_Mesh::getPhysicsProperties(){
    return this->getPhysics();
}


// boilerplate virtual functions needed to ensure thread-safety
FE_Mesh* FE_Mesh::setPos(FE_Vec4 posa){ this->internalSetPos(posa); return this;}
FE_Mesh* FE_Mesh::setRot(FE_Vec4 posa){ this->internalSetRot(posa); return this;}
FE_Mesh* FE_Mesh::setRotEuler(FE_Vec4 axis_angle){ this->internalSetRot(FE_Quat(axis_angle)); return this;}
FE_Mesh* FE_Mesh::setScale(FE_Vec4 posa){ this->internalSetScale(posa); return this;}

FE_Mesh* FE_Mesh::setX(float posa){ this->internalSetX(posa); return this;}
FE_Mesh* FE_Mesh::setY(float posa){ this->internalSetY(posa); return this;}
FE_Mesh* FE_Mesh::setZ(float posa){ this->internalSetZ(posa); return this;}

FE_Mesh* FE_Mesh::setRotX(float posa){ this->internalSetRotX(posa); return this;}
FE_Mesh* FE_Mesh::setRotY(float posa){ this->internalSetRotY(posa); return this;}
FE_Mesh* FE_Mesh::setRotZ(float posa){ this->internalSetRotZ(posa); return this;}

FE_Mesh* FE_Mesh::setScaleX(float posa){ this->internalSetScaleX(posa); return this;}
FE_Mesh* FE_Mesh::setScaleY(float posa){ this->internalSetScaleY(posa); return this;}
FE_Mesh* FE_Mesh::setScaleZ(float posa){ this->internalSetScaleZ(posa); return this;}
// *********


