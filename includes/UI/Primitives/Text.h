#ifndef TEXT_H
#define TEXT_H

#include "Objects/Colour.h"
#include "Math/Point2.h"
#include "UIItem.h"
#include <string>

using namespace std;

class Text : public UIItem
{
private:
    int textLength;
    int textHeight;
    string text;

public:
    Colour color;
    float size;
    int strokeWidth;
    bool centerAlign;
    bool verticalCenterAlign;

    Text() {}
    ~Text() {}
    Text(Colour color, float size, string text, int strokeWidth = 1, bool centerAlign = false, bool verticalCenterAlign = true);

    void render(Point2 topLeft, Point2 bottomRight) override;

    void setText(string text);
    string getText();
};

#endif
