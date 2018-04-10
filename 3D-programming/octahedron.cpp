#include "octahedron.h"
#include "cmath"
#include "vertex.h"
#include "vec3.h"
#include "matrix4x4.h"
#include "renderwindow.h"
#include "shader.h"
#include "camera.h"
#include "constants.h"


Octahedron::Octahedron()
{

}

Octahedron::Octahedron(RenderWindow *Owner, int n)
    : SceneObject(Owner), mRecursions(n), mIndex(0)
{
    mNumberOfVertices = 3 * 8 * pow(4, mRecursions);
    mVertices = new Vertex[mNumberOfVertices];
    makeUnitOctahedron();
}

Octahedron::~Octahedron()
{
    delete[] mVertices;
}

void Octahedron::createObject()
{
    initializeOpenGLFunctions();

    std::string vertexPath = gsl::shaderFilePath + "plainvertex.vert";
    std::string fragPath = gsl::shaderFilePath + "plainfragment.frag";
    mShader = new Shader(vertexPath.c_str(),
                         fragPath.c_str());

    // Create buffers
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 mNumberOfVertices*sizeof(Vertex),
                 vertices(),
                 GL_STATIC_DRAW);

    // Tell the GPU how to load vertex positions
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell the GPU how to load colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8* sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Tell the GPU how to load uv's
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    mMVPMatrixUniform = glGetUniformLocation(mShader->getProgram(), "matrix");
}

void Octahedron::drawObject()
{
    // Fortell til openGL at den skal bruke denne shaderen
    glUseProgram(mShader->getProgram());

    // Fortell til openGL at vi vil bruke denne VAOen
    glBindVertexArray(mVertexArrayObject);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();
    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();
    Matrix4x4 Model = getModelMatrix();

    mMVPMatrix = Projection * View * Model;

    glUniformMatrix4fv(mMVPMatrixUniform, 1, GL_TRUE, mMVPMatrix.constData());

    glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);

    mOwner->checkForGLerrors();
}

void Octahedron::update(float deltaTime)
{
}

void Octahedron::applyForce(int direction)
{

    switch(direction)
    {
    case 1:
    {
        if(getTransform().getPosition().getZ() > -1.65f)
        {
            Vec3 forward = this->getTransform().getPosition();
            forward = forward + Vec3(0.f, 0.f, -0.05f);
            this->getTransform().setPosition(forward);
            break;
        }
    }
    case 2:
    {
        if(getTransform().getPosition().getX() > -1.65f)
        {
            Vec3 right = this->getTransform().getPosition();
            right = right + Vec3(-0.05f, 0.f, 0.f);
            this->getTransform().setPosition(right);
            break;
        }
    }
    case 3:
    {
        if(getTransform().getPosition().getZ() < 1.65f)
        {
            Vec3 backward = this->getTransform().getPosition();
            backward = backward + Vec3(0.f, 0.f, 0.05f);
            this->getTransform().setPosition(backward);
            break;
        }
    }
    case 4:
    {
        if(getTransform().getPosition().getX() < 1.65f)
        {
            Vec3 left = this->getTransform().getPosition();
            left = left + Vec3(0.05f, 0.f, 0.f);
            this->getTransform().setPosition(left);
            break;
        }
    }
    }
}

void Octahedron::makeTriangle(const Vec3 &v1, const Vec3 &v2, const Vec3 &v3)
{
    mVertices[mIndex].set_xyz(v1.getX(), v1.getY(), v1.getZ());
    mVertices[mIndex].set_normal(v1.getX(), v1.getY(), v1.getZ());
    mVertices[mIndex].set_st(0.0f, 0.0f);
    ++mIndex;
    mVertices[mIndex].set_xyz(v2.getX(), v2.getY(), v2.getZ());
    mVertices[mIndex].set_normal(v2.getX(), v2.getY(), v2.getZ());
    mVertices[mIndex].set_st(1.0f, 0.0f);
    ++mIndex;
    mVertices[mIndex].set_xyz(v3.getX(), v3.getY(), v3.getZ());
    mVertices[mIndex].set_normal(v3.getX(), v3.getY(), v3.getZ());
    mVertices[mIndex].set_st(0.5f, 1.0f);
    ++mIndex;
}

void Octahedron::subDivide(const Vec3 &a, const Vec3 &b, const Vec3 &c, int n)
{
    if (n>0) {
        Vec3 v1 = a+b; v1.normalize();
        Vec3 v2 = a+c; v2.normalize();
        Vec3 v3 = c+b; v3.normalize();
        subDivide(a, v1, v2, n-1);
        subDivide(c, v2, v3, n-1);
        subDivide(b, v3, v1, n-1);
        subDivide(v3, v2, v1, n-1);
    } else {
        makeTriangle(a, b, c);
    }
}

void Octahedron::makeUnitOctahedron()
{
    Vec3 v0(0, 0, 1);
    Vec3 v1(1, 0, 0);
    Vec3 v2(0, 1, 0);
    Vec3 v3(-1, 0, 0);
    Vec3 v4(0, -1, 0);
    Vec3 v5(0, 0, -1);

    subDivide(v0, v1, v2, mRecursions);
    subDivide(v0, v2, v3, mRecursions);
    subDivide(v0, v3, v4, mRecursions);
    subDivide(v0, v4, v1, mRecursions);
    subDivide(v5, v2, v1, mRecursions);
    subDivide(v5, v3, v2, mRecursions);
    subDivide(v5, v4, v3, mRecursions);
    subDivide(v5, v1, v4, mRecursions);
}


