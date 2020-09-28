#pragma once

#include <GL/glew.h>
#include <Windows.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <unordered_map>

#include "Paddle.h"
#include "Ball.h"
#include "Shader.h"
#include "MusicBox.h"
#include "Walls.h"

//play volume dimension constants
//play area dimensions ->
//far xy plane 55
//
#define CAM_Z_NEAR    0.1f
#define CAM_Z_FAR     1000.0f
#define CAM_FOV       45.0f

//movement - play board (?) dimensions - macro names self explanatory
#define PADDLE_Z      -5.0f
#define PADDLE_MAX_X  4.0f
#define PADDLE_MAX_Y  2.25f

#define BALL_RADIUS 0.2f

#define BACK_WALL -45.0f

//Margin in which ball-paddle collisions are checked, "spatial window" when player can bounce the ball.
//If paddle is not present during ball's journey through this region of Z coords and ball reaches PADDLE_Z,
//bounce fails.
#define PADDLE_Z_HIT_MARGIN 0.2f

//Every time ball hits a wall, velocity component in direction perpendicular to plane hit gets multiplied
//by this factor (XY plane excluded - z velocity is not affected by this factor).
#define BOUNCE_DAMPING_FACTOR 0.65f

//Displacement of ball and paddle centers on ball-paddle hits is multiplied by this factor and applied
//to ball as its velocity in XY plane.
#define BOUNCE_DISPLACEMENT 0.0f //USELESS XDD


#define PADDLE_VEL_MULTIPLIER 0.0f //USELESS XDD

//Ball's z velocity gets multiplier by this value every time when ball gets bounced by enemy's paddle.
#define Z_BOUNCE_VEL_MULTIPLIER 1.05f

//After calculating paddle's velocity when bouncing the ball, 
#define PADDLE_BALL_SPIN_ACC  0.0008f


#define BALL_ACC_WINDUP 0.002f



typedef std::unordered_map<const char*, int> SoundsLib;

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
  SoundsLib sounds_;

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

