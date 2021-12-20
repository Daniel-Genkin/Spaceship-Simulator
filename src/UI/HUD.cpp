// Include header file for syntax
#include "UI/HUD.h"

// Include Dependencies
#include "Quests/ObjectiveMessage.h"
#include "Math/Point2.h"
#include "Objects/Material.h"
#include "Objects/Ship/Ship.h"
#include "Quests/Quest.h"
#include "Quests/QuestLog.h"

#include <iostream>
#include <string>

using namespace std;

HUD::HUD(Ship *ship, QuestLog *questLog)
{
    this->showInstructions = false;
    this->showQuestLog = false;
    this->ship = ship;
    this->questLog = questLog;
    this->instructionsModal = InstructionsModal();
    this->questsModal = QuestsModal(questLog);
    this->questMessageText = Text(WHITE, 0.15, "", TEXT_THICKNESS, true);
    this->questButton = Button(GREY, Text(GREY, 0.5, "!", THICKNESS, true));
    this->helpButton = Button(GREY, Text(GREY, 0.5, "?", THICKNESS, true));
    this->compass = Compass(ship, questLog);
    this->boostIndicator = Meter(BLUE, Text(GREY, 0.2, "B\nO\nO\nS\nT", 1, true, false), this->ship->maxBoost);
    this->throttleIndicator = Meter(GOLD, Text(GREY, 0.2, "T\nR\nO\nT\nT\nL\nE", 1, true, false), 100);
    this->avoidingObstacleText = Text(RED, 0.5, "AVOIDING COLLISION!!", 1, true);
};

void HUD::drawQuestMessage(int w, int h)
{
    if (this->timerUntilNextMessage <= 0)
    {
        this->questMessageText.setText(this->questMessage);
    }
    glPushMatrix();
    glRasterPos2f(0, 0); // needed for macos as it doesn't support glTranslatef for text
    this->questMessageText.render(Point2(50, h - 80), Point2(w - 50, h - 110));
    glPopMatrix();
}

void HUD::render(int width, int height)
{
    // Switch to 2D mode
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    // Draw the HUD
    // Turn off the lighting
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    // Draw the instructions modal
    if (this->showInstructions)
    {
        this->instructionsModal.render(width, height);
    }

    // Draw the quest log modal
    if (this->showQuestLog)
    {
        this->questsModal.render(width, height);
    }

    if (questMessage.length() != 0)
    {
        drawQuestMessage(width, height);
    }
    // Draw buttons
    int h = height / 10;
    this->helpButton.render(Point2(10.0f, h + 10.0f), Point2(h + 10.0f, 10.0f));
    this->questButton.render(Point2((2 * THICKNESS) + h + 10.0f, h + 10.0f), Point2((2 * THICKNESS) + 2 * h + 10.0f, 10.0f));

    // compass
    this->compass.render(Point2(100, screenHeight - 10), Point2(screenWidth - 100, screenHeight - 75));

    // boost meter
    this->boostIndicator.render(Point2(screenWidth - 70, 300), Point2(screenWidth - 15, 15));

    // Throttle meter
    this->throttleIndicator.render(Point2(screenWidth - 140, 300), Point2(screenWidth - 85, 15));

    // collision avoidence message
    if (this->ship->overrideControlsTimer > 0)
    {
        this->avoidingObstacleText.render(Point2(0, screenHeight), Point2(screenWidth, 200));
    }

    // Reset Line thickness
    glLineWidth(1.0f);

    // Turn on the lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Restore the previous view
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
};

void HUD::mouseClick(int button, int state, int x, int y)
{
    // Check if mouse position is within help menu
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        showInstructions = !showInstructions && this->helpButton.clickTest(Point2(x, y));
        showQuestLog = !showQuestLog && this->questButton.clickTest(Point2(x, y));
    }
    questsModal.mouseClick(button, state, x, y);
    instructionsModal.mouseClick(button, state, x, y);
}

void HUD::resize(int newWidth, int newHeight)
{
    screenWidth = newWidth;
    screenHeight = newHeight;
}

void HUD::update(Vector3 shipPosition, float deltaTime)
{
    this->timerUntilNextMessage -= deltaTime;

    // update the keep going and getting further text at the top of the screen
    vector<ObjectiveMessage> messages = this->questLog->update(shipPosition);
    bool hasFinished = false;
    for (ObjectiveMessage &msg : messages)
    {
        this->questMessage = msg.msg;
        switch (msg.status)
        {
        case Status::NEEDS_HINT:
        case Status::NO_PROGRESS:
            break;
        case Status::FINISHED:
            hasFinished = true;
            this->timerUntilNextMessage = 4; // show message for 4 seconds
            this->questMessageText.setText(this->questMessage);
            break;
        }
    }
    // if a quest is completed, remove from the list in the modal
    if (hasFinished)
    {
        questsModal.updateList();
    }

    // update boost indicator
    this->boostIndicator.progress = this->ship->boostLeft;

    // update throttle
    this->throttleIndicator.progress = this->ship->getThrottle() * 100;
}

void HUD::ForceRefreshUI()
{
    questsModal.updateList();
}
