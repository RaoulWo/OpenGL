#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define ASSERT(x) if (!(x)) __debugbreak();
#define CallGl(x) ClearGlError();\
  x;\
  ASSERT(LogGlCall(#x, __FILE__, __LINE__))

static void ClearGlError() {
  while (glGetError() != GL_NO_ERROR) {

  }
}

static bool LogGlCall(const char* function, const char* file, int line) {
  while (GLenum error = glGetError()) {
    std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

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
  
  // Create vertex buffer
  unsigned int buffer;
  CallGl(glGenBuffers(1, &buffer));
  CallGl(glBindBuffer(GL_ARRAY_BUFFER, buffer));
  CallGl(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

  CallGl(glEnableVertexAttribArray(0));
  CallGl(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

  // Create index buffer
  unsigned int indexBufferObject;
  CallGl(glGenBuffers(1, &indexBufferObject));
  CallGl(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject));
  CallGl(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

  ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
  unsigned int shader = CreateShader(source.vertex_source, source.fragment_source);
  CallGl(glUseProgram(shader));

  CallGl(int location = glGetUniformLocation(shader, "u_color"));
  ASSERT(location != -1);
  CallGl(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

  float red = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    CallGl(glUniform4f(location, red, 0.3f, 0.8f, 1.0f));
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

  glfwTerminate();
  return 0;
}