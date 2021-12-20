// Include header file for syntax
#include "UI/Meter.h"

// Include glut dependencies
#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

Meter::Meter(Colour colour, Text title, int maxProgress)
{
    this->color = colour;
    this->title = title;
    this->maxProgress = maxProgress;
};

void Meter::render(Point2 topLeft, Point2 bottomRight)
{
    glPushMatrix();
    title.render(Point2(topLeft.x, topLeft.y - 45), bottomRight);
    glColor4fv(GREY.getRGBA());
    glLineWidth(THICKNESS);
    // Draw outline
    glBegin(GL_LINE_LOOP);
    glVertex2i(topLeft.x, topLeft.y);
    glVertex2i(bottomRight.x, topLeft.y);
    glVertex2i(bottomRight.x, bottomRight.y);
    glVertex2i(topLeft.x, bottomRight.y);
    glEnd();
    int amount = (float)(topLeft.y - bottomRight.y) / (float)this->maxProgress * this->progress;
    glColor4fv(this->color.getRGBA());
    glBegin(GL_POLYGON);
    glVertex2i(topLeft.x, amount + bottomRight.y);
    glVertex2i(topLeft.x, bottomRight.y);
    glVertex2i(bottomRight.x, bottomRight.y);
    glVertex2i(bottomRight.x, amount + bottomRight.y);
    glEnd();
    glPopMatrix();
};