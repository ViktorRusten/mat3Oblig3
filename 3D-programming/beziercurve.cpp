#include "beziercurve.h"
#include "plane.h"
#include "shader.h"
#include "renderwindow.h"
#include "camera.h"
#include "vertex.h"
#include "constants.h"
#include "transform.h"
#include <vector>

BezierCurve::BezierCurve()
{

}

BezierCurve::BezierCurve(RenderWindow *Owner, Vec3 Position, Quaternion Rotation, Vec3 colors, Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4)
    : SceneObject(Owner), mColors{colors}, mP1{p1},mP2{p2},mP3{p3},mP4{p4}
{
    transform.setPosition(Position);
    transform.setRotation(Rotation);

    mNumberOfVertices = 4;
    mVertices = new Vertex[mNumberOfVertices]
    {
        {p1, colors, Vec2()},
        {p2, colors, Vec2()},
        {p3, colors, Vec2()},
        {p4, colors, Vec2()}
    };
}

BezierCurve::~BezierCurve()
{
    delete[] mVertices;
}

void BezierCurve::createObject()
{
    initializeOpenGLFunctions();

    std::string vertexPath = gsl::shaderFilePath + "plainvertex.vert";
    std::string fragPath = gsl::shaderFilePath + "plainfragment.frag";
    mShader = new Shader(vertexPath.c_str(),
                         fragPath.c_str());


    Vertex vtx[11];
    bezierPoints(vtx);

    // Gen VAO
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    // Gen Buffer Object
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 11*sizeof(Vertex),
                 vtx,
                 GL_STATIC_DRAW);

    // Tell our GPU how to read positions
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Tell our GPU how to read colors
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Tell our GPU how to read uv's
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          8*sizeof(GLfloat),
                          (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    mMVPMatrixUniform = glGetUniformLocation(mShader->getProgram(), "matrix");
}

void BezierCurve::drawObject()
{
    // Tell the GPU to use a shader
    glUseProgram(mShader->getProgram());

    // Read our vertex data
    glBindVertexArray(mVertexArrayObject);

    Matrix4x4 Projection = mOwner->getCamera()->getPerspectiveMatrix();
    Matrix4x4 View = mOwner->getCamera()->getViewMatrix();
    Matrix4x4 Model = getModelMatrix();

    mMVPMatrix = Projection * View * Model;

    // Load the matrix into the shader
    glUniformMatrix4fv(mMVPMatrixUniform, 1, GL_TRUE, mMVPMatrix.constData());

    glDrawArrays(GL_LINE_STRIP, 0, 11);

    mOwner->checkForGLerrors();
}

Vec3 BezierCurve::evaluateBezier(int degree, float t)
{
    Vec3 c[4];
    for(int i = 0; i<4; i++)
        c[i] = mVertices[i].get_xyz();

    for(int k=1; k<=degree; k++)
    {
        for(int i=0; i<degree-k+1; i++)
            c[i] = c[i] * (1-t) + c[i+1] * t;
    }
    return c[0];
}

void BezierCurve::bezierPoints(Vertex vtx[])
{
    for(auto i=0; i<11; i++)
    {
        float t = float(i)/10;
        Vec3 x = evaluateBezier(3,t);
        vtx[i].set_xyz(x);
    }
}
