#ifndef NOISEFILTER_H
#define NOISEFILTER_H

#include "Libs/FastNoiseLite.h"
#include "Libs/nlohmann_json.hpp"
#include "Math/Vector3.h"

#include <vector>

using json = nlohmann::json;

// Allows us to serialize the FractalType enum in JSON
NLOHMANN_JSON_SERIALIZE_ENUM(
    FastNoiseLite::FractalType,
    {{FastNoiseLite::FractalType_None, nullptr},
     {FastNoiseLite::FractalType_FBm, "FBm"},
     {FastNoiseLite::FractalType_Ridged, "Ridged"},
     {FastNoiseLite::FractalType_PingPong, "PingPong"},
     {FastNoiseLite::FractalType_DomainWarpProgressive, "DWProgressive"},
     {FastNoiseLite::FractalType_DomainWarpIndependent, "DWIndependent"}});

// Settings for the actual FastNoiseLite objects
struct NoiseParameters
{
    FastNoiseLite::FractalType fractalType = FastNoiseLite::FractalType_None;
    float frequency = 0.01;
    int octaves = 3;
    float lacunarity = 2.0;
    float gain = 0.5;
    float weight = 0.0;
    float pingPong = 2.0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(NoiseParameters, fractalType, frequency, octaves, lacunarity, gain, weight, pingPong);
};

struct WarpParameters : NoiseParameters
{
    float warpAmplitude = 1.0;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(WarpParameters, fractalType, frequency, octaves, lacunarity, gain, weight, pingPong, warpAmplitude);
};

// Noise filter for fractal noise.
class NoiseFilter
{
private:
    FastNoiseLite noise;
    FastNoiseLite noiseWarp;
    NoiseParameters noiseParams;
    WarpParameters warpParams;

public:
    float amplitude = 1;
    Vector3 offset;
    float minValue = 0;
    bool useDomainWarp = false;
    float multiUseFirstAsMask = false;

    NoiseFilter();
    NoiseFilter(float frequency, int octaves, float fLacunarity, float fGain, float fWeight, float fPingPong = 2.0);

    void setFilterParameters(float amplitude, Vector3 offset, float minValue);
    void setNoiseParameters(float frequency, int octaves, float fLacunarity, float fGain, float fWeight, float fPingPong = 2.0);
    void setWarpParameters(float frequency, int octaves, float fLacunarity, float fGain, float fPingPong, float warpAmp);
    void setFractal(FastNoiseLite::FractalType fType);
    void randomizeSeed(); // Needed when loading from JSON.

    NoiseParameters getNoiseParameters();
    WarpParameters getWarpParameters();

    float eval(Vector3 point); // Uses Vector3 because that's what the models use apparently.

    static NoiseFilter basicNoiseFilter();

    // Required to give access to private parameters
    friend void to_json(json &j, const NoiseFilter &nf);
    friend void from_json(const json &j, NoiseFilter &nf);
};

#endif /* NOISEFILTER_H */
