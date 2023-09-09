#pragma once

#include "renderer.h"

class Texture {
 public:
   Texture(const std::string& path);
   ~Texture();

   void Bind(unsigned int slot = 0) const;
   void Unbind() const;

   inline int width() const { return width_; }
   inline int height() const { return height_; }

 private:
  unsigned int renderer_id_;
  std::string path_;
  unsigned char* local_buffer_;
  int width_, height_, bytes_per_pixel_;
};