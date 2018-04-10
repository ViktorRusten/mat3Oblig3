#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "sceneobject.h"
#include "quaternion.h"

class Vertex;
class Matrix4x4;
class Shader;

class Triangle : public SceneObject, protected QOpenGLFunctions_4_1_Core
{
public:
    Triangle(RenderWindow* Owner);
    ~Triangle();

    void createObject() override;
    void drawObject() override;

    void update(float deltaTime) override;
private:
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;

    Shader* mShader;
};

#endif // TRIANGLE_H
