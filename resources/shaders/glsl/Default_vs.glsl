#version 420 core

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 in_TexCoord0;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 worldMatrix;

out vec2 TexCoord0;

void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(in_Position , 1.0);
	TexCoord0 = in_TexCoord0;
}