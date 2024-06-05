#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 Position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define MAX_POINT_LIGHTS 6
#define MAX_DIR_LIGHTS 5

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform int dirLightsLength;
uniform PointLight pointLights[MAX_DIR_LIGHTS];
uniform int pointLightsLength;

uniform Material material;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	// Properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);


	vec3 result = vec3(0.0f);
	// phase 1: directional lighting
	for(int i = 0; i < dirLightsLength; i++) {
		result += CalcDirLight(dirLights[i], norm, viewDir);
	}
	// phase 2: point light
	for(int i = 0; i < pointLightsLength; i++) {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	gl_FragColor = vec4(result, 1.0);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.Position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.Position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, TexCoords)));
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}
