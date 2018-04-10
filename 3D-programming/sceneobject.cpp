#include "sceneobject.h"
#include "vertex.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(RenderWindow *Owner)
    : mOwner{Owner}
{

}

SceneObject::~SceneObject()
{
    //std::cout << "SceneObject deleted\n";
}

Transform& SceneObject::getTransform()
{
    return transform;
}

Vertex *SceneObject::vertices() const
{
    return mVertices;
}

