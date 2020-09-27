#pragma once

#include <GL/glew.h>
#include <Windows.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Paddle.h"
#include "Ball.h"
#include "Shader.h"
//#include "SoundManager.h"
#include "MusicBox.h"
#include "Walls.h"

//play volume dimension constants
//play area dimensions ->
//far xy plane 55
//
#define CAM_Z_NEAR    0.1f
#define CAM_Z_FAR     1000.0f
#define CAM_FOV       45.0f

//movement
#define PADDLE_Z      -5.0f
#define PADDLE_MAX_X  4.0f
#define PADDLE_MAX_Y  2.25f

#define BALL_RADIUS 0.2f

#define BACK_WALL -45.0f

//bounce
#define BOUNCE_DISPLACEMENT 0.5f
#define PADDLE_VEL_MULTIPLIER 0.5f
#define PADDLE_BALL_SPIN_ACC  3.00f

class Game
{
private:
  glm::mat4 view_matrix_;
  glm::mat4 projection_matrix_;

  HWND hWnd_;

  int screen_width_;
  int screen_height_;

  HDC hDC_; //handle to device context, needed for rendering
  
  //directsound
  MusicBox* music_box_;
  int test_sound_ = -1;


  Paddle* paddle_ = nullptr;

  Paddle* enemy_paddle_ = nullptr;

  Ball* ball_ = nullptr;

  Walls* walls_ = nullptr;

  Shader shader_;

  //paddle position from last frame to calculate paddle
  glm::vec2 last_paddle_position_ = glm::vec2(0.0f);

public:
  Game(HWND hWnd, int width, int height, HDC hDC);

  ~Game();
  
  //set everything that will be necessary 
  void Initialize();

  //mouse move handler
  void OnMouseMove(int x, int y);

  //update everything time-related
  void Update(double elapsed_time);

  void RenderScene();

  //uptade viewport / handle resizing
  void ResizeScene(int width, int height);
};

