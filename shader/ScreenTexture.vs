#version 330 core

layout (location=0) in vec3 Position;
layout (location=1) in vec2 TexCoords;

out vec2 TexPos;

void main()
{
	gl_Position = vec4(Position.x, Position.y, 0.0, 1.0);
	TexPos = TexCoords;
}