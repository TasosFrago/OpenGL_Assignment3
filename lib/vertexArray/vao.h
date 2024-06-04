#ifndef VAO_H
#define VAO_H

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

#if (!defined(__cplusplus) && !defined(bool) && !defined(SP_BOOL_TYPE))
	typedef enum bool { false = 0, true = !false } bool;
	#define SP_BOOL_TYPE
#endif

#include "vbo.h"
#include "vbLayout.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef uint32_t VAO_t;

void vaoGen(VAO_t *vao);
void vaoBind(VAO_t *vao);
void vaoAddBuffer(VAO_t *vao, VBO_t *vbo, VBLayout_t *layout);
/* void vaoAddBufferM(VAO_t *vao, VBO_t *vbo[], VBLayout_t *layout); */
void vaoAddBufferM(VAO_t *vao, VBO_t vbo[], VBLayout_t *layout);
void vaoDelete(VAO_t *vao);

#if defined(__cplusplus)
}
#endif


#endif // VAO_H
