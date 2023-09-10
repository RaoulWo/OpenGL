#pragma once

#include <functional>
#include <vector>
#include <string>
#include <tuple>

namespace Test {

class Test {
 public:
  Test() {}
  virtual ~Test() {}

  virtual void OnUpdate(float deltaTime) {}
  virtual void OnRender() {}
  virtual void OnImGuiRender() {}
};

class TestMenu : public Test {
 public:
   TestMenu(Test*& current);

  void OnUpdate(float deltaTime) override;
  void OnRender() override;
  void OnImGuiRender() override;

  template<typename T>
  void RegisterTest(const std::string& name) {
    tests_.push_back(std::make_pair(name, []() { return new T(); }));
  }
 private:
  std::vector<std::pair<std::string, std::function<Test*()>>> tests_;
  Test*& current_;
};

}  // Test