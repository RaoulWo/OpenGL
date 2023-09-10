#pragma once

#include <memory>

#include "test.h"
#include "texture.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

namespace Test {

class TestTexture : public Test {
 public:
  TestTexture();
  ~TestTexture();

  void OnUpdate(float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
 private:
  std::unique_ptr<VertexArray> vertex_array_;
  std::unique_ptr<VertexBuffer> vertex_buffer_;
  std::unique_ptr<IndexBuffer> index_buffer_;
  std::unique_ptr<Shader> shader_;
  std::unique_ptr<Texture> texture_;

  glm::mat4 proj_, view_;
  glm::vec3 translation_a_, translation_b_;
};

}  // Test