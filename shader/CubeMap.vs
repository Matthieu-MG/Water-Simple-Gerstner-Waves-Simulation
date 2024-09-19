#version 330 core

layout (location=0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	// remove translation
	// mat4 new_view = mat4( mat3(view) );

	vec4 pos = projection * view * vec4(aPos, 1.0);
	gl_Position = pos.xyww; 
	TexCoords = aPos;
}