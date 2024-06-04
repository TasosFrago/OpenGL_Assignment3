#include <stdio.h>
#include <stdlib.h>

#include "windowGlfw.h"
#include "dbg_assert.h"


void FrameBufferSizeCallback(GLFWwindow *fun, int width, int height)
{
	glViewport(0, 0, width, height);
}

void error_callback(int error, const char *description)
{
	DBG_LOG(description, ERROR);
}

void createWindow(window_t *window)
{
	window->win_ptr = 0;
	DBG_ASSERT(window->width > 100 && window->height >100);

	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize glfw.\n");
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window->win_ptr = glfwCreateWindow(window->width, window->height, "OpenGL Erg", NULL, NULL);

	if(!window) {
		fprintf(stderr, "Failed to initialize window.\n");
		exit(-1);
	}

	glfwMakeContextCurrent(window->win_ptr);

	glfwSetFramebufferSizeCallback(window->win_ptr, FrameBufferSizeCallback);

	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize glew.\n");
		glfwTerminate();
		exit(-1);
	}

	glfwSetErrorCallback(error_callback);
}

void destroyWindow(window_t *window)
{
	if(window) {
		glfwDestroyWindow(window->win_ptr);
		glfwTerminate();
	}
}
