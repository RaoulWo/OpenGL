#include "shader.h"

#include "renderer.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const std::string& file) : file_(file), renderer_id_(0) {
  ShaderProgramSource source = ParseShader(file);
  renderer_id_ = CreateShader(source.vertex_source, source.fragment_source);
}

Shader::~Shader() {
  CallGl(glDeleteProgram(renderer_id_));
}

void Shader::Bind() const {
  CallGl(glUseProgram(renderer_id_));
}

void Shader::Unbind() const {
  CallGl(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4) {
  CallGl(glUniform4f(GetUniformLocation(name), v1, v2, v3, v4));
}

ShaderProgramSource Shader::ParseShader(const std::string& path) {
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
      }
      else if (line.find("fragment") != std::string::npos) {
        type = ShaderType::Fragment;
      }
    }
    else {
      ss[(int)type] << line << '\n';
    }
  }

  return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
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

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

int Shader::GetUniformLocation(const std::string& name) {
  if (uniform_location_cache_.find(name) != uniform_location_cache_.end()) {
    return uniform_location_cache_[name];
  }

  CallGl(int location = glGetUniformLocation(renderer_id_, name.c_str()));
  if (location == -1) {
    std::cout << "Warning: uniform'" << name << "' doesn't exist!" << std::endl;
  }
  uniform_location_cache_[name] = location;
  
  return location;
}
