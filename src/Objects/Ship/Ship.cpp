// Include Dependencies
#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Objects/Ship/Ship.h"
#include "Math/Math.h"

#include <cmath>
#include <vector>

#define FORWARD Vector3(0, 0, -1)
#define UP Vector3(0, 1, 0)
#define LEFT Vector3(1, 0, 0)
#define AVOID_TIMER_DURATION 50  // number of frames that the ship's collision avoidence override engages for
#define AVOID_IGNORE_DURATION 25 // number of frames that the ship's collision avoidence override ignores subsiquent collisions (to avoid it jumping around)

void Ship::setup(Loader &loader)
{
    // Set the ship's position to the origin
    this->position = Vector3(0.0f, 15.0f, 50.0f);
    // Make the ship face the planets
    this->rotation = Quaternion(0.0f, -0.973905f, 0.0f, 0.226948f);

    // Set the ship's speed
    this->normalSpeed = 20.0f;
    this->boostSpeed = this->normalSpeed * 15;
    this->boostAccel = 500;
    this->speed = this->normalSpeed;

    this->rotationSpeed = 90.0f;
    this->boostLeft = this->maxBoost;
    this->boosting = false;

    try
    {
        this->model = loader.loadModelFromAssets("assets/Ship", "obj.obj");
    }
    catch (char const *msg)
    {
        std::cout << "error loading ship model..." << std::endl;
    }

    this->cameraPosition = Vector3(0.0f, -1.3f, -7.0f);
}

void Ship::init()
{
    this->model.init();
}

float sign(float x)
{
    if (abs(x) < 0.0001f)
    {
        return 0;
    }
    return copysign(1, x);
}

void Ship::pitch(int val)
{
    if (this->overrideControlsTimer > 0)
    {
        return;
    }
    this->rotationInput.x = val;

    this->rotatingXDir = -sign(val);
}

void Ship::yaw(int val)
{
    if (this->overrideControlsTimer > 0)
    {
        return;
    }
    this->rotationInput.y = val;

    this->rotatingYDir = -sign(val);
}

void Ship::roll(int val)
{
    if (this->overrideControlsTimer > 0)
    {
        return;
    }
    this->rotationInput.z = val;

    this->rotatingZDir = sign(val);
}

void Ship::adjustThrottle(int state)
{
    if (this->overrideControlsTimer > 0)
    {
        return;
    }
    this->throttleInput = state;
}

float Ship::getThrottle()
{
    return this->throttle;
}

void Ship::setBoostMode(bool state)
{
    if (this->overrideControlsTimer > 0)
    {
        return;
    }
    this->boosting = state;
    if (this->boosting)
    {
        this->throttle = 1;
    }
}

void Ship::render()
{
    glEnable(GL_LIGHT1);

    glPushMatrix();
        glTranslatef(cameraPosition.x, cameraPosition.y, cameraPosition.z);
        // set lighting so we can see the ship when facing away from the sun
        glLightfv(GL_LIGHT1, GL_POSITION, this->backupLightPosition);
        glLightfv(GL_LIGHT1, GL_AMBIENT, this->backupLightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, this->backupLightDiffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, this->backupLightSpecular);
        // Render the ship's model
        this->model.render();
    glPopMatrix();
    glDisable(GL_LIGHT1); // prevent light from affecting any other models
}

