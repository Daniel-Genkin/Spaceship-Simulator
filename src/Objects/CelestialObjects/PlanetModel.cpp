#include "Objects/CelestialObjects/PlanetModel.h"
#include "Math/Math.h"

#include <math.h>
#include <iostream>

PlanetModel::PlanetModel() : Model()
{
    this->orbitSpeed = randFloat(10, 20);
}

void PlanetModel::animate(float s)
{
    float rotation = this->orbitSpeed * s / this->position.length();
    this->rotation.x += randFloat(0.06, 0.4);

    // apply the rotation to the position. Needed as we need to know the planet locations
    // but glRotate doesn't change position, as we must do it manually

    float newX = this->position.x * cos(rotation) + this->position.z * sin(rotation);
    float newZ = -this->position.x * sin(rotation) + this->position.z * cos(rotation);

    this->position.x = newX;
    this->position.z = newZ;

    for (long unsigned int i = 0; i < this->outposts.size(); i++)
    {
        this->outposts[i].animate(s, this->position);
    }
    Model::animate(s);
}

void PlanetModel::init()
{
    // load textures of each outpost
    for (Outpost &o : this->outposts)
    {
        o.init();
    }
    // init itself
    Model::init();
}

void PlanetModel::render()
{
    glPushMatrix();
    // render the planet (along with its transforms)
    Model::render();

    // orbit outposts
    for (long unsigned int i = 0; i < this->outposts.size(); i++)
    {
        this->outposts[i].render();
    }
    glPopMatrix();
}