#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource {
  std::string vertex_source;
  std::string fragment_source;
};

class Shader {
 public:
  Shader(const std::string& file);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void SetUniform1i(const std::string& name, int value);
  void SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
 private:
  ShaderProgramSource ParseShader(const std::string& path);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
  int GetUniformLocation(const std::string& name);

  unsigned int renderer_id_;
  std::string file_;
  std::unordered_map<std::string, int> uniform_location_cache_;
};