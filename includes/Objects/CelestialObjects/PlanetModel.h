#ifndef PLANETMODEL_H
#define PLANETMODEL_H

#include "Math/Vector3.h"
#include "Objects/Model.h"
#include "Outposts/Outpost.h"
#include <vector>

class PlanetModel : public Model
{
private:
    float orbitSpeed;

public:
    float radius;
    vector<Outpost> outposts;

    PlanetModel();

    void init() override;
    void render() override;
    void animate(float s) override;
};

#endif
