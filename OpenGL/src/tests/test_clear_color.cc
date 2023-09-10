#include "test_clear_color.h"

#include "imgui/imgui.h"

#include "renderer.h"

namespace Test {

TestClearColor::TestClearColor() : clear_color_{ 0.2f, 0.3f, 0.8f, 1.0f } {

}

TestClearColor::~TestClearColor() {

}

void TestClearColor::OnUpdate(float deltaTime) {

}

void TestClearColor::OnRender() {
  CallGl(glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]));
  CallGl(glClear(GL_COLOR_BUFFER_BIT));
}

void TestClearColor::OnImGuiRender() {
  ImGui::ColorEdit4("Clear Color", clear_color_);
}

}  // Test