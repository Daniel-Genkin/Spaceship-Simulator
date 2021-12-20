#include "UI/QuestsModal.h"
#include "UI/Primitives/Text.h"
#include "UI/Primitives/Button.h"

#define MAX_QUESTS 5
bool sema = false;

QuestsModal::QuestsModal(QuestLog *log) : Modal()
{
    this->backgroundColor = BACKGROUND_COLOUR;
    this->log = log;
    rebuildUI();
}

void QuestsModal::rebuildUI()
{
    this->items.clear();
    this->items.push_back(shared_ptr<Text>(new Text(WHITE, 0.2, "Quest Log:")));

    if (log->activeQuests.size() < MAX_QUESTS)
    {
        this->items.push_back(shared_ptr<Button>(
            new Button(WHITE, Text(WHITE, 0.15, "Generate new quest"), true, "addQuest")));
    }

    for (shared_ptr<Quest> q : log->activeQuests)
    {
        this->items.push_back(
            shared_ptr<Text>(new Text(WHITE, 0.15, (q == log->mainQuest ? "Main: " : "") + q->title)));
        this->items.push_back(
            shared_ptr<Text>(new Text(WHITE, 0.12, q->message)));
    }
}

void QuestsModal::updateList()
{
    rebuildUI();
}

void QuestsModal::render(int width, int height)
{
    Modal::render(width, height);
}

void QuestsModal::mouseClick(int button, int state, int x, int y)
{
    // Check if mouse position is within help menu
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !sema)
    {
        sema = true;

        for (long unsigned int i = 0; i < this->items.size(); i++)
        {
            shared_ptr<Button> asButton = static_pointer_cast<Button>(this->items[i]);
            if (asButton && asButton.use_count() > 0 && asButton->clickTest(Point2(x, y)))
            {
                if (asButton->id == "addQuest")
                {
                    this->log->claimQuest(this->log->generateQuest());
                    updateList();
                }
            }
        }
    }
    if (state == GLUT_UP)
    { // if user holds the mouse button, don't spam create new quests
        sema = false;
    }
}
