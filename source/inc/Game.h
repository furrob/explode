#pragma once

#include <GL/glew.h>
#include <Windows.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Paddle.h"
#include "Ball.h"
#include "Shader.h"
#include "SoundManager.h"
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
#define BOUNCE_DISPLACEMENT 2.0f

class Game
{
private:
  glm::mat4 view_matrix_;
  glm::mat4 projection_matrix_;
  SoundManager SM;
  int sound;


  int screen_width_;
  int screen_height_;

  HDC hDC_; //handle to device context, needed for rendering

  Paddle* paddle_ = nullptr;

  Paddle* enemy_paddle_ = nullptr;

  Ball* ball_ = nullptr;

  Walls* walls_ = nullptr;

  Shader shader_;

public:
  Game(int width, int height, HDC hDC);

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

