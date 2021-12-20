#include "Objects/Colour.h"

#include <algorithm>
#include <cmath>
#include <iostream>

Colour::Colour(float r, float g, float b, float a)
{
	this->rgba[0] = r;
	this->rgba[1] = g;
	this->rgba[2] = b;
	this->rgba[3] = a;

	float h = 0, s = 0, l = 0;
	float maxChannel = std::max(std::max(r, g), b);
	float minChannel = std::min(std::min(r, g), b);
	float chroma = maxChannel - minChannel;

	// Following HSL equations from Wikipedia
	// Hue
	if (std::abs(chroma) > 1e-6)
	{
		if (std::abs(maxChannel - r) < 1e-6)
		{
			// Hue is divided by 360 to get values in the range [0, 1].
			h = 60.0 * (g - b) / chroma / 360.0;
		}
		else if (std::abs(maxChannel - g) < 1e-6)
		{
			h = 60.0 * (2 + (b - r) / chroma) / 360.0;
		}
		else
		{
			h = 60.0 * (4 + (r - g) / chroma) / 360.0;
		}
	}
	// Lightness
	l = (float)(maxChannel + minChannel) / 2.0;
	// Saturation
	if (l > 0 && l < 1)
	{
		s = (maxChannel - l) / std::min(l, 1 - l);
	}

	this->hsla[0] = h;
	this->hsla[1] = s;
	this->hsla[2] = l;
	this->hsla[3] = a;
}

Colour::Colour()
{
	this->rgba[0] = 1.0;
	this->rgba[1] = 1.0;
	this->rgba[2] = 1.0;
	this->rgba[3] = 1.0;

	this->hsla[0] = 0.0;
	this->hsla[1] = 1.0;
	this->hsla[2] = 1.0;
	this->hsla[3] = 1.0;
}

Colour Colour::fromHSLA(float h, float s, float l, float a)
{
	// First convert from HSLA to RGBA. Formulae are from Wikipedia.
	float chroma = (1.0 - std::abs(2.0 * l - 1.0)) * s;

	float hPrime = h * 360.0 / 60.0;
	float x = chroma * (1.0 - std::abs(std::fmod(hPrime, 2.0) - 1.0));

	float r = 0, g = 0, b = 0;
	if (hPrime >= 0 && hPrime < 1)
	{
		r = chroma;
		g = x;
	}
	else if (1 <= hPrime && hPrime < 2)
	{
		r = x;
		g = chroma;
	}
	else if (2 <= hPrime && hPrime < 3)
	{
		g = chroma;
		b = x;
	}
	else if (3 <= hPrime && hPrime < 4)
	{
		g = x;
		b = chroma;
	}
	else if (4 <= hPrime && hPrime < 5)
	{
		r = x;
		b = chroma;
	}
	else if (5 <= hPrime && hPrime < 6)
	{
		r = chroma;
		b = x;
	}
	float m = l - chroma / 2.0;
	r += m;
	g += m;
	b += m;

	return Colour(r, g, b, a);
}

float *Colour::getRGBA()
{
	return rgba;
}

float *Colour::getHSLA()
{
	return hsla;
}
