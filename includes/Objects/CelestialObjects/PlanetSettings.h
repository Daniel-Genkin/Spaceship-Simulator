#ifndef PLANETSETTINGS_H
#define PLANETSETTINGS_H

#include "Math/Vector3.h"
#include "Objects/Gradient.h"
#include "NoiseFilter.h"

#include <vector>

class PlanetSettings
{
private:
    std::vector<NoiseFilter> filters;
    bool useRandomGradient = false; // Mainly for templates loaded from JSON.

    static std::vector<PlanetSettings> storedSettings;

public:
    // Asteroids should be smaller than regular planets.
    // Only relevant for templates loaded from JSON.
    bool isAsteroid;

    float specularShine = 1;
    float radius;
    Gradient gradient;

    PlanetSettings();
    static PlanetSettings basicPlanet(float radius);
    void addFilter(NoiseFilter filter, bool useFirstAsMask);

    // Are these needed? We should make sure the filters are set up the way we want before adding them.
    // void getFilter(int index);
    // void setFilter(int index, NoiseFilter filter, bool useFirstAsMask);

    Vector3 getPlanetPoint(Vector3 spherePoint);

    // JSON loading
    static void loadSettings();
    static void clearStoredSettings();
    static PlanetSettings randomStoredSettings(float minRadius, float maxRadius);

    // JSON conversion
    friend void to_json(json &j, const PlanetSettings &ps);
    friend void from_json(const json &j, PlanetSettings &ps);
};

#endif /* PLANETSETTINGS_H */
