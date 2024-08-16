#version 330 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uvs;
layout (location = 2) in vec2 a_offset;

uniform mat4 u_model		 = mat4(1.0);
uniform mat4 u_view			 = mat4(1.0);
uniform mat4 u_projection	 = mat4(1.0);

out vec2 v_uvs;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_position.xy + a_offset, a_position.z, 1.0);
	v_uvs = a_uvs;
}