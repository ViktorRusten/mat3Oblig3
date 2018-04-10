#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "sceneobject.h"
#include "vec3.h"

class Shader;

class BezierCurve : public SceneObject, protected QOpenGLFunctions_4_1_Core
{
public:
    BezierCurve();
    BezierCurve(RenderWindow* Owner, Vec3 Position, Quaternion Rotation, Vec3 colors, Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4);
    ~BezierCurve();

    void createObject() override;
    void drawObject() override;

    Vec3 mColors = (red, green, blue);

    Vec3 evaluateBezier(int degree, float t);
    void bezierPoints(Vertex vtx[11]);
    Vec3 b[4];

private:
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;
    GLfloat blue = 1.f;
    GLfloat red = 0.f;
    GLfloat green = 0.f;
    Vec3 mP1;
    Vec3 mP2;
    Vec3 mP3;
    Vec3 mP4;

    Shader* mShader;
};

#endif // BEZIERCURVE_H
