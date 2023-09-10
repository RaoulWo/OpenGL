#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "renderer.h"
#include "texture.h"
#include "shader.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

#include "tests/test_clear_color.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main(void) {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(960, 540, "Hello, world!", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
    std::cout << "GLEW initialization error!" << std::endl;

  std::cout << glGetString(GL_VERSION) << std::endl;

  {
    CallGl(glEnable(GL_BLEND));
    CallGl(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();


    Test::Test* current = nullptr;
    Test::TestMenu* menu = new Test::TestMenu(current);
    current = menu;

    Test::TestClearColor test;

    menu->RegisterTest<Test::TestClearColor>("Clear Color");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      renderer.Clear();
      CallGl(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      if (current) {
        current->OnUpdate(0.0f);
        current->OnRender();

        ImGui::Begin("Test");

        if (current != menu && ImGui::Button("<-")) {
          delete current;
          current = menu;
        }
        current->OnImGuiRender();

        ImGui::End();
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

    delete current;
    if (current != menu) delete menu;
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}