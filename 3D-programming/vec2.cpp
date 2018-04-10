#include "vec2.h"

Vec2::Vec2(GLfloat x_in, GLfloat y_in) : x{x_in}, y{y_in} { }

const Vec2 &Vec2::operator =(const Vec2 &v)
{
    x = v.x;
    y = v.y;
    return *this;
}

Vec2 Vec2::operator +(const Vec2 &v) const
{
    Vec2 u;
    u.x = x + v.x;
    u.y = y + v.y;
    return u;
}

Vec2 Vec2::operator -(const Vec2 &v) const
{
    Vec2 u;
    u.x = x - v.x;
    u.y = y - v.y;
    return u;
}

Vec3 Vec2::operator ^(const Vec2 &v) const
{
    return Vec3(getX(), 0.f, getY())^Vec3(v.getX(), 0.f, v.getY());
}

Vec3 Vec2::barycentricCoordinates(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3)
{
    Vec2 p12 = p2-p1;
    Vec2 p13 = p3-p1;

    Vec3 n = p12^p13;

    float areal_123 = n.length(); // dobbelt areal

    Vec3 baryc; // til retur. Husk u

    Vec2 p = p2 - *this;

    Vec2 q = p3 - *this;

    n = p^q;

    baryc.setX(n.getY()/areal_123);

    // v
    p = p3 - *this;
    q = p1 - *this;

    n = p^q;

    baryc.setY(n.getY() / areal_123);

    // w
    p = p1 - *this;
    q = p2 - *this;
    n = p^q;

    baryc.setZ(n.getY() / areal_123);

    return baryc*-1;
}

GLfloat Vec2::getX() const
{
    return x;
}

void Vec2::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vec2::getY() const
{
    return y;
}

void Vec2::setY(const GLfloat &value)
{
    y = value;
}
