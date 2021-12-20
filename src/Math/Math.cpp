#include "Math/Math.h"

#define _USE_MATH_DEFINES // Required for some platforms
#include <cmath>

// Linear interpolation between start and end.
float lerp(float start, float end, float t)
{
    t = clamp(t);
    if (t >= 1)
    {
        return end;
    }
    return start + (end - start) * t;
}

// Clamps v to be between min and max.
float clamp(float v, float min, float max)
{
    if (v < min)
    {
        return min;
    }
    else if (v > max)
    {
        return max;
    }
    return v;
}

// Converts radians to degrees
float toRadians(float degrees)
{
    return degrees * (PI / 180.0);
}

float randFloat(float min, float max)
{
    return (rand() / (float)(RAND_MAX / (max - min))) + min;
}

int randInt(int min, int max)
{
    if (min == max)
    {
        return min;
    }
    return (rand() % (max - min)) + min;
}