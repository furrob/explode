#include "Game.h"


Game::Game(int width, int height, HDC hDC) : shader_("../../source/shaders/vertex.glsl", "../../source/shaders/fragment.glsl")
{
  
  hDC_ = hDC;

  shader_.use();

  view_matrix_ = glm::mat4(1.0f); //"camera"
  view_matrix_ = glm::translate(view_matrix_, glm::vec3(0.0f, 0.0f, -1.0f)); //camera at origin

  shader_.setUniform("view", view_matrix_);

  //viewport and perspective matrix settings
  ResizeScene(width, height);
}

Game::~Game()
{
  delete paddle_;
  delete enemy_paddle_;
  delete ball_;
  delete walls_;
}

void Game::Initialize()
{
  //ALL PATHS DEFINED HERE, maybe pull it somewhere else later idk
  paddle_ = new Paddle("./models/paddle.obj", "./textures/texture.png");

  enemy_paddle_ = new Paddle("./models/paddle.obj", "./textures/texture.png");
  enemy_paddle_->y_rotation_ = 180.0f;
  enemy_paddle_->set_position(glm::vec3(0.0f, 0.0f, BACK_WALL));

  ball_ = new Ball("./models/ball.obj", "./textures/test.png");

  walls_ = new Walls(PADDLE_Z, BACK_WALL, PADDLE_MAX_X, PADDLE_MAX_Y);
}

void Game::OnMouseMove(int _x, int _y)
{
  //x and y relative to upper-left corner of window's client area
  //map x and y to screen coords (?) like <-1;1> or something
  float x = (_x - (screen_width_ / 2.0f)) / (screen_width_ / 2.0f);
  float y = (_y - (screen_height_ / 2.0f)) / (screen_height_ / 2.0f);

  float x_pos = x * PADDLE_MAX_X;
  float y_pos = -y * PADDLE_MAX_Y;

  x_pos = (x_pos + paddle_->body_.right > PADDLE_MAX_X) ? PADDLE_MAX_X - paddle_->body_.right : x_pos;
  x_pos = (x_pos - paddle_->body_.left < -PADDLE_MAX_X) ? -PADDLE_MAX_X + paddle_->body_.right : x_pos;
  y_pos = (y_pos + paddle_->body_.top > PADDLE_MAX_Y) ? PADDLE_MAX_Y - paddle_->body_.top : y_pos;
  y_pos = (y_pos - paddle_->body_.bottom < -PADDLE_MAX_Y) ? -PADDLE_MAX_Y + paddle_->body_.bottom : y_pos;

  paddle_->set_position(glm::vec3(x_pos, y_pos, PADDLE_Z));
}

