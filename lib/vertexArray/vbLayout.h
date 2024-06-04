#ifndef VBLAYOUT_H
#define VBLAYOUT_H

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

#include <stdint.h>

#if (!defined(__cplusplus) && !defined(bool) && !defined(SP_BOOL_TYPE))
	typedef enum bool { false = 0, true = !false } bool;
	#define SP_BOOL_TYPE
#endif


#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
	GLenum type;
	int32_t count;
	GLboolean normalized;
	uint32_t typeSize;
} VBLayout_Element_t; // Vertex Buffer Layout Element

typedef struct {
	uint32_t stride;
	size_t size; // number of elements
	size_t capacity; // actual size of elements array
	VBLayout_Element_t *elements;
} VBLayout_t;

typedef enum {
	VBL_STATUS_OK,
	VBL_ARR_EMPTY_ARR,
	VBL_ARR_SHOULD_BE_EMPTY,
} VBLayout_status_en;

VBLayout_status_en vbl_new(VBLayout_t *vbl, uint32_t stride);
void vbl_destroy(VBLayout_t *vbl);

VBLayout_status_en vbl_push_float(VBLayout_t *vbl, int32_t count);
VBLayout_status_en vbl_push_uint(VBLayout_t *vbl, int32_t count);
VBLayout_status_en vbl_push_uchar(VBLayout_t *vbl, int32_t count);

#if defined(__cplusplus)
}
#endif


#endif // VBLAYOUT_H
