#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg_assert.h"

#define ERROR_STR_S 200

void __dbg_assert(const char *expr, const char *file, int line, const char *func)
{
	fprintf(stderr, "\033[0;31m[ERROR]\033[0m::ASSERT_FAILED: %s\n\tFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\nEXITING...\n", expr, file, line, func);
	exit(-1);
}

void __dbg_log(const char *msg, const char *file, int line, const char *func, errorL_t level)
{
	switch(level) {
	case LOG:
		fprintf(stdout, "[LOG]::%s\n\tFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\n", msg, file, line, func);
		break;
	case WARNING:
		fprintf(stderr, "\033[0;33m[WARNING]\033[0m::%s\n\tFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\n", msg, file, line, func);
		break;
	case ERROR:
		fprintf(stderr, "\033[0;31m[ERROR]\033[0m::%s\n\tFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\n", msg, file, line, func);
		break;
	}
}

GLenum __dbg_glCheckError(const char *file, int line, const char *func)
{
	GLenum errorCode;

	while((errorCode = glGetError()) != GL_NO_ERROR) {
		char error[ERROR_STR_S];

		switch(errorCode) {
                case GL_INVALID_ENUM:
			strncpy(error, "INVALID_ENUM", ERROR_STR_S);
			break;
		case GL_INVALID_VALUE:
			strncpy(error, "INVALID_VALUE", ERROR_STR_S);
			break;
		case GL_INVALID_OPERATION:
			strncpy(error, "INVALID_OPERATION", ERROR_STR_S);
			break;
		case GL_STACK_OVERFLOW:
			strncpy(error, "STACK_OVERFLOW", ERROR_STR_S);
			break;
		case GL_STACK_UNDERFLOW:
			strncpy(error, "STACK_UNDERFLOW", ERROR_STR_S);
			break;
		case GL_OUT_OF_MEMORY:
			strncpy(error, "OUT_OF_MEMORY", ERROR_STR_S);
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			strncpy(error, "INVALID_FRAMEBUFFER_OPERATION", ERROR_STR_S);
			break;
		}
		fprintf(stderr, "\033[0;31m[ERROR]\033[0m::%s\n\tFILE: %s\n\tLINE: %d\n\tFUNCTION: %s\n", error, file, line, func);
	}
	return errorCode;
}
