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



class Ball
{
private:
  Mesh mesh_;

  GLuint texture_main_;

  void LoadTexture(const char* texturePath);

public:
  glm::mat4 model_matrix_;

  glm::vec3 scale_;

  glm::vec3 position_; //position of this thing

  glm::vec3 velocity_;

  //acceleration only in x-y plane
  glm::vec2 acceleration_;

  Ball(const char* meshPath, const char* texturePath);

  //~Ball();
  //returns position after update
  void Update(double elapsed_time);

  void Draw(Shader* shader);

};

