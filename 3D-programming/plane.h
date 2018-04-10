#ifndef PLANE_H
#define PLANE_H

#include "sceneobject.h"
#include "vec3.h"

class Shader;

class Plane : public SceneObject, protected QOpenGLFunctions_4_1_Core
{
public:
    Plane();

    Plane(RenderWindow* Owner, Vec3 Position, Quaternion Rotation, Vec3 colors);
    Plane(RenderWindow* Owner, Vec3 Position, Quaternion Rotation, Vec3 colors,
          GLfloat zmin,GLfloat zmax, GLfloat xmin, GLfloat xmax, GLfloat interval);
    ~Plane();

    void createObject() override;
    void drawObject() override;
    void calculatePoints();
    GLfloat derivedX(GLfloat x);
    GLfloat derivedZ(GLfloat z);
    Vec3 calculateNormal(GLfloat x, GLfloat z);

    Vec3 mColors = (red, green, blue);

    std::vector<Vec3*> trianglePosition;
    std::vector<Vec3*> index;
    std::vector<int> triPos;

    int calculateNrVerticies();
    GLfloat findY(GLfloat x, GLfloat z);
    int getTriangle(Vec3 point);




    GLfloat mZMin = -2;
    GLfloat mZMax = 2;
    GLfloat mXMin = -2;
    GLfloat mXMax = 2;
    GLfloat mInterval = 1;
    GLfloat xLength;
    GLfloat zLength;
    int nrOfX;
    int nrOfZ;

    GLfloat yFloat = 0.f;
    GLfloat xFloat = 0.f;
    GLfloat zFloat = 0.f;

private:
    GLuint mVertexArrayObject;
    GLuint mVertexBufferObject;
    GLuint mMVPMatrixUniform;
    GLfloat blue = 0.f;
    GLfloat red = 0.f;
    GLfloat green = 0.f;

    Shader* mShader;
};

#endif // PLANE_H
