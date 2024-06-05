#include <iostream>
#include <cstring>
#include <cstdio>

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

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
