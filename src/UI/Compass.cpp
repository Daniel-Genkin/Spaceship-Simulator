#include "UI/Compass.h"
#include <iostream>

#define tickSpacing 30
#define subTicksSpacing 10
#define tickHeight 5
#define subTickHeight 3

using namespace std;

void drawMarker(Vector3 shipPos, Vector3 pos, Vector3 shipForward, Vector3 shipNormal, float centerX, bool isMainQuest)
{
    // get the delta vector between ship and planet positions and represent as 1 unit of distance away from the ship
    Vector3 delta = (pos - shipPos).normalize();

    // project the delta onto the compass (a line essentially that is along the ship's noraml vector)
    float xPos = delta.dot(shipNormal) * centerX + centerX;

    // dot the ship's forward with the delta. if it is negative then it is behind the ship and we ignore
    if (shipForward.dot(delta) < 0)
    {
        return; // planet is behind the ship
    }

    // if the xPos would make the indicator outside the bounds of the compass, ignore
    // (small adjustment for character width basically)
    if (xPos < 0 || xPos > 2 * centerX - 6/*scaled character width*/)
    {
        return;
    }

    glColor4fv(WHITE.getRGBA());
    glPushMatrix();
    glRasterPos2i(xPos, -1);
    glLineWidth(3);
    glScalef(1, 1, 1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, isMainQuest ? '+' : 'o');
    glPopMatrix();
}

void drawCompass(Point2 topLeft, Point2 bottomRight)
{
    glLineWidth(TEXT_THICKNESS);
    glColor4fv(GREY.getRGBA());
    // draw outer box
    glBegin(GL_LINE_LOOP);
    glVertex2i(topLeft.x, topLeft.y);
    glVertex2i(bottomRight.x, topLeft.y);
    glVertex2i(bottomRight.x, bottomRight.y);
    glVertex2i(topLeft.x, bottomRight.y);
    glEnd();

    // draw the ticks
    int lenX = bottomRight.x - topLeft.x;
    int lenY = topLeft.y - bottomRight.y;
    // make the teeth here to avoid duplicate calculations in the loop (x = start, y = end)
    Point2 bottomTick = Point2(THICKNESS, tickHeight + THICKNESS);
    Point2 bottomSubTick = Point2(lenY - THICKNESS, lenY - tickHeight - THICKNESS);
    Point2 topTick = Point2(THICKNESS, subTickHeight + THICKNESS);
    Point2 topSubTick = Point2(lenY - THICKNESS, lenY - subTickHeight - THICKNESS);
    glPushMatrix();
    glTranslatef(topLeft.x, bottomRight.y, 0);
    glBegin(GL_LINES);
    for (int loc = 0; loc <= lenX; loc += tickSpacing)
    { // draw big ticks
        for (int loc2 = loc + subTicksSpacing; loc2 <= min(loc + tickSpacing, lenX); loc2 += subTicksSpacing)
        { // draw little ticks
            glVertex2i(loc2, topTick.x);
            glVertex2i(loc2, topTick.y);
            glVertex2i(loc2, topSubTick.x);
            glVertex2i(loc2, topSubTick.y);
        }
        glVertex2i(loc, bottomTick.x);
        glVertex2i(loc, bottomTick.y);
        glVertex2i(loc, bottomSubTick.x);
        glVertex2i(loc, bottomSubTick.y);
    }
    glEnd();
    glPopMatrix();
}

Compass::Compass(Ship *ship, QuestLog *log)
{
    this->log = log;
    this->ship = ship;
    this->title = Text(WHITE, 0.2, "Quest Compass", 1, true);
}

void Compass::render(Point2 topLeft, Point2 bottomRight)
{
    // get the normal and forward for the ticks later
    Vector3 normal = this->ship->rotation.applyRotation(Vector3(1, 0, 0));
    Vector3 forward = this->ship->rotation.applyRotation(Vector3(0, 0, -1));

    // title
    glPushMatrix();
    this->title.render(topLeft, Point2(bottomRight.x, topLeft.y - 40));
    glPopMatrix();

    topLeft.y -= 40;
    float centerX = (bottomRight.x - topLeft.x) / 2;
    float centerY = (topLeft.y - bottomRight.y) / 2;

    // compass
    glPushMatrix();
    drawCompass(topLeft, bottomRight);
    glTranslatef(topLeft.x, topLeft.y - centerY - THICKNESS, 0);
    for (shared_ptr<Quest> q : this->log->activeQuests)
    {
        drawMarker(this->ship->position, q->target->position, forward, normal, centerX, this->log->mainQuest == q);
    }
    glPopMatrix();
}