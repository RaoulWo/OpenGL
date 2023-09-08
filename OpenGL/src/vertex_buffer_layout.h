#pragma once

#include "renderer.h"

#include <stdexcept>
#include <vector>

struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int SizeOf(unsigned int type) {
    switch (type) {
      case GL_FLOAT:
        return sizeof(GLfloat);
      case GL_UNSIGNED_INT:
        return sizeof(GLuint);
      case GL_UNSIGNED_BYTE:
        return sizeof(GLbyte);
    }

    ASSERT(false);
    return 0;
  }
};

class VertexBufferLayout {
 public:
  VertexBufferLayout();

  template<typename T>
  void Push(unsigned int count) {
    std::runtime_error(false);
  }
  template<>
  void Push<float>(unsigned int count) {
    elements_.push_back({ GL_FLOAT, count, GL_FALSE });
    stride_ += count * VertexBufferElement::SizeOf(GL_FLOAT);
  }
  template<>
  void Push<unsigned int>(unsigned int count) {
    elements_.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
    stride_ += count * VertexBufferElement::SizeOf(GL_UNSIGNED_INT);
  }
  template<>
  void Push<unsigned char>(unsigned int count) {
    elements_.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
    stride_ += count * VertexBufferElement::SizeOf(GL_UNSIGNED_BYTE);
  }

  const std::vector<VertexBufferElement>& elements() const { return elements_; }
  unsigned int stride() const { return stride_; }

 private:
  std::vector<VertexBufferElement> elements_;
  unsigned int stride_;
};