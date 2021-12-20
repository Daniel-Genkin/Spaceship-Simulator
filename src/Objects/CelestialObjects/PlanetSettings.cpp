#include "Objects/CelestialObjects/PlanetSettings.h"

#include "Math/Math.h"
#include "Objects/CelestialObjects/NoiseFilter.h"

#include <fstream>
#include <iostream>

using std::string;
using std::vector;

PlanetSettings::PlanetSettings() {}

PlanetSettings PlanetSettings::basicPlanet(float radius)
{
    PlanetSettings toReturn = PlanetSettings();
    toReturn.radius = radius;
    toReturn.addFilter(NoiseFilter::basicNoiseFilter(), false);
    return toReturn;
}

void PlanetSettings::addFilter(NoiseFilter filter, bool useFirstAsMask)
{
    filter.multiUseFirstAsMask = useFirstAsMask;
    filters.push_back(filter);
}

Vector3 PlanetSettings::getPlanetPoint(Vector3 spherePoint)
{
    if (filters.empty())
    {
        return spherePoint * radius;
    }

    float firstLayerVal = filters[0].eval(spherePoint);
    float elevation = firstLayerVal; // Assumes that you want all your noise filters enabled if they're in the vector.

    for (unsigned int i = 1; i < filters.size(); i++)
    {
        float mask = 1;
        if (filters[i].multiUseFirstAsMask)
        {
            mask = firstLayerVal;
        }
        elevation += filters[i].eval(spherePoint) * mask;
    }

    return spherePoint * radius * (1 + elevation);
}

std::vector<PlanetSettings> PlanetSettings::storedSettings;

// Loads settings defined in assets.
void PlanetSettings::loadSettings()
{
    // Unfortunately, there isn't a cross-platform way to loop over a directory in C++11.
    // Instead, we'll load everything specified in "assets/PlanetSettings/toLoad.txt".
    vector<string> fileNames;
    string line;
    std::ifstream toLoad("assets/Custom/PlanetSettings/toLoad.txt");
    while (std::getline(toLoad, line))
    {
        // Skip lines that are empty or commented with "#".
        if (line == "" || line[0] == '#')
        {
            continue;
        }
        fileNames.push_back(line);
    }
    toLoad.close();

    // Now load settings
    for (unsigned int i = 0; i < fileNames.size(); i++)
    {
        string fileName = fileNames[i];
        std::ifstream settingsFile("assets/Custom/PlanetSettings/" + fileName);
        json j;
        settingsFile >> j;
        settingsFile.close();
        PlanetSettings::storedSettings.push_back(j.get<PlanetSettings>());
    }
}

// Clears stored settings to free up memory.
void PlanetSettings::clearStoredSettings()
{
    storedSettings.clear();
}

// Returns a randomly chosen stored PlanetSettings whose radius
// is determined by the given floats.
PlanetSettings PlanetSettings::randomStoredSettings(float minRadius, float maxRadius)
{
    int i = randInt(0, PlanetSettings::storedSettings.size());
    PlanetSettings ps = PlanetSettings::storedSettings.at(i);
    float asteroidMult = ps.isAsteroid ? 0.3 : 1.0;
    ps.radius = randFloat(minRadius, maxRadius) * asteroidMult;
    if (ps.useRandomGradient)
    {
        ps.gradient = Gradient::randomStoredGradient();
    }
    for (unsigned int i = 0; i < ps.filters.size(); i++)
    {
        ps.filters[i].randomizeSeed();
    }
    return ps;
}