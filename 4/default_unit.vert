#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTex;
//layout (location = 3) in float aTexIndex;

out vec3 v_color;
out vec2 v_texCoord;
//out float v_texIndex;

uniform mat4 v_MVP;

void main()
{
	v_color = aColor;
	v_texCoord = aTex;
	//v_texIndex = aTexIndex;

	gl_Position = v_MVP * vec4(aPos, 1.0);
}