#ifndef VBO_H
#define VBO_H

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

#if (!defined(__cplusplus) && !defined(bool) && !defined(SP_BOOL_TYPE))
	typedef enum bool { false = 0, true = !false } bool;
	#define SP_BOOL_TYPE
#endif


#if defined(__cplusplus)
extern "C" {
#endif

typedef uint32_t VBO_t;

void vboGen(VBO_t *vbo, const void *data, size_t size, GLenum usage);
void vboBind(VBO_t *vbo);

#if defined(__cplusplus)
}
#endif

#endif // VBO_H
