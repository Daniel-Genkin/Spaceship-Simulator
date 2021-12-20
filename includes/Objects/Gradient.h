#ifndef GRADIENT_H
#define GRADIENT_H

#include <vector>
#include <tuple>

#include "Libs/nlohmann_json.hpp"

#include "Objects/Colour.h"

using json = nlohmann::json;

class Gradient
{
private:
	float minVal = 0;
	float maxVal = 1;
	std::vector<Colour> colours;
	std::vector<float> positions;

	static std::vector<Gradient> storedGradients;

public:
	Gradient();
	Gradient(Colour start, float min, float max);

	void addColour(Colour colour, float position);
	Colour eval(float position);
	std::vector<float> createImage();

	static void loadGradients();
	static void clearStoredGradients();
	static Gradient randomStoredGradient();

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Gradient, minVal, maxVal, colours, positions)
};

#endif /* GRADIENT_H */
