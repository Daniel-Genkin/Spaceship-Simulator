#ifndef MESH_H
#define MESH_H

#include <vector>
#include "Material.h"

class Mesh
{
public:
    vector<unsigned int> vertexIndices = vector<unsigned int>();
    vector<float> vertices = vector<float>();
    vector<float> normals = vector<float>();
    vector<float> textureCoords = vector<float>();

    Material material;

    Mesh() {}
};

#endif
