#include "Quests/QuestLog.h"
#include <iostream>
#include "Math/Math.h"

void QuestLog::setPotentialTargets(vector<PlanetModel> &potentialTargets)
{
    this->potentialTargets = &potentialTargets;
}

bool contains(vector<shared_ptr<Quest>>& quests, vector<PlanetModel>* planets, int index) {
    for (shared_ptr<Quest>& q : quests) {
        if (q->target == &planets->at(index)){
            return true;
        }
    }
    return false;
}

shared_ptr<Quest> QuestLog::generateQuest() 
{
    int targetIndex = randInt(0, this->potentialTargets->size());
    
    // make sure we don't generate duplicates
    int numPlanetsUntested = this->potentialTargets->size();
    while(contains(this->activeQuests, this->potentialTargets, targetIndex)) {
        targetIndex = (targetIndex + 1) % this->potentialTargets->size();
        if (--numPlanetsUntested <= 1) {
            throw "All planets have a quest already";
        }
    }

    PlanetModel& target = this->potentialTargets->at(targetIndex);

    string planetName = "Planet ";
    planetName = planetName.append(to_string(targetIndex));

    this->questCounter++;

    return shared_ptr<Quest>(new Quest(
        string("Quest ").append(to_string(this->questCounter)),
        string("Find ").append(planetName),
        string("Congrats! You found ").append(planetName),
        &target));
}

void QuestLog::claimQuest(shared_ptr<Quest> q)
{
    this->activeQuests.push_back(q);
    this->mainQuest = q;
}

vector<ObjectiveMessage> QuestLog::update(Vector3 shipPosition)
{
    vector<long unsigned int> finishedQuests;
    vector<ObjectiveMessage> messages;
    for (long unsigned int i = 0; i < activeQuests.size(); i++)
    {
        shared_ptr<Quest> q = this->activeQuests[i];
        ObjectiveMessage result = q->update(shipPosition);
        messages.push_back(result);
        if (result.status == Status::FINISHED)
        {
            finishedQuests.push_back(i);
        }
    }

    // avoids modifying list while looping over it
    for (long unsigned int questIndex = 0; questIndex < finishedQuests.size(); questIndex++)
    {
        if (this->mainQuest == this->activeQuests[finishedQuests[questIndex]])
        {
            this->mainQuest = nullptr;
            // make a new main quest
            if (this->activeQuests.size() - finishedQuests.size() == 0)
            {
                claimQuest(generateQuest());
            }
            else
            {
                this->mainQuest = this->activeQuests[0];
            }
        }
        questCompleted(finishedQuests[questIndex]);
    }
    finishedQuests.clear();

    return messages;
}

void QuestLog::questCompleted(unsigned int index)
{
    this->activeQuests.erase(this->activeQuests.begin() + index);
}

QuestLog::~QuestLog()
{
    this->activeQuests.clear();
}
