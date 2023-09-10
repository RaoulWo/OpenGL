#pragma once

#include "test.h"

namespace Test {

class TestClearColor : public Test {
 public:
  TestClearColor();
  ~TestClearColor();

  void OnUpdate(float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;
 private:
  float clear_color_[4];
};

}  // Test