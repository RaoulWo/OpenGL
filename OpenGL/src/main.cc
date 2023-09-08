#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "renderer.h"

#include "vertex_array.h"
#include "index_buffer.h"
#include "vertex_buffer.h"

struct ShaderProgramSource {
  std::string vertex_source;
  std::string fragment_source;
};

static ShaderProgramSource ParseShader(const std::string& path) {
  std::ifstream stream(path);

  enum class ShaderType {
    None = -1,
    Vertex = 0,
    Fragment = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::None;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos) {
        type = ShaderType::Vertex;
      } else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::Fragment;
      }
    } else {
      ss[(int)type] << line << '\n';
    }
  }

  return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
  CallGl(unsigned int id = glCreateShader(type));
  const char* src = source.c_str();
  CallGl(glShaderSource(id, 1, &src, nullptr));
  CallGl(glCompileShader(id));

  int result;
  CallGl(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE) {
    int length;
    CallGl(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char));
    CallGl(glGetShaderInfoLog(id, length, &length, message));

    std::cout << "Failed to compile shader" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
    std::cout << message << std::endl;

    CallGl(glDeleteShader(id));

    return 0;
  }

  return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
  CallGl(unsigned int program = glCreateProgram());
  unsigned int compiledVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int compiledFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  CallGl(glAttachShader(program, compiledVertexShader));
  CallGl(glAttachShader(program, compiledFragmentShader));
  CallGl(glLinkProgram(program));
  CallGl(glValidateProgram(program));

  CallGl(glDeleteShader(compiledVertexShader));
  CallGl(glDeleteShader(compiledFragmentShader));

  return program;
}

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

  VertexBuffer* vertexBuffer = new VertexBuffer(positions, 4 * 2 * sizeof(float));
  VertexBufferLayout layout;
  layout.Push<float>(2);

  VertexArray vertexArray;
  vertexArray.AddBuffer(*vertexBuffer, layout);

  // Create index buffer
  IndexBuffer* indexBuffer = new IndexBuffer(indices, 6);

  ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
  unsigned int shader = CreateShader(source.vertex_source, source.fragment_source);
  CallGl(glUseProgram(shader));

  CallGl(int location = glGetUniformLocation(shader, "u_color"));
  ASSERT(location != -1);
  CallGl(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

  CallGl(glBindVertexArray(0));
  CallGl(glUseProgram(0));
  CallGl(glBindBuffer(GL_ARRAY_BUFFER, 0));
  CallGl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


  float red = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    CallGl(glUseProgram(shader));
    CallGl(glUniform4f(location, red, 0.3f, 0.8f, 1.0f));

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

  CallGl(glDeleteProgram(shader));

  delete vertexBuffer;
  delete indexBuffer;

  glfwTerminate();
  return 0;
}