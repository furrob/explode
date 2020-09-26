#include "Game.h"


Game::Game(int width, int height, HDC hDC) : shader_("../../source/shaders/vertex.glsl", "../../source/shaders/fragment.glsl")
{
  
  hDC_ = hDC;

  shader_.use();

  view_matrix_ = glm::mat4(1.0f); //"camera"
  view_matrix_ = glm::translate(view_matrix_, glm::vec3(0.0f, 0.0f, 0.0f)); //move everything in scene 5 units deep into the monitor so we can see it

  shader_.setUniform("view", view_matrix_);

  //viewport and perspective matrix settings
  ResizeScene(width, height);

  //persp once calculated stays valid until window resizing
  projection_matrix_ = glm::perspective(glm::radians(CAM_FOV),
    static_cast<float>(screen_width_) / static_cast<float>(screen_height_), CAM_Z_NEAR, CAM_Z_FAR);

  
}

Game::~Game()
{
  delete paddle_;
}

void Game::Initialize()
{
  //ALL PATHS DEFINED HERE, maybe pull it somewhere else later idk
  paddle_ = new Paddle("./models/handPaddle.obj", "./textures/test.png");
  sound = SM.addSound("sounds/sweethome.raw");
}

void Game::OnMouseMove(int _x, int _y)
{
  //x and y relative to upper-left corner of window's client area
  //map x and y to screen coords (?) like <-1;1> or something

  float x = (_x - (screen_width_ / 2.0f)) / (screen_width_ / 2.0f);
  float y = (_y - (screen_height_ / 2.0f)) / (screen_height_ / 2.0f);

  float x_pos = (x * CAM_Z_NEAR) * abs(PADDLE_Z);
  float y_pos = (y * CAM_Z_NEAR) * abs(PADDLE_Z);

  int xD = projection_matrix_.length();

  paddle_->set_position(glm::vec3(x_pos + 1.5f, -y_pos, PADDLE_Z));
  //paddle_->set_position(glm::vec3(6 * x + 1.5f, 3 * -y, PADDLE_Z));
}

void Game::Update(double elapsed_time)
{
  //TEST
  if((GetAsyncKeyState(VK_F2)&0x8000)!=0)
  {
    SM.playSound(sound);
  }
  
  //TEST
}

void Game::RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(1.0f, 0.5f, 0.25f, 0.0f);

  //set up shader uniforms

  //shader_.setUniform("view", view_matrix_); //camera stays in place so why bother to update since there is only one shader? (for now hehe)
  //shader_.setUniform("projection", projection_matrix_); //changes only if window was resized, moved to ResizeScene (idk if this should be like that

  //render paddle
  paddle_->Draw(&shader_);
  

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
