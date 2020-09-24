#include "Game.h"

Game::Game(int width, int height, HDC hDC) : paddle_(), shader_("../../source/shaders/vertex.glsl", "../../source/shaders/fragment.glsl")
{
  hDC_ = hDC;

  shader_.use();

  view_matrix_ = glm::mat4(1.0f); //"camera"
  view_matrix_ = glm::translate(view_matrix_, glm::vec3(0.0f, 0.0f, -3.0f)); //move everything in scene 3 units deep into the monitor so we can see it

  shader_.setUniform("view", view_matrix_);

  //viewport and perspective matrix settings
  ResizeScene(width, height);

  //persp once calculated stays valid until window resizing
  projection_matrix_ = glm::perspective(glm::radians(45.0f),
    static_cast<float>(screen_width_) / static_cast<float>(screen_height_), 0.1f, 1000.0f);

  
}

Game::~Game()
{
  
}

void Game::RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(0.25f, 0.5f, 0.75f, 0.0f);

  //set up shader uniforms

  //shader_.setUniform("view", view_matrix_); //camera stays in place so why bother to update since there is only one shader? (for now hehe)
  //shader_.setUniform("projection", projection_matrix_); //changes only if window was resized, moved to ResizeScene (idk if this should be like that

  //render paddle
  paddle_.Draw(&shader_);
  

  SwapBuffers(hDC_);
  glFlush();//? idk if its necessary
}

void Game::ResizeScene(int width, int height)
{
  screen_width_ = width;
  screen_height_ = height;

  glViewport(0, 0, screen_width_, screen_height_);

  projection_matrix_ = glm::perspective(glm::radians(45.0f),
    static_cast<float>(screen_width_) / screen_height_, 0.1f, 1000.0f);

  shader_.setUniform("projection", projection_matrix_);
}
