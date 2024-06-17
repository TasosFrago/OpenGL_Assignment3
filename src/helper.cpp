#include <iostream>
#include <cstring>
#include <cstdio>

#include "helper.hpp"
#include "stb_image/stb_image.h"


Allocator::~Allocator()
{
	for(char *ptr : allocations) {
		free(ptr);
	}
}

char *Allocator::allocate(size_t size)
{
	char *ptr = new char[size];
	if(ptr) {
		allocations.push_back(ptr);
	}
	return ptr;
}

void Allocator::freeAll()
{
	for(char *ptr : allocations) {
		free(ptr);
	}
	allocations.clear();
}

const char *f(Allocator& arena, const char *str, int id)
{
	size_t len = strlen(str) + 3;
	char *res = arena.allocate(len);
	sprintf(res, str, id);
	return res;
}

uint32_t loadTexture(const char *path)
{
	uint32_t textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;

	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

	if(!data) {
		std::cout << "[ERROR]::Failed to load texture at path:(" << path << ")" << std::endl;
		goto out_free_data;
	}

	GLenum format;
	switch(nrComponents) {
	case 1: format = GL_RED;  break;
	case 3: format = GL_RGB;  break;
	case 4: format = GL_RGBA; break;
	}

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	goto out_free_data;

out_free_data:
	stbi_image_free(data);
	return textureID;
}

glm::mat4 updateModelTrans(CubeRotationAttribs attrb, glm::mat4 &rotationTrans, glm::mat4 &spinTrans, float deltaTime)
{
	glm::mat4 identity(1.0f);
	glm::mat4 model2Spin = glm::rotate(identity, attrb.SpinSpeed * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 model2Rotation = glm::rotate(identity, attrb.RotationSpeed * deltaTime, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model2Trans = glm::translate(identity, glm::vec3(attrb.RotationRadius + 2.0f, 0.0f, 0.0f));
	glm::mat4 model2Scale = glm::scale(identity, attrb.scaleVec);

	if(attrb.RotationToggle) {
		rotationTrans = model2Rotation * rotationTrans;
	}
	if(attrb.SpinToggle) {
		spinTrans = model2Spin * spinTrans;
	}
	return (model2Scale * rotationTrans * model2Trans * spinTrans);
}

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

void calcTangBitang(Vertex& v0, Vertex& v1, Vertex& v2)
{
	// Compute Edges
	glm::vec3 edge1 = v1.Position - v0.Position;
	glm::vec3 edge2 = v2.Position - v0.Position;
	// Compute delta UV
	glm::vec2 deltaUV1 = v1.TexCoords - v0.TexCoords;
	glm::vec2 deltaUV2 = v2.TexCoords - v0.TexCoords;

	// Calculate inverse of the determinant
	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	// Calculate tangent
	glm::vec3 tangent;
	tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent = glm::normalize(tangent);

	// Calculate bitangent
	glm::vec3 bitangent;
	bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent = glm::normalize(bitangent);

	// Assign to vertices
	v0.Tangent = v1.Tangent = v2.Tangent = tangent;
	v0.Bitangent = v1.Bitangent = v2.Bitangent = bitangent;
}

std::vector<float> processVertexData(const float *rVertices, size_t vSize)
{
	// Convert raw vertex array to Vertex structures
	std::vector<Vertex> vertices;
	for(size_t i = 0; i < vSize; i +=8) {
		Vertex vertex;
		vertex.Position = glm::vec3(rVertices[i], rVertices[i + 1], rVertices[i + 2]);
		vertex.Normal = glm::vec3(rVertices[i+3], rVertices[i+4], rVertices[i+5]);
		vertex.TexCoords = glm::vec2(rVertices[i+6], rVertices[i+7]);
		vertices.push_back(vertex);
	}

	// Calculate tangent and bitangent for each triangle
	for(size_t i = 0; i < vertices.size(); i += 3) {
		calcTangBitang(vertices[i], vertices[i+1], vertices[i+2]);
	}

	// Calculate Vertex structures back to array format
	std::vector<float> extendedVertices;
	for(const auto& vertex : vertices) {
		extendedVertices.push_back(vertex.Position.x);
		extendedVertices.push_back(vertex.Position.y);
		extendedVertices.push_back(vertex.Position.z);
		extendedVertices.push_back(vertex.Normal.x);
		extendedVertices.push_back(vertex.Normal.y);
		extendedVertices.push_back(vertex.Normal.z);
		extendedVertices.push_back(vertex.TexCoords.x);
		extendedVertices.push_back(vertex.TexCoords.y);
		extendedVertices.push_back(vertex.Tangent.x);
		extendedVertices.push_back(vertex.Tangent.y);
		extendedVertices.push_back(vertex.Tangent.z);
		extendedVertices.push_back(vertex.Bitangent.x);
		extendedVertices.push_back(vertex.Bitangent.y);
		extendedVertices.push_back(vertex.Bitangent.z);
	}

	return extendedVertices;
}
