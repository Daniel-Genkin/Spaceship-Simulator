#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
public:
    int programID = -1;
    Shader() {};

    ~Shader();
    
    void init(std::string vertexPath, std::string fragmentPath);
    // Activate the shader in OpenGL.
    void use();
};

#endif /* SHADER_H */
