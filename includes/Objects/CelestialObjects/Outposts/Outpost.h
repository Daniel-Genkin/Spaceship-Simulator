#ifndef OUTPOST_H
#define OUTPOST_H

#include "Interfaces/GameObject.h"
#include "Math/Vector3.h"
#include "Objects/ObjectLoader/Loader.h"
#include "Objects/Model.h"

class Outpost : public GameObject, public Renderable
{
private:
    float orbitSpeed;
    float orbitRadius;
    Model outpost;
    void animate(float s) override{};

public:
    Outpost() {}

    Outpost(Vector3 planetPosition, float orbitRadius, Loader &loader);

    void animate(float s, Vector3 planetPosition);
    void render() override;

    void init() override {}
};

#endif /* OUTPOST_H */
