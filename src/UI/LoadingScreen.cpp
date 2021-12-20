#include "Libs/glad.h"
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/freeglut.h"
#endif

#include <iostream>
#include "UI/LoadingScreen.h"

LoadingScreen::LoadingScreen()
{
    this->Title = Text(BLUE, 0.7, "Space Explorer", 5, true);
    this->loading = Text(GREY, 0.5, "Loading");
    this->log = Text(GREY, 0.15, "");
}

void LoadingScreen::animate()
{
    string text = this->loading.getText().append(".");
    if (text.length() > 10)
    {
        text = "Loading";
    }
    this->loading.setText(text);

    // progress bar
}

void LoadingScreen::nextLoadingStep(string msg)
{
    this->log.setText(msg + "\n" + this->log.getText());
    this->progress++;
}

void LoadingScreen::setNumLoadingSteps(int maxProgress)
{
    this->maxProgress = maxProgress;
}

void LoadingScreen::render(int screenWidth, int screenHeight)
{
    glPushMatrix();
    this->Title.render(Point2(0, screenHeight), Point2(screenWidth, screenHeight - 120));
    this->loading.render(Point2(15, screenHeight - 120), Point2(screenWidth, screenHeight - 200));

    //progress bar
    int amount = (float)screenWidth / (float)this->maxProgress * this->progress;
    glColor4fv(BLUE.getRGBA());
    glBegin(GL_POLYGON);
    glVertex2i(0, screenHeight - 220);
    glVertex2i(0, screenHeight - 240);
    glVertex2i(amount, screenHeight - 240);
    glVertex2i(amount, screenHeight - 220);
    glEnd();

    // height doesn't matter here as we just overflow
    this->log.render(Point2(30, screenHeight - 260), Point2(screenWidth - 30, screenHeight - 260));

    // background
    glColor4fv(BLACK.getRGBA());
    glBegin(GL_POLYGON);
    glVertex2i(0, 0);
    glVertex2i(screenWidth, 0);
    glVertex2i(screenWidth, screenHeight);
    glVertex2i(0, screenHeight);
    glEnd();
    glPopMatrix();
}