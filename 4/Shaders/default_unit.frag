#version 460 core

out vec4 FragColor;

in vec3 v_color;
in vec2 v_texCoord;
//in float v_texIndex;

uniform sampler2D tex0;
//uniform sampler2D u_texture[3];

void main()
{
	//int index = int(v_texIndex);
	//FragColor = texture(u_texture[index], texCoord);
	FragColor = texture(tex0, v_texCoord);;
}