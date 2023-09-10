#include "test.h"
#include <imgui/imgui.h>

namespace Test {

TestMenu::TestMenu(Test*& current) : current_(current) {

}

void TestMenu::OnUpdate(float deltaTime) {

}

void TestMenu::OnRender() {
  
}

void TestMenu::OnImGuiRender() {
  for (auto& test : tests_) {
    if (ImGui::Button(test.first.c_str())) {
      current_ = test.second();
    }
  }
}

}  // Test