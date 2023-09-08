#include "index_buffer.h"

#include "renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : count_(count) {
  CallGl(glGenBuffers(1, &renderer_id_));
  CallGl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
  CallGl(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer() {
  CallGl(glDeleteBuffers(1, &renderer_id_));
}

void IndexBuffer::Bind() const {
  CallGl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_id_));
}

void IndexBuffer::Unbind() const {
  CallGl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}