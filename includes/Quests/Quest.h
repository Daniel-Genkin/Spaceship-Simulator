#ifndef QUEST_H
#define QUEST_H

#include <list>
#include <string>
#include "Objects/CelestialObjects/PlanetModel.h"
#include "ObjectiveMessage.h"
#include "Math/Vector3.h"
using namespace std;

class Quest
{
private:
    float lastDist = -1;

public:
    Quest() {}
    Quest(string title,
          string message,
          string completedMessage,
          PlanetModel *target);
    string title;
    string message;
    string completedMessage;
    PlanetModel *target;
    ObjectiveMessage update(Vector3 shipPosition);
};

#endif