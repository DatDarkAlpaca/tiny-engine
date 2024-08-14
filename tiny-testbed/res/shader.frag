#version 330 core
out vec4 out_color;

uniform sampler2D u_texture;
in vec2 v_uvs;

void main()
{
	out_color = texture(u_texture, v_uvs);
}