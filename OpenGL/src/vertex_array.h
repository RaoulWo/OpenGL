#pragma once

#include "vertex_buffer.h"

class VertexBufferLayout;

class VertexArray {
 public:
  VertexArray();
  ~VertexArray();

  void Bind() const;
  void Unbind() const;
  void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);

 private:
  unsigned int renderer_id_;
};