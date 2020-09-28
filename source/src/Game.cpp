#include "Game.h"


Game::Game(HWND hWnd, int width, int height, HDC hDC) : shader_("../../source/shaders/vertex.glsl", "../../source/shaders/fragment.glsl")
{
  hWnd_ = hWnd;
  hDC_ = hDC;

  shader_.use();

  glm::vec3 view_pos(0.0f, 0.0f, -1.0f);

  view_matrix_ = glm::mat4(1.0f); //"camera"
  view_matrix_ = glm::translate(view_matrix_, view_pos); //camera at origin

  //for shading purposes
  shader_.setUniform("view_pos", view_pos);

  shader_.setUniform("view", view_matrix_);
  //light color in shader
  shader_.setUniform("light_color", glm::vec3(1.0f, 1.0f, 1.0f));

  //viewport and perspective matrix settings
  ResizeScene(width, height);
}

Game::~Game()
{
  delete paddle_;
  delete enemy_paddle_;
  delete music_box_;
  delete ball_;
  delete walls_;
}

void Game::Initialize()
{
  //ALL PATHS DEFINED HERE, maybe pull it somewhere else later idk
  paddle_ = new Paddle("./models/paddle.obj", "./textures/texture.png");

  enemy_paddle_ = new Paddle("./models/paddle.obj", "./textures/texture.png");
  enemy_paddle_->y_rotation_ = 180.0f;

  ball_ = new Ball("./models/ball.obj", "./textures/test.png");
  ball_->position_ = glm::vec3(0.0f, 0.0f, PADDLE_Z - PADDLE_Z_HIT_MARGIN - BALL_RADIUS);

  walls_ = new Walls(PADDLE_Z, BACK_WALL, PADDLE_MAX_X, PADDLE_MAX_Y);

  music_box_ = new MusicBox(hWnd_);
  //load sounds
  //sound_pong_ = music_box_->SoundLoad("./sounds/pong.raw");
  sounds_["pong"] = music_box_->SoundLoad("./sounds/pong.raw");
  sounds_["theme"] = music_box_->SoundLoad("./sounds/mainTheme.raw");
  sounds_["lose"] = music_box_->SoundLoad("./sounds/lose.raw");
  music_box_->SoundPlay(sounds_["theme"], 1);
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
  if((GetAsyncKeyState(VK_TAB) & 0x8000) != 0)
  {
    ball_->velocity_ = glm::vec3(0.0f, 0.0f, -20.0f);
  }

  //update ball position, velocity etc.
  ball_->position_ += ball_->velocity_ * (float)elapsed_time;
  ball_->velocity_.x += ball_->acceleration_.x * (float)elapsed_time;
  ball_->velocity_.y += ball_->acceleration_.y * (float)elapsed_time;

  ball_->acceleration_ += (ball_->acceleration_ * (float)(BALL_ACC_WINDUP * elapsed_time));

  //enemy paddle AI
  float x_pos = ball_->position_.x;
  float y_pos = ball_->position_.y;

  x_pos = (x_pos + enemy_paddle_->body_.right > PADDLE_MAX_X) ? PADDLE_MAX_X - enemy_paddle_->body_.right : x_pos;
  x_pos = (x_pos - enemy_paddle_->body_.left < -PADDLE_MAX_X) ? -PADDLE_MAX_X + enemy_paddle_->body_.right : x_pos;
  y_pos = (y_pos + enemy_paddle_->body_.top > PADDLE_MAX_Y) ? PADDLE_MAX_Y - enemy_paddle_->body_.top : y_pos;
  y_pos = (y_pos - enemy_paddle_->body_.bottom < -PADDLE_MAX_Y) ? -PADDLE_MAX_Y + enemy_paddle_->body_.bottom : y_pos;
  enemy_paddle_->set_position(glm::vec3(x_pos, y_pos, BACK_WALL));

  //update ball matrices
  ball_->model_matrix_ = glm::translate(glm::mat4(1.0f), ball_->position_);

  //check if ball reached back wall
  if(ball_->position_.z - BALL_RADIUS < BACK_WALL)
  {
    //sound
    music_box_->SoundPlay(sounds_["pong"], 0);

    ball_->velocity_.z = (ball_->velocity_.z) * -Z_BOUNCE_VEL_MULTIPLIER;
    ball_->position_.z = BACK_WALL + BALL_RADIUS;

    //reset accel
    ball_->acceleration_ = glm::vec2(0.0f);
  }
  else if(ball_->position_.z + BALL_RADIUS > PADDLE_Z - PADDLE_Z_HIT_MARGIN) //if it reached paddle's depth <-- MARGIN INCLUDED
  {
    //check if it has hit the paddle (more-than-half-ball bounces are also valid)
    if(ball_->position_.x - BALL_RADIUS < paddle_->position_.x + paddle_->body_.right &&
      ball_->position_.x + BALL_RADIUS > paddle_->position_.x - paddle_->body_.left &&
      ball_->position_.y - BALL_RADIUS < paddle_->position_.y + paddle_->body_.top &&
      ball_->position_.y + BALL_RADIUS > paddle_->position_.y - paddle_->body_.bottom)
    {
      //HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT-HIT
      //sound
      music_box_->SoundPlay(sounds_["pong"],0);

      ball_->velocity_.z = -(ball_->velocity_.z);

      //calculate displacement between centers and modify ball velocity based on that displacement
     // glm::vec2 bounce_velocity = glm::vec2(ball_->position_.x - paddle_->position_.x,
     //   ball_->position_.y - paddle_->position_.y) * (float)BOUNCE_DISPLACEMENT;

      //ball_->velocity_.x += bounce_velocity.x;
      //ball_->velocity_.y += bounce_velocity.y;

      //calculate paddle velocity
      glm::vec2 paddle_velocity = (glm::vec2(paddle_->position_.x, paddle_->position_.y) - last_paddle_position_) / ((float)elapsed_time);

      //ball_->velocity_.x += (paddle_velocity.x * PADDLE_VEL_MULTIPLIER);
      //ball_->velocity_.y += (paddle_velocity.y * PADDLE_VEL_MULTIPLIER);

      ball_->acceleration_ = (paddle_velocity/(float)elapsed_time) * -(float)PADDLE_BALL_SPIN_ACC;


      ball_->position_.z = PADDLE_Z - BALL_RADIUS - PADDLE_Z_HIT_MARGIN;
    }
    else if(ball_->position_.z + BALL_RADIUS > PADDLE_Z)//last moment to hit the ball has passed :( :( :(
    {
      ball_->velocity_ = glm::vec3(0.0f);
      ball_->position_ = glm::vec3(0.0f, 0.0f, PADDLE_Z - 0.5f);
      music_box_->SoundPlay(sounds_["lose"], 0);

    }
  }

  //left side collision
  if(ball_->position_.x  - BALL_RADIUS < -PADDLE_MAX_X)
  {
    ball_->velocity_.x = -(ball_->velocity_.x) * BOUNCE_DAMPING_FACTOR;
    ball_->position_.x = -PADDLE_MAX_X + BALL_RADIUS;
  }
  //right side
  else if(ball_->position_.x + BALL_RADIUS > PADDLE_MAX_X)
  {
    ball_->velocity_.x = -(ball_->velocity_.x) * BOUNCE_DAMPING_FACTOR;
    ball_->position_.x = PADDLE_MAX_X - BALL_RADIUS;
  }

  //top collision
  if(ball_->position_.y + BALL_RADIUS > PADDLE_MAX_Y)
  {
    ball_->velocity_.y = -(ball_->velocity_.y) * BOUNCE_DAMPING_FACTOR;
    ball_->position_.y = PADDLE_MAX_Y - BALL_RADIUS;
  }
  //bottom collision
  else if(ball_->position_.y - BALL_RADIUS < -PADDLE_MAX_Y)
  {
    ball_->velocity_.y = -(ball_->velocity_.y) * BOUNCE_DAMPING_FACTOR;
    ball_->position_.y = -PADDLE_MAX_Y + BALL_RADIUS;
  }
  
  

  //save paddle position to use it in next frame
  last_paddle_position_ = glm::vec2(paddle_->position_.x, paddle_->position_.y);
}

void Game::RenderScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glClearColor(0.2f, 0.1f, 0.15f, 0.0f);

  //set up shader uniforms

  //shader_.setUniform("view", view_matrix_); //camera stays in place so why bother to update since there is only one shader? (for now hehe)
  //shader_.setUniform("projection", projection_matrix_); //changes only if window was resized, moved to ResizeScene (idk if this should be like that

  //LIGHT POSITION SHADER!!!! AND COLOR
  shader_.setUniform("light_pos", ball_->position_);

  //walls
  shader_.setUniform("walls", true);
  walls_->Draw(&shader_);
  shader_.setUniform("walls", false);

  //enemy paddle
  enemy_paddle_->Draw(&shader_);

  //ball, because it is the light source, turn off shading when rendering it
  shader_.setUniform("ball", true);
  ball_->Draw(&shader_);
  shader_.setUniform("ball", false);

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
