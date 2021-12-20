#include "Util/JSONConversion.h"

#include "Math/Math.h"
#include "Objects/Gradient.h"

#include <vector>

// Convert a Colour to JSON
void to_json(json &j, const Colour &c)
{
    j = json{{"rgba", c.rgba}, {"hsla", c.hsla}};
}

// Convert JSON to a Colour
void from_json(const json &j, Colour &c)
{
    // Read RGBA if set, otherwise read HSLA. If both fail, then something's wrong.
    try
    {
        std::vector<float> rgba = j.at("rgba");
        c = Colour(rgba.at(0), rgba.at(1), rgba.at(2), rgba.at(3));
    }
    catch (const nlohmann::detail::out_of_range &e)
    {
        std::vector<float> hsla = j.at("hsla");
        c = Colour::fromHSLA(hsla.at(0), hsla.at(1), hsla.at(2), hsla.at(3));
    }
}

// Convert NoiseFilter to JSON
void to_json(json &j, const NoiseFilter &nf)
{
    // Create the JSON object with the regular properties
    j = json{
        {"offset", nf.offset},
        {"amplitude", nf.amplitude},
        {"minValue", nf.minValue},
        {"useFirstAsMask", nf.multiUseFirstAsMask},
        {"noiseParams", nf.noiseParams}};

    if (nf.useDomainWarp)
    {
        j["warpParams"] = nf.warpParams;
    }
}

// Convert JSON to NoiseFilter
void from_json(const json &j, NoiseFilter &nf)
{
    // Regular properties
    j.at("offset").get_to(nf.offset);
    j.at("minValue").get_to(nf.minValue);
    j.at("amplitude").get_to(nf.amplitude);
    j.at("useFirstAsMask").get_to(nf.multiUseFirstAsMask);
    NoiseParameters np = j.at("noiseParams");
    nf.setNoiseParameters(np.frequency, np.octaves, np.lacunarity, np.gain, np.weight, np.pingPong);
    nf.setFractal(np.fractalType);

    // If warp parameters are set, get those values.
    try
    {
        WarpParameters wp = j.at("warpParams");
        nf.setWarpParameters(wp.frequency, wp.octaves, wp.lacunarity, wp.gain, wp.pingPong, wp.warpAmplitude);
    }
    catch (const nlohmann::detail::out_of_range &e)
    {
        nf.useDomainWarp = false;
    }
}

// Convert planet settings to JSON
void to_json(json &j, const PlanetSettings &ps)
{
    // We are not storing radius, since that's determined elsewhere.
    j = json{
        {"isAsteroid", ps.isAsteroid},
        {"specularShine", ps.specularShine},
        {"gradient", ps.gradient},
        {"noiseFilters", ps.filters}};
}

// Convert JSON to planet settings
void from_json(const json &j, PlanetSettings &ps)
{
    // Regular properties. Radius is irrelevant here.
    j.at("isAsteroid").get_to(ps.isAsteroid);
    j.at("specularShine").get_to(ps.specularShine);
    j.at("noiseFilters").get_to(ps.filters);

    // If gradient is set, load that. Otherwise, get a random one that's been loaded.
    try
    {
        j.at("gradient").get_to(ps.gradient);
    }
    catch (const nlohmann::detail::out_of_range &e)
    {
        ps.gradient = Gradient(); // Set gradient to a placeholder.
        ps.useRandomGradient = true;
    }
}