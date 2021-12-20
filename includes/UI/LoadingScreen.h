#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include "Primitives/Text.h"
#include "Constants.h"
#include <string>

using namespace std;

class LoadingScreen
{
private:
    string text;
    Text Title;
    Text loading;
    Text log;
    int progress;
    int maxProgress;

public:
    LoadingScreen();
    void render(int screenWidth, int screenHeight);
    void animate();
    void nextLoadingStep(string msg);
    void setNumLoadingSteps(int maxProgress);
};

#endif
