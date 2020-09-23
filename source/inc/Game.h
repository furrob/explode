#pragma once

#include <Windows.h>

class Game
{
private:


public:

  Game();

  //set everything that will be necessary 
  void Initialize();

  //update everything time-related and render scene
  void Update();

  //uptade viewport / handle resizing
  void ResizeScene();
};

