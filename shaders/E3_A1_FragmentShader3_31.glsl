#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D noLight;

	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define MAX_DIR_LIGHTS 6

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;

uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform int dirLightsLength;

uniform sampler2D texture1;
uniform sampler2D texture2;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec4 TexColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.2);

	// Properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0f);
	// phase 1: directional lighting
	for(int i = 0; i < dirLightsLength; i++) {
		result += CalcDirLight(dirLights[i], norm, viewDir);
	}

	FragColor = TexColor * vec4(result, 1.0f);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
	// combine results
	vec3 ambient = light.ambient * vec3(1.0f);
	vec3 diffuse = light.diffuse * (diff * vec3(1.0f));
	vec3 specular = light.specular * (spec * vec3(1.0f));
	return (ambient + diffuse + specular);
}
