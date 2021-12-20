#version 120

uniform int screenWidth;
uniform int screenHeight;
// attribute vec3 vPos;
varying vec3 worldPos;

uniform samplerCube texID;

void main()
{
    worldPos = gl_Vertex.xyz; // For use in frag shader.

    vec4 pos = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(gl_Vertex.xyz, 1.0);
    gl_Position = pos.xyww;
}
