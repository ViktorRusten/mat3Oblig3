#include "vec3.h"
#include "matrix4x4.h"
#include <cmath>

#define M_PI 3.14159265358979323846     //Visual studio

Vec3::Vec3(GLfloat x_in, GLfloat y_in, GLfloat z_in)
    : x{x_in}, y{y_in}, z{z_in}
{
}

const Vec3 &Vec3::operator =(const Vec3 &v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

Vec3 Vec3::operator +(const Vec3 &v) const
{
    Vec3 u;
    u.x = x + v.x;
    u.y = y + v.y;
    u.z = z + v.z;
    return u;
}

Vec3 Vec3::operator -(const Vec3 &v) const
{
    Vec3 u;
    u.x = x - v.x;
    u.y = y - v.y;
    u.z = z - v.z;
    return u;
}

Vec3 Vec3::operator ^(const Vec3 &v) const
{
    Vec3 w;
    w.x =  y*v.z - z*v.y;
    w.y =  z*v.x - x*v.z;
    w.z =  x*v.y - y*v.x;
    return w;
}

float Vec3::vecLength(Vec3 otherPoint)
{
    return
    sqrt(pow((x*otherPoint.getX()),2) +
         pow((y*otherPoint.getY()),2) +
         pow((z*otherPoint.getZ()),2));
}

GLfloat Vec3::areal(GLfloat a, GLfloat b, GLfloat c)
{
    GLfloat s = (a+b+c)/2;
    GLfloat Q = sqrt(s * (s-a) * (s-b) * (s-c));
    return Q;
}

void Vec3::normalize()
{
    GLfloat d = x*x+y*y+z*z;
    d = sqrt(d);
    if (d>0.0)
    {
        x=x/d;
        y=y/d;
        z=z/d;
    }
}

Vec3 Vec3::normalized()
{
    Vec3 normalized;
    GLfloat d = x*x+y*y+z*z;
    d = sqrt(d);
    if (d>0.0)
    {
        normalized.x=x/d;
        normalized.y=y/d;
        normalized.z=z/d;
    }
    return normalized;
}

void Vec3::rotateX(double angle)
{
    Vec3 dir;
    angle = deg2Rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x;
    dir.y = y * cos(angle) + z * -sin(angle);
    dir.z = y * sin(angle) + z * cos(angle) ;

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

void Vec3::rotateY(double angle)
{
    Vec3 dir;
    angle = deg2Rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x * cos(angle) + z * sin(angle);
    dir.y = y;
    dir.z = x * -sin(angle) + z * cos(angle);

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

void Vec3::rotateZ(double angle)
{
    Vec3 dir;
    angle = deg2Rad(angle);
    //qDebug() << "Angle: " << angle;
    dir.x = x * cos(angle) + y * -sin(angle);
    dir.y = x * sin(angle) + y * cos(angle);
    dir.z = z;

    x = dir.x;
    y = dir.y;
    z = dir.z;
}

Vec3 Vec3::operator *(Matrix4x4 q) const
{
    q = q;

    return *this;
}

bool Vec3::barycentriccoordinates(Vec3 a, Vec3 b, Vec3 c)
{
    Vec3 point = *this;
    bool funnet;

    std::cout << point << std::endl;

    std::cout << "\n" << a << "\n" << b<< "\n" << c<< std::endl;

    GLfloat ab = a.vecLength(b);
    GLfloat bc = b.vecLength(c);
    GLfloat ac = a.vecLength(c);

    std::cout << "\n" << ab << "\n" << bc << "\n" << ac << std::endl;

    GLfloat ap = a.vecLength(point);
    GLfloat bp = b.vecLength(point);
    GLfloat cp = c.vecLength(point);

    std::cout << "\n" << ap << "\n" << bp << "\n" << ap << std::endl;

    GLfloat triArea = areal(ab, ac, bc);
    GLfloat areaab = areal(ab, ap, bp);
    GLfloat areabc = areal(bc, bp, cp);
    GLfloat areaac = areal(ac, ap, cp);

    std::cout << "\n" << triArea << "\n" << areaab << "\n" << areabc <<  "\n" << areaac <<std::endl;

    GLfloat sum = areaab+areaac+areabc - triArea;

    std::cout << sum << std::endl;

    if(sum == 0.f)
        return funnet = true;
    else
        return funnet = false;
}

float Vec3::length()
{
    return sqrt(x*x+y*y+z*z);
}

double Vec3::rad2Deg(double &rad)
{
    return (rad * 180)/M_PI;    //M_PI is in cmath
}

double Vec3::deg2Rad(double &deg)
{
    return (deg * M_PI)/180;    //M_PI is in cmath
}

GLfloat Vec3::getX() const
{
    return x;
}

void Vec3::setX(const GLfloat &value)
{
    x = value;
}

GLfloat Vec3::getY() const
{
    return y;
}

void Vec3::setY(const GLfloat &value)
{
    y = value;
}

GLfloat Vec3::getZ() const
{
    return z;
}

void Vec3::setZ(const GLfloat &value)
{
    z = value;
}

Vec3 Vec3::operator *(float c) const
{
    return Vec3(x*c, y*c, z*c);
}

float Vec3::operator *(const Vec3 &v) const
{
    return x*v.x + y*v.y + z*v.z;
}
