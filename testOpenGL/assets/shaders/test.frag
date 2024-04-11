#include <shaders/common.h>

in vec3 outColor;
in vec2 outTexCoord;

// bindless texture
// uvec2 for uint64_t handle
layout(binding = MATERIAL) uniform material
{
	uvec2 Diffuse;
} Material;

layout(location = FRAG_COLOR, index = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(sampler2D(Material.Diffuse), outTexCoord);

    // FragColor = vec4(1, 0, 0, 1);
    // FragColor = mix(texture(inTexture0, outTexCoord), texture(inTexture1, outTexCoord), 1);
} 