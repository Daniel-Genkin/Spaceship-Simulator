#ifndef QUESTLOG_H
#define QUESTLOG_H

#include <vector>
#include <string>
#include "Interfaces/GameObject.h"
#include "Objects/CelestialObjects/PlanetModel.h"
#include "Quest.h"
#include "Math/Vector3.h"
#include "ObjectiveMessage.h"
#include <memory>

using namespace std;

class QuestLog
{
private:
    unsigned int questCounter = 0;
    vector<PlanetModel> *potentialTargets;

public:
    shared_ptr<Quest> mainQuest = nullptr;
    vector<shared_ptr<Quest>> activeQuests;

    QuestLog() {}
    ~QuestLog();

    void setPotentialTargets(vector<PlanetModel> &potentialTargets);

    // Called when a new quest should be generated (randomly chooses one of the potetnial targets as the target)
    shared_ptr<Quest> generateQuest();

    // Adds a quest to the log
    void claimQuest(shared_ptr<Quest> q);

    // Called once per game loop cycle to test for quest progress
    vector<ObjectiveMessage> update(Vector3 shipPosition);

    // Called by the quests when they are completed
    void questCompleted(unsigned int index);
};

#endif