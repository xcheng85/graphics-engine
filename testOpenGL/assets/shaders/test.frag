#include <shaders/common.h>

in block
{
    vec2 TexCoord;
} In;

// bindless texture
// uvec2 for uint64_t handle
layout(binding = MATERIAL) uniform material
{
	uvec2 Diffuse;
} Material;

layout(location = FRAG_COLOR, index = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(sampler2D(Material.Diffuse), In.TexCoord);

    // FragColor = mix(texture(inTexture0, outTexCoord), texture(inTexture1, outTexCoord), 1);
} 