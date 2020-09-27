#pragma once

#include <glew.h>

#include "Shader.h"
#include "vertex.h"

class Walls
{
private:
  GLuint vao_ = 0, vbo_ = 0, ebo_ = 0;
  GLuint indices_count_;

  glm::mat4 model_matrix_;
public:
  Walls(float front_depth, float back_depth, float half_width, float half_height);

  void Draw(Shader* shader);
};

