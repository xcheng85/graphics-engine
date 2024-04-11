#include <shaders/common.h>

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout(std140, binding = 0) uniform PerFrameData
{
	uniform mat4 mvp;
};

out vec3 outColor;
out vec2 outTexCoord;

// ARB_separate_shader_objects requires built-in block gl_PerVertex to be redeclared before accessing its members
out gl_PerVertex
{
    vec4 gl_Position;
};

void main()
{
    outColor = aColor;
    outTexCoord = aTexCoord;
    // mat4 transform1 = mat4(2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    // left-hand
    gl_Position = mvp * vec4(aPos, 1.0f);
}