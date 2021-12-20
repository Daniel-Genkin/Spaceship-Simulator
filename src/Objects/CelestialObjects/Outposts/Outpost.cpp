#include "Objects/CelestialObjects/Outposts/Outpost.h"

#include <cmath>
#include "Math/Math.h"

Outpost::Outpost(Vector3 planetPosition, float planetRadius, Loader &loader)
{
    this->orbitRadius = planetRadius + randFloat(2, 5);
    this->position = planetPosition;
    this->rotation.x = randFloat(0, 2 * PI);
    this->outpost.rotation.y += 28.0f * (this->rotation.x * 180 / PI);
    this->orbitSpeed = 0.5;
    try
    {
        this->outpost = loader.loadModelFromAssets(string("assets/Outpost") + to_string(randInt(1,4)), "obj.obj");
        this->outpost.position = Vector3(0, 0, this->orbitRadius);
    }
    catch (char const *msg)
    {
        std::cout << "error loading outpost model..." << std::endl;
    }
}

void Outpost::animate(float s, Vector3 planetPosition)
{
    this->rotation.x += this->orbitSpeed * s;
    this->outpost.rotation.y += 28.0f * s;

    // apply the rotation to the position. Needed as we need to know the planet locations
    // but glRotate doesn't change position, as we must do it manually. Note that since we
    // are not orbiting around the origin we first set the orbit around the origin then offset
    // to the planet position
    this->position = Vector3(0, 0, 2.0f * this->orbitRadius);
    float newX = this->position.x * cos(this->rotation.x) + this->position.z * sin(this->rotation.x);
    float newZ = this->position.z * cos(this->rotation.x) - this->position.x * sin(this->rotation.x);

    this->position = Vector3(
        newX + planetPosition.x,
        planetPosition.y,
        newZ + planetPosition.z - this->orbitRadius);
}

void Outpost::render()
{
    glPushMatrix();
    glTranslatef(this->position.x, this->position.y, this->position.z);
    this->outpost.render();
    glPopMatrix();
}