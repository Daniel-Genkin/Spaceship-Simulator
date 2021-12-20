#ifndef RENDERABLE_H
#define RENDERABLE_H

class Renderable
{
public:
    // setup the renderable object
    virtual void init() = 0;

    // called once per frame to render the object
    virtual void render() = 0;

    // called once per game loop iteration to update the object
    virtual void animate(float s) = 0;

    virtual ~Renderable() = default;
};

#endif /* RENDERABLE_H */
