#include "Objects/Gradient.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include "Math/Math.h"

using std::string;
using std::vector;

// Constructs a gradient with black as the starting colour.
Gradient::Gradient()
{
    colours.push_back(Colour(0, 0, 0));
    positions.push_back(0);
}

// Creates a gradient with a custom start colour, as well as min and max values.
Gradient::Gradient(Colour start, float min, float max)
{
    minVal = min;
    maxVal = max;
    colours.push_back(start);
    positions.push_back(min);
}

// Adds the specified colour to the gradient at the given position.
void Gradient::addColour(Colour colour, float position)
{
    position = clamp(position, minVal, maxVal);
    colours.push_back(colour);
    positions.push_back(position);
}

// Evaluate the gradient at the given position.
Colour Gradient::eval(float position)
{
    position = clamp(position, minVal, maxVal);

    // Find where on the gradient the position is
    int leftIndex = 0, rightIndex = colours.size() - 1;
    for (unsigned int i = 1; i < colours.size() - 1; i++)
    {
        if (positions[i] < position)
        {
            leftIndex = i;
        }
        else if (positions[i] >= position)
        {
            rightIndex = i;
            break;
        }
    }
    Colour left = colours[leftIndex];
    Colour right = colours[rightIndex];

    // Previously we had HSLA interpolation only, but that produces funny values sometimes.
    // This discussion (https://pyxeledit.com/forum/discussion/110/palette-gradient-algorithms)
    // brought up a model where you use both an RGB and HSL interpolation depending on hues.

    // Get the HSL interpolation and the alpha channel
    float *leftVals = left.getHSLA();
    float *rightVals = right.getHSLA();

    float leftPos = positions[leftIndex];
    float rightPos = positions[rightIndex];
    float h = lerp(leftVals[0], rightVals[0], (position - leftPos) / (rightPos - leftPos));
    float s = lerp(leftVals[1], rightVals[1], (position - leftPos) / (rightPos - leftPos));
    float l = lerp(leftVals[2], rightVals[2], (position - leftPos) / (rightPos - leftPos));
    float a = lerp(leftVals[3], rightVals[3], (position - leftPos) / (rightPos - leftPos));

    // Get the RGB interpolation in HSL
    leftVals = left.getRGBA();
    rightVals = right.getRGBA();
    float r = lerp(leftVals[0], rightVals[0], (position - leftPos) / (rightPos - leftPos));
    float g = lerp(leftVals[1], rightVals[1], (position - leftPos) / (rightPos - leftPos));
    float b = lerp(leftVals[2], rightVals[2], (position - leftPos) / (rightPos - leftPos));
    float *hsla2 = Colour(r, g, b, a).getHSLA();

    // Weight interpolations based on difference in hue.
    // 0 is full HSL while 1 is full RGB.
    float balance = std::abs(rightVals[0] - leftVals[0]) * 2;
    h = hsla2[0] * balance + h * (1 - balance);
    s = hsla2[1] * balance + s * (1 - balance);
    l = hsla2[2] * balance + l * (1 - balance);

    return Colour::fromHSLA(h, s, l, a);
}

// Creates an array representing an image file generated from this gradient.
vector<float> Gradient::createImage()
{
    int imageResolution = 256;
    vector<float> image = vector<float>(imageResolution * 3); // 3 values per pixel
    float width = maxVal - minVal;
    for (int x = 0; x < imageResolution; x++)
    {
        float percent = (float)x / (imageResolution - 1);
        float *rgba = eval(minVal + width * percent).getRGBA();
        int i = x * 3;
        image[i] = rgba[0];
        image[i + 1] = rgba[1];
        image[i + 2] = rgba[2];
    }
    return image;
}

vector<Gradient> Gradient::storedGradients;

// Loads gradients defined in assets.
void Gradient::loadGradients()
{
    // Unfortunately, there isn't a cross-platform way to loop over a directory in C++11.
    // Instead, we'll load everything specified in "assets/Gradients/toLoad.txt".
    vector<string> fileNames;
    string line;
    std::ifstream toLoad("assets/Custom/Gradients/toLoad.txt");
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

    // Now load gradients
    for (unsigned int i = 0; i < fileNames.size(); i++)
    {
        string fileName = fileNames[i];
        std::ifstream gradFile("assets/Custom/Gradients/" + fileName);
        json j;
        gradFile >> j;
        gradFile.close();
        Gradient::storedGradients.push_back(j.get<Gradient>());
    }
}

// Clears stored gradients to free up memory.
void Gradient::clearStoredGradients()
{
    storedGradients.clear();
}

// Returns a randomly chosen stored gradient.
Gradient Gradient::randomStoredGradient()
{
    int i = randInt(0, Gradient::storedGradients.size());
    return Gradient::storedGradients.at(i);
}