#pragma once

#include <iostream>
#include <assert.h>

/* 'GL_CHECK_ERRORS' macros all around the code are intended to check the current
 * error bit for any error which might be raised by passing invalid args to an
 * OpenGL function, or when there is some problem with the OpenGL state machine.
 *
 * For any such error, the macro traps it and generates a debug assertion.
 * Since this macro calls 'glGetError()' inside a debug assert, it is stripped
 * from the 'Release' build. */
#ifdef NDEBUG

#define GL_CHECK_ERRORS ((void)0)

#else

//#define GL_CHECK_ERRORS assert(glGetError() == GL_NO_ERROR)
#define GL_CHECK_ERRORS _glCheckErrors(_CRT_WIDE(__FILE__), __LINE__)

inline void _glProcessError(GLenum err, const wchar_t *file, int line)
{
	std::cerr << gluErrorString(err) << " at \"";
	std::wcerr << file << ':' << line << "\"\n";
	_wassert(L"OpenGL error", file, line);
}

inline void _glCheckErrors(const wchar_t *file, int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) _glProcessError(err, file, line);
}

#endif