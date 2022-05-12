#version 330 core
out vec4 FragColor;

uniform vec4 mainColor = vec4(1.0, 0, 0, 1.0);

void main()
{
	FragColor = mainColor;
}