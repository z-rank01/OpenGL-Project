#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 TexCoord;

out vec4 Color;
out vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 perspective;


void main()
{
	gl_Position = perspective * view * model * vec4(pos, 1.0);
	Color = vec4(col, 1.0);
	uv = vec2(TexCoord.x, TexCoord.y);
}