#include "Math/Quaternion.h"
#include <cmath>

Quaternion::Quaternion() {}
Quaternion::~Quaternion() {}

Quaternion::Quaternion(float x, float y, float z, float w)
{
    this->w = w;
    this->x = x; // i
    this->y = y; // j
    this->z = z; // k
}

Quaternion Quaternion::conj(Quaternion q)
{
    return Quaternion(-q.x, -q.y, -q.z, q.w);
}

Quaternion Quaternion::normalize()
{
    float length = sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2) + pow(this->w, 2));
    return Quaternion(-this->x / length, -this->y / length, -this->z / length, this->w / length);
}

Quaternion Quaternion::rotationQuaternion(Vector3 axis, float angle)
{
    float halfAngle = angle / 2;
    float s = sin(halfAngle);

    return Quaternion(
        s * axis.x,
        s * axis.y,
        s * axis.z,
        cos(halfAngle));
}

Quaternion Quaternion::operator*(Quaternion a)
{ // a * this
    return Quaternion(
        a.w * this->x + a.x * this->w + a.y * this->z - a.z * this->y,
        a.w * this->y + a.y * this->w + a.z * this->x - a.x * this->z,
        a.w * this->z + a.z * this->w + a.x * this->y - a.y * this->x,
        a.w * this->w - a.x * this->x - a.y * this->y - a.z * this->z);
}

Vector3 Quaternion::toEulerAngles()
{
    return Vector3(
        std::atan2(2 * (this->w * this->x + this->y * this->z), 1 - 2 * (this->x * this->x + this->y * this->y)),
        std::asin(2 * (this->w * this->y - this->z * this->x)),
        std::atan2(2 * (this->w * this->z + this->x * this->y), 1 - 2 * (this->y * this->y + this->z * this->z)));
}

Vector3 Quaternion::toVector()
{
    return Vector3(this->x, this->y, this->z);
}

Vector3 Quaternion::applyRotation(Vector3 point)
{
    Vector3 v = Vector3(this->x, this->y, this->z);
    Vector3 t = v.cross(point) * 2.0f;
    return point + t * this->w + v.cross(t);
}

// convert rotation quaternion into matrix (thanks Dr. Brett for the link https://paroj.github.io/gltut/Positioning/Tut08%20Quaternions.html)
float *Quaternion::toMatrix()
{
    float xx = 2.0f * this->x;
    float yy = 2.0f * this->y;
    float zz = 2.0f * this->z;

    return new float[16]{
        1.0f - yy * this->y - zz * this->z, xx * this->y - zz * this->w, xx * this->z + yy * this->w, 0.0f,
        xx * this->y + zz * this->w, 1.0f - xx * this->x - zz * this->z, yy * this->z - xx * this->w, 0.0f,
        xx * this->z - yy * this->w, yy * this->z + xx * this->w, 1.0f - xx * this->x - yy * this->y, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
}