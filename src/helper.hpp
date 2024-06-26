#ifndef HELPER_H
#define HELPER_H

#include <vector>
#include <cstdint>

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCALE_M(S) glm::vec3(S, S, S)

enum LightType {
	POSITIONAL,
	DIRECTIONAL
};

struct Allocator {
	std::vector<char *> allocations;

	~Allocator();
	char *allocate(size_t size);
	void freeAll();
};

const char *f(Allocator& arena, const char *str, int id);
uint32_t loadTexture(const char *path);

struct CubeRotationAttribs {
	float RotationSpeed;
	float SpinSpeed;
	bool RotationToggle;
	bool SpinToggle;
	float RotationRadius;
	glm::vec3 scaleVec;
};
glm::mat4 updateModelTrans(CubeRotationAttribs attrb, glm::mat4 &rotationTrans, glm::mat4 &spinTrans, float deltaTime);

std::vector<float> processVertexData(const float *rVertices, size_t vSize);

#endif // HELPER_H
