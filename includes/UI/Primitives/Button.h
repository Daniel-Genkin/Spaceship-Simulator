#ifndef BUTTON_H
#define BUTTON_H

#include "Objects/Colour.h"
#include "Math/Point2.h"
#include "UIItem.h"
#include "Text.h"
#include "UI/Constants.h"

class Button : public UIItem
{
private:
public:
    string id;
    Colour color;
    Text text;
    Point2 topLeft;
    Point2 bottomRight;
    bool useOutline;

    Button() {}
    ~Button() {}
    Button(Colour color, Text text, bool useOutline = true, string id = "");

    void render(Point2 topLeft, Point2 bottomRight) override;

    bool clickTest(Point2 position);
};

#endif /* BUTTON_H */
