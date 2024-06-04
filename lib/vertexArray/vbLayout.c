#include <stdio.h>
#include <stdlib.h>

#include "vbLayout.h"

#define VBL_ARR_DEFAULT_SIZE 9


VBLayout_status_en vbl_new(VBLayout_t *vbl, uint32_t stride)
{
	/* if(vbl != NULL) { */
	/* 	return VBL_ARR_SHOULD_BE_EMPTY; */
	/* } */

	vbl->stride = stride;
	vbl->capacity = VBL_ARR_DEFAULT_SIZE;
	vbl->size = 0;
	vbl->elements = malloc(VBL_ARR_DEFAULT_SIZE * sizeof(VBLayout_Element_t));

	return VBL_STATUS_OK;
}

void vbl_destroy(VBLayout_t *vbl)
{
	free(vbl->elements);
}

VBLayout_status_en vbl_push_float(VBLayout_t *vbl, int32_t count)
{
	if(vbl == NULL) {
		return VBL_ARR_EMPTY_ARR;
	}

	// Check if full array to reallocate
	if(vbl->size == vbl->capacity) {
		vbl->capacity *= 2;
		vbl->elements = realloc(vbl->elements, (vbl->capacity) * sizeof(VBLayout_Element_t));
	}

	// Add new element
	vbl->elements[vbl->size] = (VBLayout_Element_t){
		.type=GL_FLOAT,
		.count=count,
		.normalized=GL_FALSE,
		.typeSize=sizeof(float)
	};
	// Then increament the size
	++vbl->size;

	return VBL_STATUS_OK;
}

VBLayout_status_en vbl_push_uint(VBLayout_t *vbl, int32_t count)
{
	if(vbl == NULL) {
		return VBL_ARR_EMPTY_ARR;
	}

	// Check if full array to reallocate
	if(vbl->size == vbl->capacity) {
		vbl->capacity *= 2;
		vbl->elements = realloc(vbl->elements, vbl->capacity * sizeof(VBLayout_Element_t));
	}

	// Add new element
	vbl->elements[vbl->size] = (VBLayout_Element_t){
		.type=GL_UNSIGNED_INT,
		.count=count,
		.normalized=GL_FALSE,
		.typeSize=sizeof(uint32_t)
	};
	// Then increament the size
	++vbl->size;

	return VBL_STATUS_OK;
}

VBLayout_status_en vbl_push_uchar(VBLayout_t *vbl, int32_t count)
{
	if(vbl == NULL) {
		return VBL_ARR_EMPTY_ARR;
	}

	// Check if full array to reallocate
	if(vbl->size == vbl->capacity) {
		vbl->capacity *= 2;
		vbl->elements = realloc(vbl->elements, vbl->capacity * sizeof(VBLayout_Element_t));
	}

	// Add new element
	vbl->elements[vbl->size] = (VBLayout_Element_t){
		.type=GL_UNSIGNED_BYTE,
		.count=count,
		.normalized=GL_TRUE,
		.typeSize=sizeof(unsigned char)
	};
	// Then increament the size
	++vbl->size;

	return VBL_STATUS_OK;
}
