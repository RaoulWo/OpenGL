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

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const {
  shader.Bind();
  vertexArray.Bind();
  indexBuffer.Bind();
  CallGl(glDrawElements(GL_TRIANGLES, indexBuffer.count(), GL_UNSIGNED_INT, nullptr));
}