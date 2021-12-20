#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Math/Vector3.h"

class GameObject
{
public:
    Vector3 position = Vector3(0, 0, 0);
    Vector3 rotation = Vector3(0, 0, 0);
    Vector3 scale = Vector3(1, 1, 1);

    virtual ~GameObject() = default;
};

#endif
