#include "vertex_array.h"
#include "vertex_buffer_layout.h"

#include "renderer.h"

VertexArray::VertexArray() {
  CallGl(glGenVertexArrays(1, &renderer_id_));
}

VertexArray::~VertexArray() {
  CallGl(glDeleteVertexArrays(1, &renderer_id_));
}

void VertexArray::Bind() const {
  CallGl(glBindVertexArray(renderer_id_));
}

void VertexArray::Unbind() const {
  CallGl(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout) {
  Bind();
  buffer.Bind();
  const auto& elements = layout.elements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++) {
    const auto& element = elements[i];
    CallGl(glEnableVertexAttribArray(0));
    CallGl(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.stride(), (const void*)offset));
    offset += element.count * VertexBufferElement::SizeOf(element.type);
  }
}