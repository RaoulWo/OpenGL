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

int main(void) {
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello, world!", NULL, NULL);
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
    //  X,     Y, texX, texY,
    -0.5f, -0.5f, 0.0f, 0.0f,  // 0
     0.5f, -0.5f, 1.0f, 0.0f,  // 1
     0.5f,  0.5f, 1.0f, 1.0f,  // 2
    -0.5f,  0.5f, 0.0f, 1.0f   // 3
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

    glm::mat4 projectionMatrix = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniformMat4f("u_model_view_proj_matrix", projectionMatrix);

    Texture texture("res/textures/settings.png");
    texture.Bind();
    shader.SetUniform1i("u_tex", 0);

    vertexArray.Unbind();
    shader.Unbind();
    vertexBuffer.Unbind();
    indexBuffer.Unbind();

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      renderer.Clear();

      shader.Bind();

      renderer.Draw(vertexArray, indexBuffer, shader);

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  }

  glfwTerminate();
  return 0;
}