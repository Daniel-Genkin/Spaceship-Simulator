#ifndef QUESTS_MODAL_H
#define QUESTS_MODAL_H

#include "Quests/Quest.h"
#include "Quests/QuestLog.h"
#include "Primitives/Modal.h"
#include "Constants.h"
#include <string>

class QuestsModal : public Modal
{
private:
    QuestLog *log;
    void rebuildUI();

public:
    QuestsModal() {}

    QuestsModal(QuestLog *log);

    void render(int width, int height);

    void updateList();

    void mouseClick(int button, int state, int x, int y);
};

#endif
