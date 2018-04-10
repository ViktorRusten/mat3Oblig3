#include "plane.h"
#include "shader.h"
#include "renderwindow.h"
#include "camera.h"
#include "vertex.h"
#include "constants.h"
#include "transform.h"
#include <math.h>
#include <vector>

Plane::Plane()
{

}

Plane::Plane(RenderWindow *Owner, Vec3 Position, Quaternion Rotation, Vec3 colors)
    : SceneObject(Owner), mColors{colors}
{
    transform.setPosition(Position);
    transform.setRotation(Rotation);

    xLength = mXMax - mXMin;
    zLength = mZMax - mZMin;
    nrOfX = ceil(xLength/mInterval);
    nrOfZ = ceil(zLength/mInterval);

    if(nrOfX == floor(xLength/mInterval))
        nrOfX++;

    if(nrOfZ == floor(zLength/mInterval))
        nrOfZ++;

    mNumberOfVertices = calculateNrVerticies();
    mVertices = new Vertex[mNumberOfVertices];
    std::cout << mNumberOfVertices << std::endl;

    calculatePoints();
    for(int i = 0; i < mNumberOfVertices; i++)
    {
        mVertices[i] = {Vec3(index[i]->getX(), index[i]->getY(), index[i]->getZ()), calculateNormal(index[i]->getX(),index[i]->getZ()), Vec2()};
    }
}

Plane::Plane(RenderWindow *Owner, Vec3 Position, Quaternion Rotation, Vec3 colors,
             GLfloat zmin, GLfloat zmax, GLfloat xmin, GLfloat xmax, GLfloat interval)
    : SceneObject(Owner), mColors{colors}, mZMin{zmin},mZMax{zmax},mXMin{xmin},mXMax{xmax}, mInterval{interval}
{
    transform.setPosition(Position);
    transform.setRotation(Rotation);

    xLength = mXMax - mXMin;
    zLength = mZMax - mZMin;
    nrOfX = ceil(xLength/mInterval);
    nrOfZ = ceil(zLength/mInterval);

    if(nrOfX == floor(xLength/mInterval))
        nrOfX++;

    if(nrOfZ == floor(zLength/mInterval))
        nrOfZ++;

    mNumberOfVertices = calculateNrVerticies();
    mVertices = new Vertex[mNumberOfVertices];

    calculatePoints();

    for(int i = 0; i < mNumberOfVertices; i++)
    {
        mVertices[i] = {Vec3(index[i]->getX(), index[i]->getY(), index[i]->getZ()), calculateNormal(index[i]->getX(),index[i]->getZ()), Vec2()};
    }
}

Plane::~Plane()
{
    delete mVertices;
}

void Plane::createObject()
{
    initializeOpenGLFunctions();

    std::string vertexPath = gsl::shaderFilePath + "plainvertex.vert";
    std::string fragPath = gsl::shaderFilePath + "plainfragment.frag";
    mShader = new Shader(vertexPath.c_str(),
                         fragPath.c_str());

    // Gen VAO
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    // Gen Buffer Object
    glGenBuffers(1, &mVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER,
                 mNumberOfVertices*sizeof(Vertex),
                 vertices(),
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

void Plane::drawObject()
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

    glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);

    mOwner->checkForGLerrors();
}

GLfloat Plane::findY(GLfloat x, GLfloat z)
{
    //return 0;
    return sin(x) + cos(z);
}

void Plane::calculatePoints()
{

    for(int j = 0; j < nrOfZ; j++)
    {
        if(j == nrOfZ-1){
            for(int i = 0; i < nrOfX; i++)
            {
                if(i == nrOfX-1){
                    yFloat = findY(mXMax, mZMax);
                    trianglePosition.push_back(static_cast<Vec3*>(
                                                   new Vec3(mXMax, yFloat, mZMax)));
                } else {
                    yFloat = findY(mXMin + (i * mInterval), mZMax);
                    trianglePosition.push_back(static_cast<Vec3*>(
                                                   new Vec3(mXMin + (i * mInterval), yFloat, mZMax)));
                }
            }
        } else {
            for(int i = 0; i < nrOfX; i++)
            {
                if(i == nrOfX-1){
                    yFloat = findY(mXMax, mZMin +(j * mInterval));
                    trianglePosition.push_back(static_cast<Vec3*>(
                                                   new Vec3(mXMax, yFloat, mZMin +(j * mInterval))));
                } else {
                    yFloat = findY(mXMin + (i * mInterval), mZMin +(j * mInterval));
                    trianglePosition.push_back(static_cast<Vec3*>(
                                                   new Vec3(mXMin + (i * mInterval), yFloat, mZMin +(j * mInterval))));
                }
            }
        }
    }

    int triCounter = 0;

    for(int j = 0; j < nrOfZ-1; j++)
    {
        for(int i = 0; i < nrOfX-1; i++)
        {
            //bottom left
            index.push_back(static_cast<Vec3*>(trianglePosition[i+(j*nrOfX)]));
            triPos.push_back(static_cast<int>(triCounter));

            //bottom right
            index.push_back(static_cast<Vec3*>(trianglePosition[i+(j*nrOfX)+1]));
            triPos.push_back(static_cast<int>(triCounter+1));

            //top left
            index.push_back(static_cast<Vec3*>(trianglePosition[i+(j*nrOfX)+nrOfX]));
            triPos.push_back(static_cast<int>(triCounter+2));

            //top left
            index.push_back(static_cast<Vec3*>(trianglePosition[i+(j*nrOfX)+nrOfX]));
            triPos.push_back(static_cast<int>(triCounter+3));

            //bottom right
            index.push_back(static_cast<Vec3*>(trianglePosition[i+(j*nrOfX)+1]));
            triPos.push_back(static_cast<int>(triCounter+4));

            //top right
            index.push_back(static_cast<Vec3*>(trianglePosition[i+(j*nrOfX)+nrOfX+1]));
            triPos.push_back(static_cast<int>(triCounter+5));

            triCounter += 6;
        }
    }
}

