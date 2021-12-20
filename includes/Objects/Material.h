#ifndef MATERIAL_H
#define MATERIAL_H

#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Colour.h"
#include <string>
#include <vector>

using namespace std;

enum TextureType
{
    diffuse,
    specular,
    emit,
    normal
};

class Texture
{
public:
    vector<float> data;
    int width;
    int height;
    int numChannels;
};

class Material
{
public:
    Texture diffuseTexture;
    string name;
    Colour ambient;
    Colour diffuse;
    Colour specular;
    Colour emissive;
    float specularHighlights;
    float opticalDensity;
    float transparency;
    float illumMode;
    GLuint diffuseTextureId = 0;

    Material(string name);

    Material(string name,
             Colour ambient,
             Colour diffuse,
             Colour specular,
             Colour emissive,
             float specularHighlights,
             float opticalDensity,
             float transparency,
             float illumMode);

    void loadTexturesIntoOpenGL();
    void setTextureData(vector<float>,
                        int width,
                        int height,
                        int numChannels,
                        TextureType type);

    Material() {}

    ~Material();
};

#endif /* MATERIAL_H */
