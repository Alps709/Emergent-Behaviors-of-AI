#version 450 core

in vec4 fragmentColour;
in vec2 fragmentTextureCoord;

out vec4 colour;

uniform sampler2D tex1;

void main(void)
{
	colour = texture(tex1, (fragmentTextureCoord));
}