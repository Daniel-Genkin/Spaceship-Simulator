#ifndef METER_H
#define METER_H

#include "Objects/Colour.h"
#include "Math/Point2.h"
#include "Primitives/UIItem.h"
#include "Primitives/Text.h"
#include "Constants.h"

class Meter : public UIItem
{
private:
public:
    Colour color;
    Text title;
    int progress = 0;
    int maxProgress;

    Meter() {}
    ~Meter() {}
    Meter(Colour color, Text title, int maxProgress = 100);

    void render(Point2 topLeft, Point2 bottomRight) override;
};

#endif /* METER_H */
