#ifndef PLANETMODELFACTORY_H
#define PLANETMODELFACTORY_H

#include <map>
#include <vector>

#include "PlanetSettings.h"
#include "PlanetModel.h"
#include "Objects/Mesh.h"
#include "Objects/Model.h"
#include "Objects/Material.h"
#include "Math/Vector3.h"

class PlanetModelFactory
{
private:
    PlanetSettings settings;
    int resolution;
    map<Vector3, Mesh> sides;
    vector<Vector3> directions;

    Vector3 getCubePoint(int x, int y, Vector3 &bottomLeft, Vector3 &xAxis, Vector3 &yAxis);

    Mesh createMesh(int s, Material mat, float &minElev, float &maxElev);
    void createUVs(Mesh &mesh, float &minElev, float &maxElev);
    void createModel();

public:
    PlanetModel model;

    PlanetModelFactory(PlanetSettings settings, int resolution);
};

#endif /* PLANETMODEL_H */
