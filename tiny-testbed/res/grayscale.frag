#version 330 core
out vec4 out_color;

uniform sampler2D u_texture_screen;
in vec2 v_uvs;

void main()
{
   out_color = texture(u_texture_screen, v_uvs);
   float average = 0.2126 * out_color.r + 0.7152 * out_color.g + 0.0722 * out_color.b;
   out_color = vec4(average, average, average, 1.0);
}