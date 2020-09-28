#pragma once

#include <glew.h>

#include "Shader.h"
#include "vertex.h"
#include <stb_image.h>

class Walls
{
private:
  GLuint vao_ = 0, vbo_ = 0, ebo_ = 0;
  GLuint indices_count_;

  glm::mat4 model_matrix_;


  GLuint texture_main_;

  void LoadTexture(const char* texturePath);

public:
  Walls(float front_depth, float back_depth, float half_width, float half_height, const char* texture_path);

  void Draw(Shader* shader);
};

