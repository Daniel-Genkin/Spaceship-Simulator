#include "Libs/glad.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "Math/Vector3.h"
#include "Objects/Model.h"
#include "Objects/Mesh.h"
#include <iostream>

// now that we have an OpenGL context, load the textures into it
void Model::init()
{
    for (Mesh &m : this->meshes)
    {
        m.material.loadTexturesIntoOpenGL();
    }
}

void Model::render()
{
    glPushMatrix();
    glTranslatef(this->position.x, this->position.y, this->position.z);
    glRotatef(this->rotation.x, 1, 0, 0);
    glRotatef(this->rotation.y, 0, 1, 0);
    glRotatef(this->rotation.z, 0, 0, 1);
    glScalef(this->scale.x, this->scale.y, this->scale.z);
    for (Mesh &m : this->meshes)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, m.material.ambient.getRGBA());
        glMaterialfv(GL_FRONT, GL_DIFFUSE, m.material.diffuse.getRGBA());
        glMaterialfv(GL_FRONT, GL_SPECULAR, m.material.specular.getRGBA());
        glMaterialfv(GL_FRONT, GL_EMISSION, m.material.emissive.getRGBA());
        glMaterialf(GL_FRONT, GL_SHININESS, m.material.specularHighlights);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, m.vertices.data());

        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, 0, m.normals.data());

        if (m.material.diffuseTextureId)
        {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glClientActiveTexture(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, m.material.diffuseTextureId);
            glTexCoordPointer(2, GL_FLOAT, 0, m.textureCoords.data());
        }

        glDrawElements(GL_TRIANGLES, m.vertexIndices.size(), GL_UNSIGNED_INT, m.vertexIndices.data());

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glPopMatrix();
}