void Game::Update(double elapsed_time)
{
  //keyboard
  if((GetAsyncKeyState(VK_TAB) & 0x8000) != 0)
  {
    ball_->velocity_ = glm::vec3(0.0f, 0.0f, -20.0f);
  }

  //update ball position, velocity etc.
  ball_->position_ += ball_->velocity_ * (float)elapsed_time;

  //enemy paddle AI ______________________________________________________________________________________________________________________Warning
  float x_pos= ball_->position_.x;
  float y_pos= ball_->position_.y;
  //float speed_factor=5;
  float difficulty=1.2; //between 0 and 1.8
  float nearBonus;
  if (ball_->position_.z <= -25*difficulty) //_________________________________________________________________________________________Enemy paddle activates after ball reaches -25 z
  {
      nearBonus = (( ball_->position_.z/ BACK_WALL )*1.8)/(difficulty+0.01);
      float x = (ball_->position_.x - enemy_paddle_->position_.x) * 2.2 * (float)elapsed_time*nearBonus;
     // x = (abs(x) <= speed_factor * (float)elapsed_time) ? (x <= 0) ? -speed_factor * (float)elapsed_time : speed_factor * (float)elapsed_time : x;
      x_pos = enemy_paddle_->position_.x + x;
    
    
      float y = (ball_->position_.y - enemy_paddle_->position_.y) * 2.2 * (float)elapsed_time* nearBonus;
      //y = (abs(y) <= speed_factor * (float)elapsed_time) ? (y <= 0) ? -speed_factor * (float)elapsed_time : speed_factor * (float)elapsed_time : y;
      y_pos = enemy_paddle_->position_.y + y;
    

    x_pos = (x_pos + enemy_paddle_->body_.right > PADDLE_MAX_X) ? PADDLE_MAX_X - enemy_paddle_->body_.right : x_pos;
    x_pos = (x_pos - enemy_paddle_->body_.left < -PADDLE_MAX_X) ? -PADDLE_MAX_X + enemy_paddle_->body_.right : x_pos;
    y_pos = (y_pos + enemy_paddle_->body_.top > PADDLE_MAX_Y) ? PADDLE_MAX_Y - enemy_paddle_->body_.top : y_pos;
    y_pos = (y_pos - enemy_paddle_->body_.bottom < -PADDLE_MAX_Y) ? -PADDLE_MAX_Y + enemy_paddle_->body_.bottom : y_pos;
    enemy_paddle_->set_position(glm::vec3(x_pos, y_pos, BACK_WALL));
  }
  //_________________________________________________________________________________________//_________________________________________________________________________________________AI END
  //update ball matrices
  ball_->model_matrix_ = glm::translate(glm::mat4(1.0f), ball_->position_);

  //check if ball reached back wall
  if(ball_->position_.z - BALL_RADIUS < BACK_WALL)
  {
    ball_->velocity_.z = (ball_->velocity_.z) * -1.05f;
    ball_->position_.z = BACK_WALL + BALL_RADIUS;
  }
  else if(ball_->position_.z + BALL_RADIUS > PADDLE_Z) //if it reached paddle's depth
  {
    //check if it has hit the paddle (more-than-half-ball bounces are also valid)
    if(ball_->position_.x - BALL_RADIUS < paddle_->position_.x + paddle_->body_.right &&
      ball_->position_.x + BALL_RADIUS > paddle_->position_.x - paddle_->body_.left &&
      ball_->position_.y - BALL_RADIUS < paddle_->position_.y + paddle_->body_.top &&
      ball_->position_.y + BALL_RADIUS > paddle_->position_.y - paddle_->body_.bottom)
    {
      //HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT
      ball_->velocity_.z = -(ball_->velocity_.z);

      //calculate displacement between centers and modify ball velocity based on that displacement
      glm::vec2 bounce_velocity = glm::vec2(ball_->position_.x - paddle_->position_.x,
        ball_->position_.y - paddle_->position_.y) * (float)BOUNCE_DISPLACEMENT;

      ball_->velocity_.x += bounce_velocity.x;
      ball_->velocity_.y += bounce_velocity.y;

      ball_->position_.z = PADDLE_Z - BALL_RADIUS;
    }
    else //no hit :(
    {
      ball_->velocity_ = glm::vec3(0.0f);
      ball_->position_ = glm::vec3(0.0f, 0.0f, PADDLE_Z - 0.5f);
    }
  }

  //left side collision
  if(ball_->position_.x  - BALL_RADIUS < -PADDLE_MAX_X)
  {
    ball_->velocity_.x = -(ball_->velocity_.x);
    ball_->position_.x = -PADDLE_MAX_X + BALL_RADIUS;
  }
  //right side
  else if(ball_->position_.x + BALL_RADIUS > PADDLE_MAX_X)
  {
    ball_->velocity_.x = -(ball_->velocity_.x);
    ball_->position_.x = PADDLE_MAX_X - BALL_RADIUS;
  }

  //top collision
  if(ball_->position_.y + BALL_RADIUS > PADDLE_MAX_Y)
  {
    ball_->velocity_.y = -(ball_->velocity_.y);
    ball_->position_.y = PADDLE_MAX_Y - BALL_RADIUS;
  }
  //bottom collision
  else if(ball_->position_.y - BALL_RADIUS < -PADDLE_MAX_Y)
  {
    ball_->velocity_.y = -(ball_->velocity_.y);
    ball_->position_.y = -PADDLE_MAX_Y + BALL_RADIUS;
  }
  
  


}

void Game::RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(1.0f, 0.5f, 0.25f, 0.0f);

  //set up shader uniforms

  //shader_.setUniform("view", view_matrix_); //camera stays in place so why bother to update since there is only one shader? (for now hehe)
  //shader_.setUniform("projection", projection_matrix_); //changes only if window was resized, moved to ResizeScene (idk if this should be like that

  //walls
  shader_.setUniform("walls", true);
  walls_->Draw(&shader_);
  shader_.setUniform("walls", false);

  //enemy paddle
  enemy_paddle_->Draw(&shader_);

  //ball
  ball_->Draw(&shader_);

  //player paddle
  shader_.setUniform("player_paddle", true);
  paddle_->Draw(&shader_);
  shader_.setUniform("player_paddle", false);
  
  

  SwapBuffers(hDC_);
  glFlush();//? idk if its necessary
}

void Game::ResizeScene(int width, int height)
{
  screen_width_ = width;
  screen_height_ = height;

  glViewport(0, 0, screen_width_, screen_height_);

  projection_matrix_ = glm::perspective(glm::radians(CAM_FOV),
    static_cast<float>(screen_width_) / static_cast<float>(screen_height_), CAM_Z_NEAR, CAM_Z_FAR);

  shader_.setUniform("projection", projection_matrix_);
}
