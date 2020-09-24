#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "vertex.h"
#include "Shader.h"

class Paddle
{
private:
  //Vertex Array Buffer (big box)
  GLuint vao_;

  //Vetrex Buffer Object
  GLuint vbo_;

  //Element Buffer Array
  GLuint ebo_;

  //number of elements in indices array
  GLuint indices_count_;

  glm::mat4 model_matrix_;

  glm::vec3 position_; //position of this thing

public:
  Paddle();

  void Draw(Shader* shader);

  ~Paddle();
};

