#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldnormal;
out vec3 fragpos;
out vec2 uv;

void main()
{	
	worldnormal = mat3(transpose(inverse(model))) * normal;
	fragpos = vec3(model * vec4(pos, 1.0));
	uv = texcoord;

	gl_Position = projection * view * vec4(fragpos, 1.0);
}