void Ship::animate(float s)
{
    // "delay" the ship rotation to give the effect that the ship is rotating
    this->model.rotation.x -= 0.2f * ((this->rotatingXDir == 0) ? sign(this->model.rotation.x) : this->rotatingXDir);
    this->model.rotation.y -= 0.2f * ((this->rotatingYDir == 0) ? sign(this->model.rotation.y) : this->rotatingYDir);
    this->model.rotation.z -= 0.2f * ((this->rotatingZDir == 0) ? sign(this->model.rotation.z) : this->rotatingZDir);
    this->model.rotation.x = copysign(max(0.0f, min(5.0f, abs(this->model.rotation.x))), this->model.rotation.x);
    this->model.rotation.y = copysign(max(0.0f, min(5.0f, abs(this->model.rotation.y))), this->model.rotation.y);
    this->model.rotation.z = copysign(max(0.0f, min(5.0f, abs(this->model.rotation.z))), this->model.rotation.z);

    // Throttle
    if (this->throttleInput != 0)
    {
        this->throttle = clamp(this->throttle + copysign(0.05, this->throttleInput));
    }

    // Thrust
    Vector3 forward = rotation.applyRotation(FORWARD);
    if (this->boosting)
    {
        if (this->boostLeft > 0)
        {
            if (this->speed < this->boostSpeed)
            {
                this->speed = clamp(this->speed + this->boostAccel * s, 0, this->boostSpeed);
            }
            this->boostLeft--;
        }
        else
        {
            this->boosting = false;
        }
    }
    else
    {
        this->boostLeft = min(boostLeft + 1, this->maxBoost); // gradually regain boost
        if (this->speed > this->normalSpeed)
        {
            this->speed = clamp(this->speed - this->boostAccel * s * 2, this->normalSpeed, this->boostSpeed);
        }
    }

    this->position = this->position + forward * this->speed * s * this->throttle; // simulate acceleration

    // Rotation
    this->rotation = Quaternion::rotationQuaternion(LEFT, toRadians(this->rotationSpeed * this->rotationInput.x * s)) * this->rotation;
    this->rotation = Quaternion::rotationQuaternion(UP, toRadians(this->rotationSpeed * this->rotationInput.y * s)) * this->rotation;
    this->rotation = Quaternion::rotationQuaternion(FORWARD, toRadians(this->rotationSpeed * this->rotationInput.z * s)) * this->rotation;

    // decrease the tiemr for overriding the controls
    if (this->overrideControlsTimer > 0)
    {
        this->overrideControlsTimer--;
        // if the first part of the animation, cancel the rotation and start the engines again
        if (this->overrideControlsTimer < AVOID_TIMER_DURATION - 15)
        {
            this->throttle = 1;
            this->boostLeft = this->maxBoost;
            this->boosting = true;
            this->rotationInput = Vector3(0, 0, 0);
        }
        // if done the animation, slow down
        if (this->overrideControlsTimer <= 0)
        {
            this->boosting = false;
            this->throttle = 1;
        }
    }
}

// modified version of http://paulbourke.net/geometry/circlesphere/raysphere.c
bool raycastToSphere(Vector3 shipPos, Vector3 objectPos, float objRadius, Vector3 &p1, Vector3 &p2)
{
    float a, b, c;
    float bb4ac;

    Vector3 ray = Vector3(
        objectPos.x - shipPos.x,
        objectPos.y - shipPos.y,
        objectPos.z - shipPos.z);

    a = pow(ray.x, 2.0f) + pow(ray.y, 2.0f) + pow(ray.z, 2.0f);
    b = 2.0f * (ray.x * (shipPos.x - objectPos.x) + ray.y * (shipPos.y - objectPos.y) + ray.z * (shipPos.z - objectPos.z));
    c = pow(objectPos.x, 2.0f) + pow(objectPos.y, 2.0f) + pow(objectPos.z, 2.0f);
    c += shipPos.x * shipPos.x + shipPos.y * shipPos.y + shipPos.z * shipPos.z;
    c -= 2.0f * (objectPos.x * shipPos.x + objectPos.y * shipPos.y + objectPos.z * shipPos.z);
    c -= pow(objRadius, 2.0f);
    bb4ac = pow(b, 2.0f) - 4.0f * a * c;
    if (abs(a) < __FLT_EPSILON__ || bb4ac < 0)
    {
        return false;
    }

    float sq = sqrt(bb4ac);
    float t1 = (-b + sq) / (2.0f * a);
    float t2 = (-b - sq) / (2.0f * a);

    p1 = shipPos + ray * t1;
    p2 = shipPos + ray * t2;

    return true;
}

void Ship::avoidObject(GameObject object, float radius)
{
    if (this->overrideControlsTimer > AVOID_TIMER_DURATION - AVOID_IGNORE_DURATION)
    {
        return;
    }

    // spherical raycast form ship to object
    Vector3 intersection1;
    Vector3 intersection2;
    if (!raycastToSphere(this->position, object.position, radius, intersection1, intersection2))
    {
        return; // no need to do anything as there is no intersection
    }

    // if both spherical colliders intersect, then deflect in a direction perpendicular to the point of intersection
    // do this by setting the rotation and then setting the overrideControls to make the ship fly autonomously (lol)
    if (this->position.fastDistanceTo(intersection2) > pow(radius, 2) + 90 /*ship radius + padding for animation*/ && this->position.fastDistanceTo(intersection1) > pow(radius, 2) + 90)
    {
        return; // no need to do anything as we are far enough away
    }
    // rotate 2.5 degrees per frame (for 15 frames)
    this->rotationInput = Quaternion::rotationQuaternion(this->position.cross(object.position).normalize() * 3.5, toRadians(180)).toVector();
    this->overrideControlsTimer = AVOID_TIMER_DURATION;
    this->throttle = 0.1;
    this->speed = 0.1;
}
