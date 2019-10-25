#version 450 core

in vec2 fragTexCoord;

out vec4 colour;

uniform sampler2D u_tex;
uniform vec3 u_textColour;

void main()
{
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(u_tex, fragTexCoord).r);
    colour = vec4(u_textColour, 1.0f) * sampled;
}