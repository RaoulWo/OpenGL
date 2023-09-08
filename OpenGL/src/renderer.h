#pragma once

#include <GL/glew.h>

#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define CallGl(x) ClearGlError();\
  x;\
  ASSERT(LogGlCall(#x, __FILE__, __LINE__))

void ClearGlError();
bool LogGlCall(const char* function, const char* file, int line);

class Renderer {
 public:
   void Clear() const;
   void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
 private:
};