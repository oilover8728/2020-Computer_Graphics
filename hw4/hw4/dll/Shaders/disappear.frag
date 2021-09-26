#version 430

uniform sampler2D texture;

in vec4 color;
in vec2 textest;
out vec4 fragcolor;

void main()
{
	fragcolor = texture2D(texture, textest)*0.8;
} 