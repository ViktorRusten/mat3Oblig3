#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QOpenGLFunctions_4_1_Core>
#include "matrix4x4.h"
#include "transform.h"
#include <memory>

class Vertex;
class Matrix4x4;
class RenderWindow;

class SceneObject
{
public:
    SceneObject();
    SceneObject(RenderWindow* Owner);
    ~SceneObject();

    // = 0 because we have to override in children
    virtual void createObject() = 0;
    virtual void drawObject() = 0;

    virtual void update(float deltaTime){}

    Matrix4x4 getModelMatrix(){ return transform.getModelMatrix(); }

    Transform& getTransform();

    int mNumberOfVertices;
    Matrix4x4 mMVPMatrix;
    Vertex *vertices() const;

    RenderWindow* mOwner{nullptr};
protected:
    Vertex* mVertices;
    Transform transform;
};

#endif // SCENEOBJECT_H
