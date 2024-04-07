#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;

out vec3 outColor;
out vec2 outTexCoord;

void main()
{
    outColor = aColor;
    outTexCoord = aTexCoord;
   // mat4 transform1 = mat4(2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    gl_Position = transform * vec4(aPos, 1.0f);
}