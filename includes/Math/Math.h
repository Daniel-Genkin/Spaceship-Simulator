#ifndef MATH_H
#define MATH_H

// Math library

#define PI 3.14159265358979323846

float lerp(float start, float end, float t);

float clamp(float v, float min = 0, float max = 1);

float toRadians(float degrees);

float randFloat(float min, float max);

int randInt(int min, int max);

#endif /* MATH_H */
