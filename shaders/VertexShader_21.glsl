#version 330 core

layout(location = 0) in vec3 inputPosition;
layout(location = 1) in vec3 inputNormal;
layout(location = 2) in vec2 inputTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(inputPosition, 1.0f));
	Normal = mat3(transpose(inverse(model))) * inputNormal;	
	TexCoords = inputTexCoords;

	gl_Position = projection * view * model * vec4(FragPos, 1.0f);
}
