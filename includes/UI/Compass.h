#ifndef COMPASS_H
#define COMPASS_H

#include "Quests/QuestLog.h"
#include "Primitives/UIItem.h"
#include "Math/Vector3.h"
#include "Objects/Ship/Ship.h"
#include "Constants.h"
#include "Primitives/Text.h"

class Compass : public UIItem
{
public:
    QuestLog *log;
    Ship *ship;
    Text title;

    Compass() {}
    ~Compass() {}
    Compass(Ship *ship, QuestLog *log);

    void render(Point2 topLeft, Point2 bottomRight) override;
};

#endif /* COMPASS_H */
