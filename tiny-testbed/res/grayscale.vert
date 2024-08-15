#version 330 core
layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;

out vec2 v_uvs;

void main()
{
	gl_Position = vec4(a_position, 1.0);
	v_uvs = a_uvs;
}