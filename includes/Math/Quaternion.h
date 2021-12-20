#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vector3.h"

class Quaternion
{
public:
    float x, y, z, w;
    Quaternion(float x, float y, float z, float w = 1.0f);
    Quaternion();
    ~Quaternion();
    Quaternion normalize();
    Quaternion operator*(Quaternion q);
    Quaternion conj(Quaternion q);
    Vector3 applyRotation(Vector3 point);
    Vector3 toEulerAngles();
    Vector3 toVector();
    float *toMatrix();

    /// ANGLE IS IN RADIANS!!!!!
    static Quaternion rotationQuaternion(Vector3 axis, float angle);
};

#endif