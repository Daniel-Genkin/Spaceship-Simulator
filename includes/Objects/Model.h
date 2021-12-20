#ifndef MODEL_H
#define MODEL_H

#include <functional>
#include <vector>
#include "Mesh.h"
#include "Math/Vector3.h"
#include "Interfaces/Renderable.h"
#include "Interfaces/GameObject.h"
#include <iostream>

class Model : public Renderable, public GameObject
{
public:
    vector<Mesh> meshes;

    void render() override;

    // prep for the model
    void init() override;

    // unused but required
    Model() {}
    void animate(float s) override {}
};

#endif
