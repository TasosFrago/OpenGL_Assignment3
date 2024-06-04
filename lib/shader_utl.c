#include <stdio.h>
#include <stdlib.h>

#include "shader_utl.h"
#include "dbg_assert.h"


// Internal Helper functions prototypes
static void __readFile(char **buffer, const char *filepath);
static GLuint __createShaderProgram(const char *vertexSrc, const char *fragmentSrc);
static GLuint __compileShader(GLenum shaderType, const char *src);

GLint shaderValidate(GLuint program)
{
	glValidateProgram(program);

	GLint isValid = false;
	glGetProgramiv(program, GL_COMPILE_STATUS, &isValid);
	if(isValid == GL_FALSE) {
		GLint maxlength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxlength);
		GLchar *errorLog = malloc(maxlength * sizeof(maxlength));

		glGetProgramInfoLog(program, maxlength, &maxlength, &errorLog[0]);

		fprintf(stderr, "\033[0;31m[ERROR]\033[0m::Failed to validate shader::%s\n", errorLog);
		free(errorLog);
	}
	return isValid;
}

GLuint shaderLoadProgram(const char *vertexPath, const char *fragmentPath)
{
	// Create vertexSrc pointer to pass
	char *vertexSrc = NULL;
	__readFile(&vertexSrc, vertexPath); // Get the code from the file

	// Create fragmentSrc pointer to pass
	char *fragmentSrc = NULL;
	__readFile(&fragmentSrc, fragmentPath); // Get the code from the file
	/* int vLength; */
	/* int fLength; */

	/* char *vertexSrc = __loadFile(vertexPath, &vLength); */
	/* char *fragmentSrc = __loadFile(fragmentPath, &fLength); */


	// Check return for errors
	if(vertexSrc == NULL || fragmentSrc == NULL) {
		fprintf(stderr, "\033[0;31m[ERROR]\033[0m::Didn't read VertexShader or FragmentShader.\n");
		return 0;
	}

	// Create shader Program
	GLuint shaderProgram = __createShaderProgram(vertexSrc, fragmentSrc);
	DBG_ASSERT(shaderProgram != 0);

	// Free src pointers
	free(vertexSrc);
	free(fragmentSrc);

	return shaderProgram;
}

static void __readFile(char **buffer, const char *filepath)
{
	FILE *file = fopen(filepath, "rt");
	if(file == NULL) {
		fprintf(stderr, "\033[0;31m[ERROR]\033[0m::Failed to open file: %s\n", filepath);
		return;
	}

	// Get the size of the file by getting the location of the cursor at the end of the file
	fseek(file, 0, SEEK_END);
	size_t fileSize = ftell(file);
	// Move the cursor back at the begining
	fseek(file, 0, SEEK_SET);

	if(fileSize > 0) {
		*buffer = (char *)malloc((fileSize + 2) * sizeof(char));
		if(*buffer == NULL) {
			fprintf(stderr, "\033[0;31m[ERROR]\033[0m::Failed to allocate memory for file: %s\n", filepath);
			return;
		}
		// Read file
		size_t count = fread(*buffer, sizeof(char), fileSize, file);

		// Correct buffer size accounting for \r\n characters
		if(count < fileSize) {
			*buffer = realloc(*buffer, (count + 2) * sizeof(char));
		}

		// Terminate buffer
		(*buffer)[count] = '\0';
	}
	fclose(file);
}

static GLuint __createShaderProgram(const char *vertexSrc, const char *fragmentSrc)
{
	// Compile Shaders
	GLuint vertexShader = __compileShader(GL_VERTEX_SHADER, vertexSrc);
	GLuint fragmentShader = __compileShader(GL_FRAGMENT_SHADER, fragmentSrc);

	// Check return for errors
	if(vertexShader == 0 || fragmentShader == 0) {
		return 0;
	}

	// Create Program
	GLuint shaderProgram = glCreateProgram();
	DBG_ASSERT(shaderProgram != 0);

	// Attch the shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Link the shaders
	glLinkProgram(shaderProgram);

	// Chech for linking errors
	GLint isLinked = false;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE) {
		GLint maxlength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxlength);
		GLchar *errorLog = malloc(maxlength * sizeof(maxlength));

		glGetProgramInfoLog(shaderProgram, maxlength, &maxlength, &errorLog[0]);

		fprintf(stderr, "\033[0;31m[ERROR]\033[0m::Failed to link shaders::%s\n", errorLog);
		free(errorLog);

		glDeleteProgram(shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
	}

	// Delete the shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

static GLuint __compileShader(GLenum shaderType, const char *src)
{
	GLuint shader = glCreateShader(shaderType);
	DBG_ASSERT(shader != 0);

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	GLint isCompiled = false;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE) {
		printf("%s", src);
		GLint maxlength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxlength);
		GLchar *errorLog = malloc(maxlength * sizeof(maxlength));

		glGetShaderInfoLog(shader, maxlength, &maxlength, &errorLog[0]);

		fprintf(stderr, "\033[0;31m[ERROR]\033[0m::Failed to compile shader::|%s\n", errorLog);
		free(errorLog);
		glDeleteShader(shader);
		return false;
	}
	return shader;
}
