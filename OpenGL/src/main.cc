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

  {
    CallGl(glEnable(GL_BLEND));
    CallGl(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray vertexArray;
    VertexBuffer vertexBuffer(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    vertexArray.AddBuffer(vertexBuffer, layout);

    // Create index buffer
    IndexBuffer indexBuffer(indices, 6);

    glm::mat4 projectionMatrix = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

    
    Shader shader("res/shaders/basic.shader");
    shader.Bind();

    Texture texture("res/textures/settings.png");
    texture.Bind();
    shader.SetUniform1i("u_tex", 0);

    vertexArray.Unbind();
    shader.Unbind();
    vertexBuffer.Unbind();
    indexBuffer.Unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    ImGui::StyleColorsDark();

    glm::vec3 translation1(200, 200, 0);
    glm::vec3 translation2(400, 200, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      renderer.Clear();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      {
        glm::mat4 modelMatrix1 = glm::translate(glm::mat4(1.0f), translation1);
        // Reverse order because OpenGL expects matrices in column-major layout
        glm::mat4 modelViewProjectionMatrix1 = projectionMatrix * viewMatrix * modelMatrix1;
        shader.Bind();
        shader.SetUniformMat4f("u_model_view_proj_matrix", modelViewProjectionMatrix1);
        renderer.Draw(vertexArray, indexBuffer, shader);
      }

      {
        glm::mat4 modelMatrix2 = glm::translate(glm::mat4(1.0f), translation2);
        // Reverse order because OpenGL expects matrices in column-major layout
        glm::mat4 modelViewProjectionMatrix2 = projectionMatrix * viewMatrix * modelMatrix2;
        shader.Bind();
        shader.SetUniformMat4f("u_model_view_proj_matrix", modelViewProjectionMatrix2);
        renderer.Draw(vertexArray, indexBuffer, shader);
      }

      // ImGui window
      {
        ImGui::SliderFloat3("Translation A", &translation1.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &translation2.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}