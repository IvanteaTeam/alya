#version 330 core

in vec2 tc;
out vec4 Color;

uniform sampler2D tex;
uniform vec3 color;

uniform sampler2D color_texture;

void main()
{
	Color = vec4(color * texture(color_texture, tc).rgb, 1);
	Color.a = 0.5;
}
