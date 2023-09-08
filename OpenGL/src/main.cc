#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "renderer.h"
#include "shader.h"
#include "vertex_array.h"
#include "index_buffer.h"
#include "vertex_buffer.h"

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
    //  X,     Y, 
    -0.5f, -0.5f, // 0
     0.5f, -0.5f, // 1
     0.5f,  0.5f, // 2
    -0.5f,  0.5f  // 3
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
  };

  {

    VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);

    VertexArray vertexArray;
    vertexArray.AddBuffer(vertexBuffer, layout);

    // Create index buffer
    IndexBuffer indexBuffer(indices, 6);

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

    vertexArray.Unbind();
    shader.Unbind();
    vertexBuffer.Unbind();
    indexBuffer.Unbind();


    float red = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
      /* Render here */
      glClear(GL_COLOR_BUFFER_BIT);

      shader.Bind();
      shader.SetUniform4f("u_color", red, 0.3f, 0.8f, 1.0f);

      vertexArray.Bind();
      CallGl(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

      if (red > 1.0f)
        increment = -0.05f;
      else if (red < 0.0f)
        increment = 0.05f;

      red += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

  }

  glfwTerminate();
  return 0;
}