GLfloat Plane::derivedX(GLfloat x)
{
    GLfloat sx = sin(0.1*x);
    GLfloat cx = cos(0.1*x);

    return 7*2*0.1*sx*cx;
}

GLfloat Plane::derivedZ(GLfloat z)
{
    GLfloat sz = sin(0.1*z);
    GLfloat cz = cos(0.1*z);

    return -7*2*0.1*sz*cz;
}

Vec3 Plane::calculateNormal(GLfloat x, GLfloat z)
{

    // z = f(x, z) + (df/dx)(x - x0) + (df/dz)(z - z0)
    GLfloat cx = derivedX(x);
    GLfloat cz = derivedZ(z);

    GLfloat cd = findY(x, z) - cx*x - cz*z;

    Vec3 P0 = Vec3(x,     cx*x       + cz*z       + cd, z);
    Vec3 P1 = Vec3(x+1.f, cx*(x+1.f) + cz*z       + cd, z);
    Vec3 P2 = Vec3(x,     cx*x       + cz*(z+1.f) + cd, z + 1.f);

    Vec3 v1 = P1 - P0;
    Vec3 v2 = P2 - P0;

    Vec3 normal = (v1^v2).normalized();

    return normal;
}

int Plane::calculateNrVerticies()
{

    return (((nrOfX-1)*(nrOfZ-1))*6);
}



int Plane::getTriangle(Vec3 point)
{

    int triangle = 0;

    Vec2 find2d = Vec2(point.getX(), point.getZ());
    Vec3 bCoords;

    Vec2 v1;
    Vec2 v2;
    Vec2 v3;

    do {

        v1 = Vec2(mVertices[triPos[triangle]].get_x(),mVertices[triPos[triangle]].get_z());
        v2 = Vec2(mVertices[triPos[triangle+1]].get_x(),mVertices[triPos[triangle+1]].get_z());
        v3 = Vec2(mVertices[triPos[triangle+2]].get_x(),mVertices[triPos[triangle+2]].get_z());

        bCoords = find2d.barycentricCoordinates(v1, v2, v3);

        // check if we are inside the triangle we want
        if(bCoords.getX() >= 0 && bCoords.getY() >= 0 && bCoords.getZ() >= 0)
        {
            std::cout << "found it!" << std::endl;
            return triangle/3;
        }

        if(triangle % 2 == 0)
        {
            if(bCoords.getX() < 0 && bCoords.getY() >= 0 && bCoords.getZ() >= 0){
                triangle += 3;
            }
            else if(bCoords.getY() < 0 && bCoords.getX() >= 0 && bCoords.getZ() >= 0){
                triangle -= 3;}
            else if(bCoords.getZ() < 0 && bCoords.getX() >= 0 && bCoords.getY() >= 0){
                triangle -= (nrOfX-2)*6;}
            else
            {
                if( bCoords.getZ() < bCoords.getY()){
                    triangle -= (nrOfX-2)*6;}
                else{
                    triangle -= 3;}
            }
        }
        else
        {
            if(bCoords.getY() < 0 && bCoords.getZ() >= 0 && bCoords.getX() >= 0){
                triangle += ((nrOfX-2)*6)+3;}
            else if(bCoords.getX() < 0 && bCoords.getY() >= 0  && bCoords.getZ() >= 0)
            {
                triangle += 3;
            }
            else if(bCoords.getZ() < 0 && bCoords.getX() >= 0 && bCoords.getY() >= 0){
                triangle -= 3;}
            else if(bCoords.getX() < 0 && bCoords.getY() < 0 && bCoords.getZ() >= 0){
                triangle += ((nrOfX-2)*6)+3;}
            else if(bCoords.getX() < 0 && bCoords.getZ() < 0 && bCoords.getY() >= 0){
                triangle += 3;}
            else
            {
                if(bCoords.getY() < bCoords.getZ()){
                    triangle -= 3;}
                else{
                    triangle += ((nrOfX-2)*6)+3;
                    }
            }

        }
        if((triangle < 0) || (triangle >= (((nrOfX-1)*(nrOfZ-1))*2*6)-1)){
            triangle = -1;
        }
    }while (triangle > 0);
    std::cout << "the point is not on the plane" << std::endl;
    return -1;
}
