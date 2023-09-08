#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define CallGl(x) ClearGlError();\
  x;\
  ASSERT(LogGlCall(#x, __FILE__, __LINE__))

void ClearGlError();
bool LogGlCall(const char* function, const char* file, int line);