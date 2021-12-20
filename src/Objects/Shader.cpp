#include "Objects/Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include "Libs/glad.h"

// Load and create a shader from the given paths.
void Shader::init(std::string vertexPath, std::string fragmentPath)
{
    std::ifstream vertexFile(vertexPath);
    std::ifstream fragmentFile(fragmentPath);
    std::stringstream textBuffer;

    // Read from files into strings.
    textBuffer << vertexFile.rdbuf();
    std::string vertexSourceStr = textBuffer.str(); // We can't just convert it directly to a const char * for some reason

    textBuffer.str("");
    textBuffer.clear();
    textBuffer << fragmentFile.rdbuf();
    std::string fragmentSourceStr = textBuffer.str();

    // Close files
    vertexFile.close();
    fragmentFile.close();

    unsigned int vID, fID;
    // Vertex
    vID = glCreateShader(GL_VERTEX_SHADER);
    const char *vertSource = vertexSourceStr.c_str();
    glShaderSource(vID, 1, &vertSource, NULL);
    glCompileShader(vID);
    // Fragment
    fID = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragSource = fragmentSourceStr.c_str();
    glShaderSource(fID, 1, &fragSource, NULL);
    glCompileShader(fID);

    // Link shaders to create program
    programID = glCreateProgram();
    glAttachShader(programID, vID);
    glAttachShader(programID, fID);
    glLinkProgram(programID);

    // Cleanup individual vertex and frag shaders
    glDeleteShader(vID);
    glDeleteShader(fID);
}

void Shader::use()
{
    glUseProgram(programID);
}

Shader::~Shader()
{
    if (programID != -1){
        glDeleteProgram(programID);
    }
}