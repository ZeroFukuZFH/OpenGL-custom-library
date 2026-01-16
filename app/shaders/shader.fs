#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 baseColor  = texture(texture1, TexCoord);
    vec4 decalColor = texture(texture2, TexCoord);
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(baseColor,decalColor, 1.0);
}