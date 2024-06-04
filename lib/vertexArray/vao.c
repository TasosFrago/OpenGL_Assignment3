#include <stdio.h>
#include <stdint.h>

#include "vao.h"
#include "lib/dbg_assert.h"


void vaoGen(VAO_t *vao)
{
	glGenVertexArrays(1, vao);
}

void vaoBind(VAO_t *vao)
{
	glBindVertexArray(*vao);
}

void vaoAddBuffer(VAO_t *vao, VBO_t *vbo, VBLayout_t *layout)
{
	vaoBind(vao);
	vboBind(vbo);
	uint32_t offset = 0;
	for(uint32_t i = 0; i < layout->size; i++) {
		glEnableVertexAttribArray(i);
		DBG_GLCHECKERROR();
		glVertexAttribPointer(
			i,
			layout->elements[i].count,
			layout->elements[i].type,
			layout->elements[i].normalized,
			layout->stride,
			(const void *)(size_t)offset
			);
		DBG_GLCHECKERROR();
		offset += layout->elements[i].count * layout->elements[i].typeSize;
	}
}

void vaoAddBufferM(VAO_t *vao, VBO_t vbo[], VBLayout_t *layout)
{
	vaoBind(vao);
	for(uint32_t i = 0; i < layout->size; i++) {
		vboBind((vbo + i));
		glEnableVertexAttribArray(i);
		DBG_GLCHECKERROR();
		glVertexAttribPointer(
			i,
			layout->elements[i].count,
			layout->elements[i].type,
			layout->elements[i].normalized,
			layout->stride,
			(const void *)0
			);
		DBG_GLCHECKERROR();
	}
}

void vaoDelete(VAO_t *vao)
{
	glDeleteVertexArrays(1, vao);
}
