#include "Objects/Material.h"

#include <iostream>

Material::Material(string name)
{
    this->name = name;
}

Material::Material(string name,
                   Colour ambient,
                   Colour diffuse,
                   Colour specular,
                   Colour emissive,
                   float specularHighlights,
                   float opticalDensity,
                   float transparency,
                   float illumMode)
{
    this->name = name;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->emissive = emissive;
    this->specularHighlights = specularHighlights;
    this->opticalDensity = opticalDensity;
    this->transparency = transparency;
    this->illumMode = illumMode;
};

// Unfortunately, due to the opengl context only begin defined on the main thread,
// we must load the textures into opengl from the main thread. This function permits this
void loadTextureIntoOpenGL(Texture &texture, GLuint &textureId)
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texture.width, texture.height, 0, (texture.numChannels == 3) ? GL_RGB : GL_RGBA, GL_FLOAT, texture.data.data());

    glBindTexture(GL_TEXTURE_2D, 0); // unbind from the texture to prevent accidental modification
}

// sets the texture data to be loaded into opengl later
void Material::setTextureData(vector<float> data, int width, int height, int numChannels, TextureType type)
{
    if (type == TextureType::diffuse)
    { // we only support diffuse for now
        this->diffuseTexture = Texture();
        this->diffuseTexture.data = data;
        this->diffuseTexture.width = width;
        this->diffuseTexture.height = height;
        this->diffuseTexture.numChannels = numChannels;
    }
}

void Material::loadTexturesIntoOpenGL()
{
    if (this->diffuseTexture.data.size() > 0)
    { // only support diffuse for now
        loadTextureIntoOpenGL(this->diffuseTexture, this->diffuseTextureId);
    }
}

Material::~Material()
{
    if (this->diffuseTextureId != 0)
    {
        glDeleteTextures(1, &(this->diffuseTextureId));
    }
}