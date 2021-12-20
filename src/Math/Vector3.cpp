// Include header file for syntax
#include "Math/Vector3.h"

#include "Math/Math.h"
// include standard libraries needed
#include <cmath>

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
};
Vector3::Vector3(float *v)
{
    this->x = v[0];
    this->y = v[1];
    this->z = v[2];
};
Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
};
Vector3::~Vector3() {}
float Vector3::length()
{
    return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
};
Vector3 Vector3::normalize()
{
    float length = this->length();
    if (length == 0)
    {
        return Vector3();
    }
    return Vector3(this->x / length, this->y / length, this->z / length);
};
float Vector3::distanceTo(Vector3 v)
{
    return sqrt(pow(this->x - v.x, 2) + pow(this->y - v.y, 2) + pow(this->z - v.z, 2));
};
float Vector3::fastDistanceTo(Vector3 v)
{
    return pow(this->x - v.x, 2) + pow(this->y - v.y, 2) + pow(this->z - v.z, 2);
};
Vector3 Vector3::cross(Vector3 v)
{
    return Vector3(this->y * v.z - this->z * v.y,
                   this->z * v.x - this->x * v.z,
                   this->x * v.y - this->y * v.x);
};
float Vector3::dot(Vector3 v)
{
    return this->x * v.x + this->y * v.y + this->z * v.z;
};
Vector3 Vector3::operator+(Vector3 v)
{
    return Vector3(this->x + v.x, this->y + v.y, this->z + v.z);
};
Vector3 Vector3::operator+(float v)
{
    return Vector3(this->x + v, this->y + v, this->z + v);
};
Vector3 Vector3::operator-(Vector3 v)
{
    return Vector3(this->x - v.x, this->y - v.y, this->z - v.z);
};
Vector3 Vector3::operator-(float v)
{
    return Vector3(this->x - v, this->y - v, this->z - v);
};
Vector3 Vector3::operator*(Vector3 v)
{
    return Vector3(this->x * v.x, this->y * v.y, this->z * v.z);
};
Vector3 Vector3::operator*(float f)
{
    return Vector3(this->x * f, this->y * f, this->z * f);
};
Vector3 Vector3::operator/(Vector3 v)
{
    return Vector3(this->x / v.x, this->y / v.y, this->z / v.z);
};
Vector3 Vector3::operator/(float f)
{
    return Vector3(this->x / f, this->y / f, this->z / f);
};
bool Vector3::operator==(Vector3 v)
{
    return this->x == v.x && this->y == v.y && this->z == v.z;
};
bool Vector3::operator!=(Vector3 v)
{
    return this->x != v.x || this->y != v.y || this->z != v.z;
};
float Vector3::operator[](int i)
{
    switch (i)
    {
    case 0:
        return this->x;
        break;
    case 1:
        return this->y;
        break;
    case 2:
        return this->z;
        break;
    default:
        return 0;
    }
};

// needed for map to work apparently
bool Vector3::operator<(const Vector3 v) const
{
    float l1 = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
    float l2 = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
    return l1 < l2;
}

Vector3 Vector3::rotate(Vector3 axis, float angle)
{
    // Rotate this vector around the given axis by the given angle
    Vector3 Q[3];

    angle = toRadians(angle);

    Q[0] = Vector3(
        pow(axis.x, 2) * (1 - cos(angle)) + cos(angle),
        axis.x * axis.y * (1 - cos(angle)) + axis.z * sin(angle),
        axis.x * axis.z * (1 - cos(angle)) - axis.y * sin(angle));
    Q[1] = Vector3(
        axis.x * axis.y * (1 - cos(angle)) - axis.z * sin(angle),
        pow(axis.y, 2) * (1 - cos(angle)) + cos(angle),
        axis.y * axis.z * (1 - cos(angle)) + axis.x * sin(angle));
    Q[2] = Vector3(
        axis.x * axis.z * (1 - cos(angle)) + axis.y * sin(angle),
        axis.y * axis.z * (1 - cos(angle)) - axis.x * sin(angle),
        pow(axis.z, 2) * (1 - cos(angle)) + cos(angle));

    Vector3 result;
    result.x = this->dot(Q[0]);
    result.y = this->dot(Q[1]);
    result.z = this->dot(Q[2]);

    return result.normalize();
};
float Vector3::angleTo(Vector3 v)
{
    float angle = acos(this->dot(v) / (this->length() * v.length()));
    return toRadians(angle);
};