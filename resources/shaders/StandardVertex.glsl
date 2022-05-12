#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 0.0f, 1.0f);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}