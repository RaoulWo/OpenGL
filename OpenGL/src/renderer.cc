#include "renderer.h"

#include <iostream>

void ClearGlError() {
  while (glGetError() != GL_NO_ERROR) {

  }
}

bool LogGlCall(const char* function, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}