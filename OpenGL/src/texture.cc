#include "texture.h"

#include "stb_image/stb_image.h"

#include <iostream>

Texture::Texture(const std::string& path) 
  : renderer_id_(0), path_(path), local_buffer_(nullptr), width_(0), height_(0), bytes_per_pixel_(0) {
  stbi_set_flip_vertically_on_load(1);
  local_buffer_ = stbi_load(path_.c_str(), &width_, &height_, &bytes_per_pixel_, 4);

  CallGl(glGenTextures(1, &renderer_id_));
  CallGl(glBindTexture(GL_TEXTURE_2D, renderer_id_));

  CallGl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  CallGl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  CallGl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  CallGl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  CallGl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer_));
  CallGl(glBindTexture(GL_TEXTURE_2D, 0));

  if (local_buffer_) {
    stbi_image_free(local_buffer_);
  }
}

Texture::~Texture() {
  CallGl(glDeleteTextures(1, &renderer_id_));
}

void Texture::Bind(unsigned int slot) const {
  CallGl(glActiveTexture(GL_TEXTURE0 + slot));
  CallGl(glBindTexture(GL_TEXTURE_2D, renderer_id_));
}

void Texture::Unbind() const {
  CallGl(glBindTexture(GL_TEXTURE_2D, 0));
}
