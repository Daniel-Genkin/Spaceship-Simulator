#include "UI/Primitives/Text.h"
#include <algorithm>

#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <vector>
#include "Util/StringOperations.h"

#include <iostream>

int calculateTextLength(string text, float size)
{
    // split the text into lines and get he max length of any line
    vector<string> lines = StringOperations::split(text, '\n');
    int maxLength = 0;
    for (string &line : lines)
    {
        // 104.76 is the width of 1 character at size 1 (yeah I know, it's a weird number)
        maxLength = max(maxLength, (int)(line.size() * 104.76 * size));
    }
    return maxLength;
}

Text::Text(Colour color, float size, string text, int strokeWidth, bool centerAlign, bool verticalCenterAlign)
{
    this->color = color;
    this->size = size;
    this->text = text;
    this->strokeWidth = strokeWidth;
    this->centerAlign = centerAlign;
    this->verticalCenterAlign = verticalCenterAlign;
    this->textLength = calculateTextLength(text, size);
    this->textHeight = 44 * this->size * (count(text.begin(), text.end(), '\n') + 1); // 44 is the approximation that we use for avg character height
};

void Text::setText(string text)
{
    this->text = text;
    this->textLength = calculateTextLength(text, this->size);
}
string Text::getText()
{
    return this->text;
}

void Text::render(Point2 topLeft, Point2 bottomRight)
{
    int yOffset = this->verticalCenterAlign ? (topLeft.y - bottomRight.y) / 2 + this->textHeight : 0;
    int xOffset = this->centerAlign ? ((bottomRight.x - topLeft.x) / 2 - this->textLength / 2) : 0;
    glPushMatrix();
    glLineWidth(this->strokeWidth);
    glTranslatef(topLeft.x + xOffset, topLeft.y - yOffset, 0.0);
    glColor4fv(this->color.getRGBA());
    glScalef(this->size, this->size, 1.0);
#ifndef __APPLE__ // MacOS doesn't support stroke string and makes life harder for everyone (as always)
    glutStrokeString(GLUT_STROKE_MONO_ROMAN, (const unsigned char *)this->text.c_str());
#else
    // works just like a typewritter (minus th ding)
    int charsOnLine = 0;
    for (int i = 0; i < this->text.length(); i++)
    {
        if (this->text[i] != '\n')
        {
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, this->text[i]);
            charsOnLine++;
        }
        else
        {
            glTranslatef(-charsOnLine * 104.76, -150, 0.0);
            charsOnLine = 0;
        }
    }
#endif
    glPopMatrix();
};