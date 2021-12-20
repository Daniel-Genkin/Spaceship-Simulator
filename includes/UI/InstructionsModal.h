#ifndef INSTRUCTIONS_MODAL_H
#define INSTRUCTIONS_MODAL_H

#include "Primitives/Modal.h"
#include "Constants.h"
#include <string>

class InstructionsModal : public Modal
{
public:
    InstructionsModal();

    void render(int width, int height);
};

#endif
