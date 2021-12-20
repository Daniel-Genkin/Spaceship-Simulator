#include "Quests/Quest.h"
#include <iostream>
#include <math.h>

#define TARGET_ACCEPT_RADIUS 2500 // 50 units ^ 2 as it is fast distance

ObjectiveMessage Quest::update(Vector3 shipPosition)
{
    ObjectiveMessage msg;
    float dist = shipPosition.fastDistanceTo(target->position);

    if (dist <= TARGET_ACCEPT_RADIUS + pow(target->radius, 2))
    {
        msg.msg = this->completedMessage;
        msg.status = Status::FINISHED;
    }
    else if (dist > this->lastDist && this->lastDist != -1)
    {
        msg.msg = "You are getting further";
        msg.status = Status::NEEDS_HINT;
    }
    else
    {
        msg.msg = "Keep going";
        msg.status = Status::NO_PROGRESS;
    }
    this->lastDist = dist;
    return msg;
}

Quest::Quest(string title,
             string message,
             string completedMessage,
             PlanetModel *target)
{
    this->title = title;
    this->message = message;
    this->completedMessage = completedMessage;
    this->target = target;
}
