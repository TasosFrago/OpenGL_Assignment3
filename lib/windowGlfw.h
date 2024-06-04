#ifndef WINDOWGLFW_H
#define WINDOWGLFW_H

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H
#include <GLFW/glfw3.h>

#if !defined(__cplusplus) && !defined(bool) && !defined(SP_BOOL_TYPE)
	typedef enum bool { false = 0, true = !false } bool;
	#define SP_BOOL_TYPE
#endif

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
	GLFWwindow *win_ptr;
	int width;
	int height;
} window_t;

void createWindow(window_t *window);
void destroyWindow(window_t *window);

#if defined(__cplusplus)
}
#endif

#endif // WINDOWGLFW_H
