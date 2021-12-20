#ifndef JSONCONVERSION_H
#define JSONCONVERSION_H

#include "Libs/nlohmann_json.hpp"
#include "Math/Vector3.h"
#include "Objects/Colour.h"
#include "Objects/CelestialObjects/NoiseFilter.h"
#include "Objects/CelestialObjects/PlanetSettings.h"

using json = nlohmann::json;

// Math Classes
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Vector3, x, y, z);

// Colours
void to_json(json &j, const Colour &c);
void from_json(const json &j, Colour &c);

// Noise Filters
void to_json(json &j, const NoiseFilter &nf);
void from_json(const json &j, NoiseFilter &nf);

// Planet Settings
void to_json(json &j, const PlanetSettings &ps);
void from_json(const json &j, PlanetSettings &ps);

#endif /* JSONCONVERSION_H */
