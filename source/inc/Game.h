#pragma once

#include <GL/glew.h>
#include <Windows.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Paddle.h"
#include "Shader.h"

class Game
{
private:
  glm::mat4 view_matrix_;
  glm::mat4 projection_matrix_;

  int screen_width_;
  int screen_height_;

  HDC hDC_; //handle to device context, needed for rendering

  Paddle paddle_;

  Shader shader_;

public:

  Game(int width, int height, HDC hDC);

  ~Game();

  //set everything that will be necessary 
  //void Initialize();

  //update everything time-related and render scene
  void Update();

  void RenderScene();

  //uptade viewport / handle resizing
  void ResizeScene(int width, int height);
};

