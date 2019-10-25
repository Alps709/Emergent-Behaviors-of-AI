#version 450 core
layout (location = 0) in vec4 vertex;  //(vec2: Position, vec2: TexCoords)

out vec2 fragTexCoord;

uniform mat4 u_proj;

void main()
{
    gl_Position = u_proj * vec4(vertex.xy, 0.0f, 1.0f);
    fragTexCoord = vertex.zw;
}