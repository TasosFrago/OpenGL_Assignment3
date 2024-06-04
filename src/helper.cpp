#include <cstring>
#include <cstdio>

#include "helper.hpp"


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

std::vector<int> getLightSrcIds(Unum num)
{
	std::vector<int> ids;
	switch(num) {
	case ID0:
		ids = { 0, 1, 2, 4, 6 };
		break;
	case ID1:
		ids = { 1, 2, 3, 5, 7 };
		break;
	case ID2:
		ids = { 2, 3, 0, 4, 7 };
		break;
	case ID3:
		ids = { 1, 2, 5, 6, 7 };
		break;
	case ID4:
		ids = { 0, 2, 4, 5, 6 };
		break;
	}
	return ids;
}

const char *f(Allocator& arena, const char *str, int id)
{
	size_t len = strlen(str) + 3;
	char *res = arena.allocate(len);
	sprintf(res, str, id);
	return res;
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
