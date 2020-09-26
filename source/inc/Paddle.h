#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <Windows.h>

#include "stb_image.h"

#include "vertex.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJLoader.h"

class Paddle
{
private:
  glm::mat4 model_matrix_;

  glm::vec3 position_; //position of this thing

  Mesh mesh_;

  GLuint texture_main_;

  void LoadTexture(const char* texturePath);

public:
  

  Paddle(const char* meshPath, const char* texturePath);

  ~Paddle();

  void Draw(Shader* shader);

  void set_position(glm::vec3 pos);
};

