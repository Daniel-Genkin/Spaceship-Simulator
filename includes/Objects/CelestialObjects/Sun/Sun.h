#ifndef SUN_H
#define SUN_H

#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Math/Vector3.h"
#include "Objects/Colour.h"
#include "Interfaces/GameObject.h"
#include "Objects/Model.h"

class Sun
{
private:
    GLenum light;
    float ambient[4];
    float diffuse[4];
    float specular[4];
    Model model;

public:
    Sun(GLenum lightNum = GL_LIGHT0);
    Sun(GLenum lightNum, Vector3 pos, Colour amb, Colour dif, Colour spec);

    GameObject getGameObject();

    void init();
    void render();

    static Sun basicSun(GLenum light, Vector3 pos);
};

#endif /* SUN_H */
