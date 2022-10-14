#version 330 core

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixup;

in vec4 Color;
in vec2 uv;

out vec4 outColor;

void main() 
{
	vec2 uvAd = vec2(1 - uv.x, uv.y);
	vec2 uvMag = vec2(uv.x * 0.1, uv.y * 0.1);
	outColor = mix(texture(texture0, uv), texture(texture1,uv), mixup);
}