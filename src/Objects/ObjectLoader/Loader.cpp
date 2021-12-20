#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <cstring>
#include <cstdlib>

#include "Math/Point2.h"
#include "Math/Vector3.h"
#include "Objects/ObjectLoader/Loader.h"
#include "Objects/Model.h"
#include "Objects/Material.h"
#include "Util/StringOperations.h"

#include "Libs/stb_image.h"

using namespace std;

// resource for obj: http://www.paulbourke.net/dataformats/obj/
// resource for mtl: http://www.paulbourke.net/dataformats/mtl/
Model Loader::loadModelFromAssets(string folder, string fileName)
{
    if (fileName.substr(fileName.size() - 4) == ".obj")
    {
        string path = folder + "/" + fileName;

        if (this->modelCache.find(path) != this->modelCache.end())
        {
            //if model has been loaded before
            return copyModelFromCache(this->modelCache[path]);
        }
        else
        {
            // if not in cache yet, load form file, add to cache and return
            Model model = loadModelFromObjFile(folder, fileName);
            this->modelCache.insert(pair<string, Model>(path, model));
            return model;
        }
    }
    else
    {
        throw "File format is not supported!";
    }
}

Model Loader::copyModelFromCache(Model model)
{
    Model newModel = Model();

    copy(model.meshes.begin(), model.meshes.end(), back_inserter(newModel.meshes));

    return newModel;
}

void Loader::invalidateCaches()
{
    this->modelCache.clear();
}

Model Loader::loadModelFromObjFile(string folder, string fileName)
{
    ifstream stream = ifstream(folder + "/" + fileName);

    if (!stream)
    {
        throw "Can't open 3D model file!";
    }

    vector<Vector3> v = vector<Vector3>();
    vector<Vector3> n = vector<Vector3>();
    vector<Point2> t = vector<Point2>();
    unsigned int counter = 0;

    Model model = Model();
    Mesh currentMesh = Mesh();
    string line;
    map<string, Material> materials;

    while (getline(stream, line))
    {
        vector<string> tokens = StringOperations::split(line, ' ');
        if (tokens[0] == "v")
        { // vertex coordinates
            Vector3 p = Vector3();
            p.x = stof(tokens[1]);
            p.y = stof(tokens[2]);
            p.z = stof(tokens[3]);
            v.push_back(p);
        }
        else if (tokens[0] == "vt")
        { // texture coordinates
            Point2 p = Point2();
            p.x = stof(tokens[1]);
            p.y = stof(tokens[2]);
            t.push_back(p);
        }
        else if (tokens[0] == "vn")
        { // normal coordinates
            Vector3 p = Vector3();
            p.x = stof(tokens[1]);
            p.y = stof(tokens[2]);
            p.z = stof(tokens[3]);
            n.push_back(p);
        }
        else if (tokens[0] == "f")
        { // face
            vector<string> indices1 = StringOperations::split(tokens[1], '/');
            vector<string> indices2 = StringOperations::split(tokens[2], '/');
            vector<string> indices3 = StringOperations::split(tokens[3], '/');

            int i10 = stoi(indices1[0]) - 1;
            int i20 = stoi(indices2[0]) - 1;
            int i30 = stoi(indices3[0]) - 1;

            currentMesh.vertices.push_back(v[i10].x);
            currentMesh.vertices.push_back(v[i10].y);
            currentMesh.vertices.push_back(v[i10].z);
            currentMesh.vertices.push_back(v[i20].x);
            currentMesh.vertices.push_back(v[i20].y);
            currentMesh.vertices.push_back(v[i20].z);
            currentMesh.vertices.push_back(v[i30].x);
            currentMesh.vertices.push_back(v[i30].y);
            currentMesh.vertices.push_back(v[i30].z);

            // if texture coords are provided
            if (indices1[1] != "")
            {
                int i11 = stoi(indices1[1]) - 1;
                int i21 = stoi(indices2[1]) - 1;
                int i31 = stoi(indices3[1]) - 1;

                currentMesh.textureCoords.push_back(t[i11].x);
                currentMesh.textureCoords.push_back(t[i11].y);
                currentMesh.textureCoords.push_back(t[i21].x);
                currentMesh.textureCoords.push_back(t[i21].y);
                currentMesh.textureCoords.push_back(t[i31].x);
                currentMesh.textureCoords.push_back(t[i31].y);
            }

            // if normals are provided
            if (indices1[2] != "")
            {
                int i12 = stoi(indices1[2]) - 1;
                int i22 = stoi(indices2[2]) - 1;
                int i32 = stoi(indices3[2]) - 1;

                currentMesh.normals.push_back(n[i12].x);
                currentMesh.normals.push_back(n[i12].y);
                currentMesh.normals.push_back(n[i12].z);
                currentMesh.normals.push_back(n[i22].x);
                currentMesh.normals.push_back(n[i22].y);
                currentMesh.normals.push_back(n[i22].z);
                currentMesh.normals.push_back(n[i32].x);
                currentMesh.normals.push_back(n[i32].y);
                currentMesh.normals.push_back(n[i32].z);

                // if normals aren't provided, calculate them
            }
            else
            {
                // get the popints associated with this vertex
                Vector3 p1 = v[i10];
                Vector3 p2 = v[i20];
                Vector3 p3 = v[i30];
                // get vertex normals
                Vector3 n1 = (p3 - p1).cross(p2 - p1);
                Vector3 n2 = (p3 - p2).cross(p1 - p2);
                Vector3 n3 = (p1 - p3).cross(p2 - p3);
                // get face normals
                Vector3 norm = (n1 + n2 + n3).normalize();
                // assign to normals array
                currentMesh.normals.push_back(norm.x);
                currentMesh.normals.push_back(norm.y);
                currentMesh.normals.push_back(norm.z);
                currentMesh.normals.push_back(norm.x);
                currentMesh.normals.push_back(norm.y);
                currentMesh.normals.push_back(norm.z);
                currentMesh.normals.push_back(norm.x);
                currentMesh.normals.push_back(norm.y);
                currentMesh.normals.push_back(norm.z);
            }

            currentMesh.vertexIndices.push_back(counter++);
            currentMesh.vertexIndices.push_back(counter++);
            currentMesh.vertexIndices.push_back(counter++);
        }
        else if (tokens[0] == "mtllib")
        {
            materials = loadMaterialsFromMtlFile(folder, tokens[1]);
        }
        else if (tokens[0] == "usemtl")
        { // material specification
            currentMesh.material = materials[tokens[1]];
        }
        else if (tokens[0] == "g" || tokens[0] == "o")
        { // object or material group specification
            if (currentMesh.vertexIndices.size() > 0)
            {
                model.meshes.push_back(currentMesh);
            }
            currentMesh = Mesh();
            counter = 0;
            if (tokens[0] == "o")
            {
                v.clear();
                n.clear();
                t.clear();
            }
        }
        tokens.clear();
    }
    if (currentMesh.vertexIndices.size() > 0)
    {
        model.meshes.push_back(currentMesh);
    }
    materials.clear();
    stream.close();

    return model;
}

