#include "Paddle.h"


void Paddle::LoadTexture(const char* texturePath)
{
  int image_width, image_height;

  unsigned char* image = stbi_load(texturePath, &image_width, &image_height, 0, 0);

  glGenTextures(1, &texture_main_);
  glBindTexture(GL_TEXTURE_2D, texture_main_);

  //if texcoords will exceed <0;1>, repeat it
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  //arguments talks for themselves i think
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  if(image)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    MessageBoxW(NULL, L"ERROR::TEXTURE_LOADING_FAILED", L"Error", MB_OK | MB_ICONERROR);
  }

  glActiveTexture(0);

  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(image);
}

Paddle::Paddle(const char* meshPath, const char* texturePath): mesh_(OBJLoader::loadOBJ(meshPath))
{
  model_matrix_ = glm::mat4(1.0f); //identity matrix (no scaling, 0 rotation and position of model at world origin)
  //test
  model_matrix_ = glm::translate(model_matrix_, glm::vec3(0.0f, -1.5f, -3.0f));

  //texture loading
  LoadTexture(texturePath);
}


void Paddle::Draw(Shader* shader)
{
  //set shader uniform
  shader->setUniform("model", model_matrix_);

  //bind mesh texture
  glBindTexture(GL_TEXTURE_2D, texture_main_);
  //draw mesh
  mesh_.Draw(*shader);

  //unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
  //unbind VAO
  glBindVertexArray(0);
}

void Paddle::set_position(glm::vec3 pos)
{
  position_ = pos;

  model_matrix_ = glm::identity<glm::mat4>();

  //translate to position
  model_matrix_ = glm::translate(model_matrix_, position_);
  // rotate to "paddle like" position
  model_matrix_ = glm::rotate(model_matrix_, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

Paddle::~Paddle()
{
  //delete shader_;
}
