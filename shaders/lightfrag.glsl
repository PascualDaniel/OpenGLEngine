#version 410 core

out vec4 color;

uniform vec4 u_lightColor;

void main()
{
	color = u_lightColor;
}