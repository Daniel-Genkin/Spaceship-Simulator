#ifndef UIITEM_H
#define UIITEM_H

#include "Math/Point2.h"

class UIItem
{
public:
    virtual void render(Point2 topLeft, Point2 bottomRight) = 0;

    virtual ~UIItem() = default;
};

#endif
