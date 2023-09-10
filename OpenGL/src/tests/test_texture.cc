#include "test_texture.h"

#include "renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Test {

TestTexture::TestTexture() : translation_a_(200, 200, 0), translation_b_(400, 200, 0) {
  const float positions[] = {
    //   X,      Y, texX, texY,
    -50.0f, -50.0f, 0.0f, 0.0f,  // 0
     50.0f, -50.0f, 1.0f, 0.0f,  // 1
     50.0f,  50.0f, 1.0f, 1.0f,  // 2
    -50.0f,  50.0f, 0.0f, 1.0f   // 3
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  CallGl(glEnable(GL_BLEND));
  CallGl(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  vertex_array_ = std::make_unique<VertexArray>();
  vertex_buffer_ = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);

  vertex_array_->AddBuffer(*vertex_buffer_, layout);

  // Create index buffer
  index_buffer_ = std::make_unique<IndexBuffer>(indices, 6);

  proj_ = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
  view_ = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

  shader_ = std::make_unique<Shader>("res/shaders/basic.shader");
  shader_->Bind();

  texture_ = std::make_unique<Texture>("res/textures/settings.png");
  shader_->SetUniform1i("u_tex", 0);
}

TestTexture::~TestTexture() {

}

void TestTexture::OnUpdate(float deltaTime) {

}

void TestTexture::OnRender() {
  CallGl(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
  CallGl(glClear(GL_COLOR_BUFFER_BIT));

  Renderer renderer;

  texture_->Bind();

  {
    glm::mat4 modelMatrix1 = glm::translate(glm::mat4(1.0f), translation_a_);
    // Reverse order because OpenGL expects matrices in column-major layout
    glm::mat4 modelViewproj_1 = proj_ * view_ * modelMatrix1;
    shader_->Bind();
    shader_->SetUniformMat4f("u_model_view_proj_matrix", modelViewproj_1);
    renderer.Draw(*vertex_array_, *index_buffer_, *shader_);
  }

  {
    glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), translation_b_);
    // Reverse order because OpenGL expects matrices in column-major layout
    glm::mat4 modelViewproj_2 = proj_ * view_ * modelMatrix2;
    shader_->Bind();
    shader_->SetUniformMat4f("u_model_view_proj_matrix", modelViewproj_2);
    renderer.Draw(*vertex_array_, *index_buffer_, *shader_);
  }

}

void TestTexture::OnImGuiRender() {
  ImGui::SliderFloat3("Translation A", &translation_a_.x, 0.0f, 960.0f);
  ImGui::SliderFloat3("Translation B", &translation_b_.x, 0.0f, 960.0f);
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

}  // Test