#ifndef SHADER_UTL_H
#define SHADER_UTL_H

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

#if(!defined(__cplusplus) && !defined(bool) && !defined(SP_BOOL_TYPE))
	typedef enum bool { false = 0, true = !false } bool;
	#define SP_BOOL_TYPE
#endif

#if defined(__cplusplus)
extern "C" {
#endif


GLuint shaderLoadProgram(const char *vertexPath, const char *fragmentPath);
GLint shaderValidate(GLuint program);

#define shaderUse(shader_ID) glUseProgram(shader_ID);


#if defined(__cplusplus)
}
#endif
#endif // SHADER_UTL_H
