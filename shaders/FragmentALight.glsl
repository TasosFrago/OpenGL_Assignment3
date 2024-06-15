#version 330 core

uniform vec3 aColor;

void main()
{
	gl_FragColor = vec4(aColor, 1.0f);
}
