#version 330 core

uniform vec3 lightSourceColor;

void main()
{
	gl_FragColor = vec4(lightSourceColor, 1.0);
}