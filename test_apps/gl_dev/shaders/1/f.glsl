#version 330 core

in vec2 tc;
out vec4 Color;

uniform sampler2D tex;
uniform vec3 color;

void main()
{
	Color = vec4(color, 1);
	//Color = texture(tex, tc);
}
