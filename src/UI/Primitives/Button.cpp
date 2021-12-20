// Include header file for syntax
#include "UI/Primitives/Button.h"

// Include glut dependencies
#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

Button::Button(Colour colour, Text text, bool useOutline, string id)
{
    this->color = colour;
    this->text = text;
    this->useOutline = useOutline;
    this->id = id;
};

bool Button::clickTest(Point2 clickPosition)
{
    return (
        clickPosition.x > this->topLeft.x &&
        clickPosition.x < this->bottomRight.x &&
        clickPosition.y < this->topLeft.y &&
        clickPosition.y > this->bottomRight.y);
}

void Button::render(Point2 topLeft, Point2 bottomRight)
{
    this->topLeft = topLeft;
    this->bottomRight = bottomRight;

    glPushMatrix();
    if (this->useOutline)
    {
        glColor4fv(this->color.getRGBA());
        glLineWidth(THICKNESS);
        // Draw Square
        glBegin(GL_LINE_LOOP);
        glVertex2i(topLeft.x, topLeft.y);
        glVertex2i(bottomRight.x, topLeft.y);
        glVertex2i(bottomRight.x, bottomRight.y);
        glVertex2i(topLeft.x, bottomRight.y);
        glEnd();
    }
    this->text.render(
        Point2(topLeft.x + THICKNESS, topLeft.y - THICKNESS),
        Point2(bottomRight.x - THICKNESS, bottomRight.y + THICKNESS));
    glPopMatrix();
};