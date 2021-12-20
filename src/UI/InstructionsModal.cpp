#include "UI/InstructionsModal.h"
#include "UI/Primitives/Text.h"
#include "UI/Primitives/Button.h"
#include <memory>

InstructionsModal::InstructionsModal() : Modal()
{
    // although we do support multiline text, this looks better as the text is more spread out
    // and more consistent with the controls section

    this->items = vector<shared_ptr<UIItem>>(15);
    this->items[0] = shared_ptr<Text>(new Text(WHITE, 0.2, "Controls"));
    this->items[1] = shared_ptr<Text>(new Text(WHITE, 0.12, ">> [w/s]: Adjust Ship's Pitch"));
    this->items[2] = shared_ptr<Text>(new Text(WHITE, 0.12, ">> [a/d]: Adjust Ship's Yaw"));
    this->items[3] = shared_ptr<Text>(new Text(WHITE, 0.12, ">> [q/e]: Adjust Ship's Roll"));
    this->items[4] = shared_ptr<Text>(new Text(WHITE, 0.12, ">> [f]: Toggle fullscreen"));
    this->items[5] = shared_ptr<Text>(new Text(WHITE, 0.12, ">> [Shift + w/s]: Adjust Ship's Throttle"));
    this->items[6] = shared_ptr<Text>(new Text(WHITE, 0.12, ">> [Space]: Activate Booster"));
    this->items[7] = shared_ptr<Text>(new Text(WHITE, 0.12, ">> [Esc]: Close Simulator"));
    this->items[8] = shared_ptr<Text>(new Text(WHITE, 0.2, "Goal"));
    this->items[9] = shared_ptr<Text>(new Text(WHITE, 0.12, ">> Complete quests by finding the target planet."));
    this->items[10] = shared_ptr<Text>(new Text(WHITE, 0.12, "You can use the compass to help guide you."));
    this->items[11] = shared_ptr<Text>(new Text(WHITE, 0.12, "The '+' indicates your main quest and 'o'"));
    this->items[12] = shared_ptr<Text>(new Text(WHITE, 0.12, "indicate your side quests. Click the '!' menu"));
    this->items[13] = shared_ptr<Text>(new Text(WHITE, 0.12, "button to see your list of quests as well as generate"));
    this->items[14] = shared_ptr<Text>(new Text(WHITE, 0.12, "new ones. Good luck space explorer!"));

    this->backgroundColor = BACKGROUND_COLOUR;
}

void InstructionsModal::render(int width, int height)
{
    Modal::render(width, height);
}
