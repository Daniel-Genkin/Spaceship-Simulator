#version 120

uniform int screenWidth;
uniform int screenHeight;
varying vec3 worldPos;

uniform samplerCube texID;

float hash3D(vec3 seed)
{
    seed = fract(seed * vec3(1324.312, 2423.543, 575.348));
    seed += dot(seed, seed + 95.325);
    return fract(seed.x * seed.y * seed.z);
}

void main()
{
    // float depth = gl_FragCoord.z / gl_FragCoord.w;
    // vec3 coordsN = vec3(gl_FragCoord.x / screenWidth, gl_FragCoord.y / screenHeight, depth);
    // vec3 rgb = vec3(hash3D((worldPos + 0.5) * 0.0001));
    // vec3 rgb = vec3(worldPos.x + 0.5, worldPos.y + 0.5, worldPos.y);
    // vec3 rgb = coordsN;

    // vec3 uv = (worldPos - 0.5 * vec3(screenWidth, screenHeight, 0)) / screenHeight;

    // vec3 rgb = vec3(0);

    // float d = length(uv);
    // float m = 0.02 / d;

    // rgb += m;

    gl_FragColor = textureCube(texID, worldPos);
    // gl_FragColor = vec4(rgb, 1.0);
}
