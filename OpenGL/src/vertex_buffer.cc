#include "vertex_buffer.h"

#include "renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
  CallGl(glGenBuffers(1, &renderer_id_));
  CallGl(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
  CallGl(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer() {
  CallGl(glDeleteBuffers(1, &renderer_id_));
}

void VertexBuffer::Bind() const {
  CallGl(glBindBuffer(GL_ARRAY_BUFFER, renderer_id_));
}

void VertexBuffer::Unbind() const {
  CallGl(glBindBuffer(GL_ARRAY_BUFFER, 0));
}