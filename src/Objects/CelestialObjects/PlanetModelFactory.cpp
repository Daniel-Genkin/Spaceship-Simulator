#include "Objects/CelestialObjects/PlanetModelFactory.h"

#include "Libs/FastNoiseLite.h"
#include "Math/Vector3.h"
#include "Objects/Colour.h"
#include "Objects/Material.h"

#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

// Inspired by Sebastian Lague's procedural planets tutorial.
// https://www.youtube.com/playlist?list=PLFt_AvWsXl0cONs3T0By4puYy6GM22ko8

PlanetModelFactory::PlanetModelFactory(PlanetSettings settings, int resolution)
{
    this->settings = settings;
    this->resolution = resolution;

    // This *should* be counter-clockwise around the sides, then the top and bottom.
    directions.push_back(Vector3(-1, 0, 0));
    directions.push_back(Vector3(0, 1, 0));
    directions.push_back(Vector3(1, 0, 0));
    directions.push_back(Vector3(0, -1, 0));
    directions.push_back(Vector3(0, 0, 1));
    directions.push_back(Vector3(0, 0, -1));

    model = PlanetModel();
    model.radius = settings.radius;
    createModel();
}

Vector3 PlanetModelFactory::getCubePoint(int x, int y, Vector3 &bottomLeft, Vector3 &xAxis, Vector3 &yAxis)
{
    float percentX = x / (float)(resolution - 1);
    float percentY = y / (float)(resolution - 1);

    Vector3 cubePoint = bottomLeft + (xAxis * percentX * 2) + (yAxis * percentY * 2);
    return cubePoint;
}

// Creates one face of the planet.
Mesh PlanetModelFactory::createMesh(int s, Material mat, float &minElev, float &maxElev)
{
    int i = 0;
    // Resolution should be at least 2.
    vector<Vector3> planetPoints(resolution * resolution);
    vector<float> vertices(resolution * resolution * 3);
    vector<float> normals(resolution * resolution * 3);
    vector<unsigned int> triangles(resolution * resolution * 6);
    int triIndex = 0;

    Vector3 zAxis = directions[s];
    // Calculate horizontal axes from the given local up direction.
    Vector3 xAxis = Vector3(zAxis.y, zAxis.z, zAxis.x); // This works since we're only ever going to be passing in cardinal directions
    Vector3 yAxis = zAxis.cross(xAxis);

    // Starting point of the square
    Vector3 bottomLeft = zAxis - xAxis - yAxis;
    // std::cout << bottomLeft.x << ", " << bottomLeft.y << ", " << bottomLeft.z << std::endl;

    // Generate the vertices for the square, and turn them into points on a sphere.
    for (int y = 0; y < resolution; y++)
    {
        for (int x = 0; x < resolution; x++)
        {
            Vector3 spherePoint = getCubePoint(x, y, bottomLeft, xAxis, yAxis).normalize();

            // Adjust point based on settings, then add to vertices
            Vector3 planetPoint = settings.getPlanetPoint(spherePoint);
            // Update min/max
            float elevation = planetPoint.length();
            if (elevation < minElev)
            {
                minElev = elevation;
            }
            if (elevation > maxElev)
            {
                maxElev = elevation;
            }

            vertices[i] = planetPoint.x;
            vertices[i + 1] = planetPoint.y;
            vertices[i + 2] = planetPoint.z;
            planetPoints[i / 3] = planetPoint;

            // Calculate normals
            int leftX = x - 1, rightX = x + 1;
            int downY = y - 1, upY = y + 1;
            // Adjacent vertices
            Vector3 leftPoint = settings.getPlanetPoint(getCubePoint(leftX, y, bottomLeft, xAxis, yAxis).normalize());
            Vector3 rightPoint = settings.getPlanetPoint(getCubePoint(rightX, y, bottomLeft, xAxis, yAxis).normalize());
            Vector3 downPoint = settings.getPlanetPoint(getCubePoint(x, downY, bottomLeft, xAxis, yAxis).normalize());
            Vector3 upPoint = settings.getPlanetPoint(getCubePoint(x, upY, bottomLeft, xAxis, yAxis).normalize());
            // Face normals
            Vector3 topLeft = (upPoint - planetPoint).cross(leftPoint - planetPoint);
            Vector3 topRight = (rightPoint - planetPoint).cross(upPoint - planetPoint);
            Vector3 botLeft = (leftPoint - planetPoint).cross(downPoint - planetPoint);
            Vector3 botRight = (downPoint - planetPoint).cross(rightPoint - planetPoint);
            // Vertex normal
            Vector3 vNormal = (topLeft + topRight + botLeft + botRight).normalize();
            normals[i] = vNormal.x;
            normals[i + 1] = vNormal.y;
            normals[i + 2] = vNormal.z;
            
            // Create triangles. Two triangles per subdivided square.
            if (x < resolution - 1 && y < resolution - 1)
            {
                triangles[triIndex] = i / 3;                        // Bottom left
                triangles[triIndex + 1] = (i / 3) + resolution + 1; // Top right
                triangles[triIndex + 2] = (i / 3) + resolution;     // Top left

                triangles[triIndex + 3] = i / 3;
                triangles[triIndex + 4] = (i / 3) + 1; // Bottom right
                triangles[triIndex + 5] = (i / 3) + resolution + 1;

                triIndex += 6;
            }
            i += 3;
        }
    }

    // Create and return the mesh
    Mesh mesh = Mesh();
    mesh.vertexIndices = triangles;
    mesh.vertices = vertices;
    mesh.normals = normals;
    mesh.material = mat;
    return mesh;
}

// Generate UV coordinates for a planet face.
void PlanetModelFactory::createUVs(Mesh &mesh, float &minElev, float &maxElev)
{
    vector<float> texCoords(mesh.vertices.size() / 3 * 2);
    int i = 0;
    float elevationDifference = maxElev - minElev;
    for (unsigned int v = 0; v < mesh.vertices.size() / 3 * 2; v += 2)
    {
        Vector3 planetPoint = Vector3(mesh.vertices[i], mesh.vertices[i + 1], mesh.vertices[i + 2]);
        float elevation = planetPoint.length();
        texCoords[v] = (elevation - minElev) / elevationDifference;
        texCoords[v + 1] = 0.5; // Texture is only 1 tall so let's just use the middle.
        i += 3;
    }

    mesh.textureCoords = texCoords;
}

void PlanetModelFactory::createModel()
{
    // Create the material for the planet
    // Colour defaultColour = Colour(0.0, 0.2, 1.0);
    Material mat = Material();
    mat.ambient = Colour(0.25, 0.25, 0.25);
    mat.diffuse = Colour(1, 1, 1);
    mat.emissive = Colour(0, 0, 0);
    mat.specular = Colour(1, 1, 1);
    mat.specularHighlights = settings.specularShine;

    // Send the texture to the material
    mat.setTextureData(settings.gradient.createImage(), 256, 1, 3, TextureType::diffuse);

    // Used for texture mapping
    // These are declared here since they need to persist for all faces.
    float minElev = __FLT_MAX__, maxElev = -__FLT_MAX__;

    // Create meshes
    for (int s = 0; s < 6; s++)
    {
        model.meshes.push_back(createMesh(s, mat, minElev, maxElev));
    }
    // Map UV coordinates
    for (int i = 0; i < 6; i++)
    {
        createUVs(model.meshes[i], minElev, maxElev);
    }
}
