#ifndef OCTAHEDRON_H
#define OCTAHEDRON_H

#include <string>
#include "sceneobject.h"
#include "quaternion.h"

class Vec3;
class Shader;

class Octahedron : public SceneObject, protected QOpenGLFunctions_4_1_Core
{
public:
    Octahedron();
    Octahedron(RenderWindow* Owner, int n = 0);
    ~Octahedron();

    void createObject() override;
    void drawObject() override;

    void update(float deltaTime) override;

    void applyForce(int direction);
    bool stop = false;

private:
    void makeTriangle(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
    void makeUnitOctahedron();

    int mRecursions;          //
    int mIndex;               // brukes i rekursjon, til å bygge m_vertices

    Shader* mShader;

    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;

    float mAngle{0.f};
};

#endif // OCTAHEDRON_H
