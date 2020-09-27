#pragma once
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>


#pragma comment(lib, "winmm.lib")

class SoundManager
{
private:
  MMRESULT mmResult;
  HWAVEOUT hWO = 0;
  std::vector<WAVEHDR> whdrs;
  std::fstream soundFile;
  WAVEFORMATEX pcmWaveFormat;
  std::vector<byte*> tracks;

public:
  SoundManager();
  ~SoundManager();
  void playSound(int bgmNumber);
  int addSound(std::string soundPath);

};

