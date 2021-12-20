#ifndef MODAL_H
#define MODAL_H

#include <iostream>
#include <vector>
#include <sstream>
#include "Objects/Colour.h"
#include "Objects/Material.h"
#include "UIItem.h"

using namespace std;

#include <memory>
#include <vector>

class Modal
{
public:
    Colour backgroundColor;
    vector<shared_ptr<UIItem>> items;

    Modal() {}

    Modal(Colour backgroundColor, vector<shared_ptr<UIItem>> items);

    virtual void mouseClick(int button, int state, int x, int y) {}

    void render(int screenWidth, int screenHeight);

    ~Modal();
};

#endif
