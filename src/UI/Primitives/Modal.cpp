#include <iostream>

#include "UI/Primitives/Text.h"
#include "UI/Primitives/Button.h"
#include "UI/Primitives/Modal.h"
#include "Objects/Colour.h"

Modal::Modal(Colour backgroundColor, vector<shared_ptr<UIItem>> items)
{
    this->backgroundColor = backgroundColor;
    this->items = items;
};

void Modal::render(int screenWidth, int screenHeight)
{
    int height = 40;
    Point2 topLeft = Point2(screenWidth / 10 + 10, 9 * screenHeight / 10 - 15);
    Point2 bottomRight = Point2(9 * screenWidth / 10 - 10, 9 * screenHeight / 10 - 15 - height);

    glPushMatrix();
    // Render items
    for (shared_ptr<UIItem> &i : this->items)
    {
        i->render(topLeft, bottomRight);
        topLeft.y -= height;
        bottomRight.y -= height;
    }
    // Draw modal shade background
    glColor4fv(this->backgroundColor.getRGBA());
    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(screenWidth, 0);
    glVertex2i(screenWidth, screenHeight);
    glVertex2i(0, screenHeight);
    glEnd();
    glPopMatrix();
};

Modal::~Modal()
{
    this->items.clear();
}

// not used but serves as a template for modals
// void Modal::mouseClick(int button, int state, int x, int y)
// {
//     // Check if mouse position is within help menu
//     if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//     {
//         for (UIItem *item : this->items)
//         {
//             auto *asButton = dynamic_cast<Button *>(item);
//             if (asButton)
//             {
//                 asButton->clickTest(Point2(x, y));
//             }
//         }
//     }
// }
