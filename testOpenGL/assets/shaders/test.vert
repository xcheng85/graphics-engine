#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 outColor;
out vec2 outTexCoord;

void main()
{
    outColor = aColor;
    outTexCoord = aTexCoord;
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}