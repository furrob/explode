#pragma once
#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <Windows.h>

class Shader
{
  GLuint ID;

public:
  
  Shader(const char* vertex_path,const char* fragment_path);

  ~Shader();

  void use() { glUseProgram(ID); }

  GLuint getID() const
  {
    return ID;
  }

  void setUniform(const std::string& name, bool value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
  }

  void setUniform(const std::string& name, int value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setUniform(const std::string& name, float value) const
  {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }

  void setUniform(const std::string& name, glm::vec3 value) const
  {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
  }

  void setUniform(const std::string& name, glm::mat4 value) const
  {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }


};
