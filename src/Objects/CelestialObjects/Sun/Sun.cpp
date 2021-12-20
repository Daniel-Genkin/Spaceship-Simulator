#include "Objects/CelestialObjects/Sun/Sun.h"

#include "Math/Math.h"
#include <iostream>
#include <vector>
#include "Objects/ObjectLoader/Loader.h"

// Creates a Sun at the origin associated with the given OpenGL light with random attributes.
Sun::Sun(GLenum lightNum)
{
	light = lightNum;
	for (int i = 0; i < 3; i++)
	{
		ambient[i] = randFloat(0, 1);
		diffuse[i] = randFloat(0, 1);
		specular[i] = randFloat(0, 1);
	}
	ambient[3] = randFloat(0, 1);
	diffuse[3] = randFloat(0, 1);
	specular[3] = randFloat(0, 1);
	this->model.position = Vector3();
	this->model.scale = Vector3(20, 20, 20);
}

// Creates a light associated with the given OpenGL light with the given attributes.
Sun::Sun(GLenum lightNum, Vector3 pos, Colour amb, Colour dif, Colour spec)
{
	this->light = lightNum;
	float* ambV = amb.getRGBA();
	float* difV = dif.getRGBA();
	float* specV = spec.getRGBA();
	for (int i = 0; i < 4; i++)
	{
		ambient[i] = ambV[i];
		diffuse[i] = difV[i];
		specular[i] = specV[i];
	}

	// load a sun model from object loader with its texture
	try
    {
		Loader loader;
        this->model = loader.loadModelFromAssets("assets/Sun", "obj.obj");
    }
    catch (char const *msg)
    {
        std::cout << "error loading sun model..." << std::endl;
    }
	this->model.position = pos;
	this->model.scale = Vector3(20, 20, 20);
}

GameObject Sun::getGameObject() {
	return this->model;
}

void Sun::init()
{
	this->model.init();
}

// Renders the light in OpenGL.
void Sun::render()
{
	// render the sun model
	// note that since it is the light, there is no light 
	// reflecting from it, thus it is very dark. So turn off lighting
	// to just show its proper colours
	glDisable(GL_LIGHTING);
	glPushMatrix();
		this->model.render();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	
	// render light emitting from the sun
	float pos[4] = {this->model.position.x, this->model.position.y, this->model.position.z, 1.0};
	glLightfv(light, GL_POSITION, pos);
	glLightfv(light, GL_AMBIENT, ambient);
	glLightfv(light, GL_DIFFUSE, diffuse);
	glLightfv(light, GL_SPECULAR, specular);

}

// Creates a basic white light at the given position.
Sun Sun::basicSun(GLenum light, Vector3 pos)
{
	return Sun(light, pos, Colour(0, 0, 0), Colour(0.8, 0.8, 0.8), Colour(1, 1, 1));
}