map<string, Material> Loader::loadMaterialsFromMtlFile(string folder, string fileName)
{
    ifstream stream = ifstream(folder + "/" + fileName);

    if (!stream)
    {
        throw "Can't open 3D model material file!";
    }

    map<string, Material> materials;

    Material currentMaterial = Material("initial");
    string line;
    while (!stream.eof())
    {
        getline(stream, line);

        vector<string> tokens = StringOperations::split(line, ' ');

        if (tokens.size() == 0 || tokens[0] == "#")
        {
            continue;
        }

        if (tokens[0] == "newmtl")
        {
            materials.insert(pair<string, Material>(currentMaterial.name, currentMaterial));
            currentMaterial = Material(tokens[1]);
        }
        else if (tokens[0] == "Ka")
        { // ambient color
            currentMaterial.ambient = Colour(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
        }
        else if (tokens[0] == "Kd")
        { // diffuse color
            currentMaterial.diffuse = Colour(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
        }
        else if (tokens[0] == "Ke")
        { // emissive color
            currentMaterial.emissive = Colour(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
        }
        else if (tokens[0] == "Ks")
        { // specular color
            currentMaterial.specular = Colour(stof(tokens[1]), stof(tokens[2]), stof(tokens[3]));
        }
        else if (tokens[0] == "Ns")
        { // specular highlights
            currentMaterial.specularHighlights = stof(tokens[1]);
        }
        else if (tokens[0] == "Ni")
        { // index of refraction / optical density
            currentMaterial.opticalDensity = stof(tokens[1]);
        }
        else if (tokens[0] == "d")
        { // dissolve / transparency
            currentMaterial.transparency = stoi(tokens[1]);
        }
        else if (tokens[0] == "illum")
        { // illumination model / mode
            currentMaterial.illumMode = stof(tokens[1]);
            /*
            0		Colour on and Ambient off
            1		Colour on and Ambient on
            2		Highlight on
            3		Reflection on and Ray trace on
            4		Transparency: Glass on
                    Reflection: Ray trace on
            5		Reflection: Fresnel on and Ray trace on
            6		Transparency: Refraction on
                    Reflection: Fresnel off and Ray trace on
            7		Transparency: Refraction on
                    Reflection: Fresnel on and Ray trace on
            8		Reflection on and Ray trace off
            9		Transparency: Glass on
                    Reflection: Ray trace off
            10		Casts shadows onto invisible surfaces
            */
        }
        else if (tokens[0] == "map_Kd")
        { // texture map
            loadTextureFromFile(folder, tokens[1], currentMaterial, TextureType::diffuse);
        }
        else if (tokens[0] == "map_Bump")
        { // normal map
            loadTextureFromFile(folder, tokens[1], currentMaterial, TextureType::normal);
        }
        else if (tokens[0] == "map_Ks")
        { // specular map
            loadTextureFromFile(folder, tokens[1], currentMaterial, TextureType::specular);
        }
        else if (tokens[0] == "map_Ke")
        { // emissive map
            loadTextureFromFile(folder, tokens[1], currentMaterial, TextureType::emit);
        }

        tokens.clear();
    }

    materials.insert(pair<string, Material>(currentMaterial.name, currentMaterial));
    stream.close();

    return materials;
}

void Loader::loadTextureFromFile(string folder, string fileName, Material &material, TextureType type)
{
    int width, height, numChannels;
    stbi_set_flip_vertically_on_load(true);
    float *data = stbi_loadf((folder + "/" + fileName).c_str(), &width, &height, &numChannels, 0);

    vector<float> vecData(data, data + width * height * numChannels);
    stbi_image_free(data);

    material.setTextureData(vecData, width, height, numChannels, type);
}

void Loader::loadCubeMap(vector<string> paths, unsigned int &id)
{
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_FALSE);

    int width, height, numChannels;
    for (unsigned int i = 0; i < paths.size(); i++)
    {
        float *data = stbi_loadf(paths[i].c_str(), &width, &height, &numChannels, 0);
        GLenum format = numChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_FLOAT, data);
        stbi_image_free(data);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

Loader::~Loader()
{
    invalidateCaches();
}
