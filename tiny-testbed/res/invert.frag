#version 330 core
out vec4 out_color;

uniform sampler2D u_texture_screen;
in vec2 v_uvs;

void main()
{
    out_color = vec4(vec3(1.0 - texture(u_texture_screen, v_uvs)), 1.0);
}