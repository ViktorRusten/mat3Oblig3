#ifndef BALL_H
#define BALL_H

#include <string>
#include "sceneobject.h"
#include "quaternion.h"

class Vec3;
class Shader;

class Ball : public SceneObject, protected QOpenGLFunctions_4_1_Core
{
public:
    Ball();
    Ball(RenderWindow* Owner, int n = 0);
    ~Ball();

    void createObject() override;
    void drawObject() override;

    float speed = 0.03f + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.05f-0.03f)));
    void update(float deltaTime) override;

private:
    void makeTriangle(const Vec3& v1, const Vec3& v2, const Vec3& v3);
    void subDivide(const Vec3& a, const Vec3& b, const Vec3& c, int n);
    void makeUnitBall();

    int mRecursions;
    int mIndex;               // brukes i rekursjon, til å bygge m_vertices

    Shader* mShader;

    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;

    float mAngle{0.f};
};

#endif // BALL_H
