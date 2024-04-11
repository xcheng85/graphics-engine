#version 460 core

out vec4 FragColor;

// uniform vec4 intensity;

in vec3 outColor;
in vec2 outTexCoord;

// uniform sampler2D inTexture0;
// uniform sampler2D inTexture1;
void main()
{
    FragColor = vec4(1, 0, 0, 1);
    // FragColor = mix(texture(inTexture0, outTexCoord), texture(inTexture1, outTexCoord), 1);
} 