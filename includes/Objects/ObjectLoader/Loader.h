#ifndef LOADER_H
#define LOADER_H

#include "Objects/Model.h"
#include <string>
#include <map>
#include <vector>

using namespace std;

class Loader
{
private:
    map<string, Material> loadMaterialsFromMtlFile(string folder, string fileName);
    Model loadModelFromObjFile(string folder, string fileName);
    Model copyModelFromCache(Model model);
    void loadTextureFromFile(string folder, string fileName, Material &material, TextureType type);

    map<string, Model> modelCache = map<string, Model>();

public:
    Model loadModelFromAssets(string folder, string fileName);

    void loadCubeMap(vector<string> paths, unsigned int &id);

    void invalidateCaches();

    Loader() {}

    ~Loader();
};

#endif /* LOADER_H */
