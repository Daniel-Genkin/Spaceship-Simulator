#ifndef COLOUR_H
#define COLOUR_H

#include "Libs/nlohmann_json.hpp"

using json = nlohmann::json;

class Colour
{
private:
    float rgba[4];
    float hsla[4];

public:
    Colour(float r, float g, float b, float a = 1.0);

    Colour();

    static Colour fromHSLA(float h, float s, float l, float a);

    float *getRGBA();
    float *getHSLA();

    // JSON conversion functions
    friend void to_json(json &j, const Colour &c);
    friend void from_json(const json &j, Colour &c);
};

#endif /* COLOUR_H */
