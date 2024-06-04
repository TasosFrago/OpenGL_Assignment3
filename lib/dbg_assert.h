#ifndef DBG_ASSERT_H
#define DBG_ASSERT_H

#ifndef GLEW_GUARD_H
#define GLEW_GUARD_H
#include <GL/glew.h>
#endif // GLEW_GUARD_H

#if !defined(__cplusplus) && !defined(bool) && !defined(SP_BOOL_TYPE)
	typedef enum bool { false = 0, true = !false } bool;
	#define SP_BOOL_TYPE
#endif

#ifdef DEBUG
	#define DBG_ASSERT(expr) \
		(void)((expr) || (__dbg_assert(#expr, __FILE__, __LINE__, __func__), 0) )
	#define DBG_GLCHECKERROR() \
		__dbg_glCheckError(__FILE__, __LINE__, __func__);
#else
	#define DBG_ASSERT(expr) (void)0
	#define DBG_GLCHECKERROR() (void)0
#endif

#define DBG_LOG(msg, level) \
	__dbg_log(msg, __FILE__, __LINE__, __func__, level)


#if defined(__cplusplus)
extern "C" {
#endif

typedef enum {
	LOG,
	WARNING,
	ERROR
} errorL_t;

void __dbg_assert(const char *expr, const char *file, int line, const char *func);
void __dbg_log(const char *msg, const char *file, int line, const char *func, errorL_t level);
GLenum __dbg_glCheckError(const char *file, int line, const char *func);

#if defined(__cplusplus)
}
#endif

#endif // DBG_ASSERT_H
