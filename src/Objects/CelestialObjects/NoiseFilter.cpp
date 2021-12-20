#include "Objects/CelestialObjects/NoiseFilter.h"

#include "Math/Math.h"

#include <fstream>
#include <iostream>
#include <string>

using std::string;
using std::vector;

NoiseFilter::NoiseFilter()
{
    noise = FastNoiseLite(randInt(0, RAND_MAX));
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noiseWarp = FastNoiseLite(randInt(0, RAND_MAX));
    noiseWarp.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2);
    noiseWarp.SetFractalType(FastNoiseLite::FractalType_DomainWarpProgressive);

    setFilterParameters(1, Vector3(0, 0, 0), 0);
}

NoiseFilter::NoiseFilter(float frequency, int octaves, float fLacunarity, float fGain, float fWeight, float fPingPong)
{
    noise = FastNoiseLite(randInt(0, RAND_MAX));
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    noiseWarp = FastNoiseLite(randInt(0, RAND_MAX));
    noiseWarp.SetDomainWarpType(FastNoiseLite::DomainWarpType_OpenSimplex2);
    noiseWarp.SetFractalType(FastNoiseLite::FractalType_DomainWarpProgressive);

    setFilterParameters(1, Vector3(0, 0, 0), 0);
    setNoiseParameters(frequency, octaves, fLacunarity, fGain, fWeight, fPingPong);
}

// Set custom noise filter parameters.
void NoiseFilter::setFilterParameters(float amplitude, Vector3 offset, float minValue)
{
    this->amplitude = amplitude;
    this->offset = offset;
    this->minValue = minValue;
}

// Set basic FastNoiseLight parameters.
void NoiseFilter::setNoiseParameters(float frequency, int octaves, float fLacunarity, float fGain, float fWeight, float fPingPong)
{
    noiseParams.frequency = frequency;
    noiseParams.octaves = octaves;
    noiseParams.lacunarity = fLacunarity;
    noiseParams.gain = fGain;
    noiseParams.weight = fWeight;
    noiseParams.pingPong = fPingPong;

    noise.SetFrequency(frequency);
    noise.SetFractalOctaves(octaves);
    noise.SetFractalLacunarity(fLacunarity);
    noise.SetFractalGain(fGain);
    noise.SetFractalWeightedStrength(fWeight);
    noise.SetFractalPingPongStrength(fPingPong);
}

// Set domain warp parameters. Domain warp is off by default, unless this function is called.
void NoiseFilter::setWarpParameters(float frequency, int octaves, float fLacunarity, float fGain, float fPingPong, float warpAmp)
{
    useDomainWarp = true;

    warpParams.frequency = frequency;
    warpParams.octaves = octaves;
    warpParams.lacunarity = fLacunarity;
    warpParams.gain = fGain;
    warpParams.pingPong = fPingPong;
    warpParams.warpAmplitude = warpAmp;

    noiseWarp.SetFrequency(frequency);
    noiseWarp.SetFractalOctaves(octaves);
    noiseWarp.SetFractalLacunarity(fLacunarity);
    noiseWarp.SetFractalGain(fGain);
    noiseWarp.SetFractalPingPongStrength(fPingPong);
    noiseWarp.SetDomainWarpAmp(warpAmp);
}

// Set the type of fractal noise to be generated.
void NoiseFilter::setFractal(FastNoiseLite::FractalType fType)
{
    noiseParams.fractalType = fType;
    noise.SetFractalType(fType);
}

// Randomizes the seed for fractal noise, and, if enabled, domain warp.
// Unfortunately, this involves recreating the objects.
void NoiseFilter::randomizeSeed()
{
    noise = FastNoiseLite(randInt(0, RAND_MAX));
    noise.SetFractalType(noiseParams.fractalType);
    noise.SetFrequency(noiseParams.frequency);
    noise.SetFractalOctaves(noiseParams.octaves);
    noise.SetFractalLacunarity(noiseParams.lacunarity);
    noise.SetFractalGain(noiseParams.gain);
    noise.SetFractalWeightedStrength(noiseParams.weight);
    noise.SetFractalPingPongStrength(noiseParams.pingPong);

    if (useDomainWarp)
    {
        noiseWarp = FastNoiseLite(randInt(0, RAND_MAX));
        noiseWarp.SetFractalType(warpParams.fractalType);
        noiseWarp.SetFrequency(warpParams.frequency);
        noiseWarp.SetFractalOctaves(warpParams.octaves);
        noiseWarp.SetFractalLacunarity(warpParams.lacunarity);
        noiseWarp.SetFractalGain(warpParams.gain);
        noiseWarp.SetFractalPingPongStrength(warpParams.pingPong);
        noiseWarp.SetDomainWarpAmp(warpParams.warpAmplitude);
    }
}

// Returns the fractal noise parameters.
NoiseParameters NoiseFilter::getNoiseParameters()
{
    return noiseParams;
}

// Returns the domain warp parameters.
WarpParameters NoiseFilter::getWarpParameters()
{
    return warpParams;
}

// Evaluate noise at the given point.
float NoiseFilter::eval(Vector3 point)
{
    Vector3 adjustedPoint(point.x + offset.x, point.y + offset.y, point.z + offset.z);
    float nx = adjustedPoint.x, ny = adjustedPoint.y, nz = adjustedPoint.z;
    noiseWarp.DomainWarp(nx, ny, nz);
    float noiseVal = (noise.GetNoise(nx, ny, nz) + 1) / 2;

    noiseVal = std::max(0.0f, noiseVal - minValue);
    return noiseVal * amplitude;
}

// Returns a noise filter with some basic settings.
NoiseFilter NoiseFilter::basicNoiseFilter()
{
    return NoiseFilter(0.15, 4, 2.4, 0.5, 0.5);
}
