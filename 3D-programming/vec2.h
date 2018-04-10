#ifndef VEC2_H
#define VEC2_H

#include <QtGui/qopengl.h> // for GLfloat
#include <cmath>
#include "matrix4x4.h"

class Vec2
{
public:
    Vec2(GLfloat x_in=0.0, GLfloat y_in=0.0);

    const Vec2 &operator = (const Vec2& v);

    Vec2 operator + (const Vec2& v) const;
    Vec2 operator - (const Vec2& v) const;

        Vec3 operator ^ (const Vec2 &v) const;  // Cross product

    Vec3 barycentricCoordinates(const Vec2& p1, const Vec2& p2, const Vec2& p3);

    GLfloat getX() const;
    void setX(const GLfloat &value);

    GLfloat getY() const;
    void setY(const GLfloat &value);

private:
    GLfloat x;
    GLfloat y;
};

#endif // VEC2_H
