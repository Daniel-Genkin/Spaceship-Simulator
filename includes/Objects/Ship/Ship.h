#ifndef SHIP_H
#define SHIP_H

#include "Math/Quaternion.h"
#include "Math/Vector3.h"
#include "Math/Vector3.h"
#include "Interfaces/Renderable.h"
#include "Objects/ObjectLoader/Loader.h"
#include "Objects/Model.h"

using namespace std;

class Ship : public Renderable
{
private:
    Vector3 rotationInput; // Modified by controls, affects rotation.
    int throttleInput = 0; // Modified by controls.
    Model model;           // Model of the ship
    float rotatingXDir = 0;
    float rotatingYDir = 0;
    float rotatingZDir = 0;
    bool boosting = false;
    float throttle = 1.0;
    float backupLightAmbient[4] = {0.09, 0.09, 0.09, 1.0};
    float backupLightDiffuse[4] = {0.9, 0.9, 0.9, 1.0};
    float backupLightSpecular[4] = {1.0, 1.0, 1.0, 1.0};
    float backupLightPosition[4] = {0, 10, -15, 0};

public:
    int overrideControlsTimer = 0;
    const int maxBoost = 100;
    Quaternion rotation;
    Vector3 cameraPosition;
    Vector3 position; // Position of the ship
    float normalSpeed, boostSpeed, boostAccel;
    float speed; // Speed of the ship
    float rotationSpeed;
    int boostLeft;

    void setup(Loader &loader);

    void pitch(int val); // Pitch the ship
    void yaw(int val);   // Yaw the ship
    void roll(int val);  // Roll the ship

    void adjustThrottle(int state);
    float getThrottle();

    void setBoostMode(bool state);

    void avoidObject(GameObject object, float radius);

    // setup the renderable object
    void init() override;

    // called once per frame to render the object
    void render() override;

    // called once per game loop iteration to update the object
    void animate(float s) override;
};

#endif /* SHIP_H */
