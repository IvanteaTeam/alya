#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 Tc;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Proj;

out vec2 tc;

void main()
{
	tc = Tc;
	gl_Position = Proj * View * Model * vec4(Pos, 1);
}
