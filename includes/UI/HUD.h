#ifndef HUD_H
#define HUD_H

#include "Primitives/Button.h"
#include "Primitives/Text.h"
#include "Objects/Colour.h"
#include "Objects/Ship/Ship.h"
#include "Quests/Quest.h"
#include "Quests/QuestLog.h"
#include "Constants.h"
#include "InstructionsModal.h"
#include "QuestsModal.h"
#include "Compass.h"
#include "Meter.h"
#include <string>
#include "Math/Vector3.h"

class HUD
{
private:
    float timerUntilNextMessage = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    InstructionsModal instructionsModal;
    QuestsModal questsModal;
    Text questMessageText;
    Text avoidingObstacleText;
    Button helpButton;
    Button questButton;
    Compass compass;
    Meter boostIndicator;
    Meter throttleIndicator;

    void drawQuestMessage(int w, int h);

public:
    bool showInstructions;
    bool showQuestLog;
    Ship *ship;
    string questMessage;
    QuestLog *questLog;
    HUD(Ship *ship, QuestLog *questLog);
    ~HUD() {}
    void render(int width, int height);

    void mouseClick(int button, int state, int x, int y);

    void resize(int newWidth, int newHeight);

    void update(Vector3 shipPosition, float deltaTime);

    // called when UI has to be manually triggered to update (ex: when a new quest is added)
    void ForceRefreshUI();
};

#endif /* HUD_H */
