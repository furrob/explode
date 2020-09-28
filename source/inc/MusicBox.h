#pragma once

#include <Windows.h>
#include <dsound.h>

#include <vector>
#include <istream>
#include <fstream>
#include <iostream>

class MusicBox
{
private:
  //interface to sound device
  LPDIRECTSOUND8 lpds;

  HRESULT CreateSoundBuffer(LPDIRECTSOUNDBUFFER8* ppDsb8, DWORD buffer_size);

  std::vector<LPDIRECTSOUNDBUFFER8> sounds_;

public:
  MusicBox(HWND hWnd); //handle to game window needed
  ~MusicBox();

  /// <summary>
  /// Loads *.raw file - PCM encoded, 16bps, 44100 samples per second, 2 channels
  /// </summary>
  /// <param name="file_path"></param>
  /// <returns></returns>
  INT SoundLoad(const char* file_path);

  void SoundPlay(INT sound_index, int flag);
};

