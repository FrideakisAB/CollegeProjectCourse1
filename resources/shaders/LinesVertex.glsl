#version 330 core

in vec2 aPos;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(aPos, 0.0f, 1.0f);
}