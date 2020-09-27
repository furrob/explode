#include "MusicBox.h"

HRESULT MusicBox::CreateSoundBuffer(LPDIRECTSOUNDBUFFER8* ppDsb8, DWORD buffer_size)
{
  WAVEFORMATEX wfx;
  DSBUFFERDESC dsbdesc;
  LPDIRECTSOUNDBUFFER pDsb = NULL; //pointer to created buffer
  HRESULT hr;

  //set up structure
  memset(&wfx, 0, sizeof(WAVEFORMATEX));
  wfx.wFormatTag = WAVE_FORMAT_PCM;
  wfx.nChannels = 2;
  wfx.nSamplesPerSec = 44100L;
  wfx.nBlockAlign = 4;  // 4 16
  wfx.nAvgBytesPerSec = 176400L;//for stereo nchannels*nsamplespersec
  wfx.wBitsPerSample = 16;
  
  //bufferdesc structure
  memset(&dsbdesc, 0, sizeof(DSBUFFERDESC));
  dsbdesc.dwSize = sizeof(DSBUFFERDESC);
  dsbdesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
  dsbdesc.dwBufferBytes = buffer_size; //size of buffer in bytes
  dsbdesc.lpwfxFormat = &wfx;
  
  //create buffer
  hr = lpds->CreateSoundBuffer(&dsbdesc, &pDsb, NULL);
  if(SUCCEEDED(hr))
  {
    hr = pDsb->QueryInterface(IID_IDirectSoundBuffer8, (LPVOID*)ppDsb8);
    pDsb->Release();
  }

  return hr;
}

MusicBox::MusicBox(HWND hWnd)
{
  HRESULT hr = DirectSoundCreate8(NULL, &lpds, NULL);
  if(FAILED(hr))
  {
    MessageBoxW(NULL, L"ERROR::MUSICBOX::DSCREATE", L"Error", MB_OK | MB_ICONERROR);
  }
  else
  {
    hr = lpds->Initialize(NULL);
    
    hr = lpds->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
  }
}

MusicBox::~MusicBox()
{
  CoUninitialize();
}

INT MusicBox::SoundLoad(const char* file_path)
{
  std::ifstream input(file_path, std::ios::binary);

  input.open(file_path, std::ios::binary | std::ios::in);

  std::vector<byte> samples(std::istreambuf_iterator<char>(input), {});

  LPDIRECTSOUNDBUFFER8 new_track;

  HRESULT hr;

  hr = CreateSoundBuffer(&new_track, samples.size());

  if(FAILED(hr))
  {
    //if something went wrong during creation
    return -1;
  }

  //memory location to write
  LPVOID lpvWrite;
  DWORD dwLength;

  if(DS_OK == new_track->Lock(0, 0, &lpvWrite, &dwLength,
    NULL, NULL, DSBLOCK_ENTIREBUFFER))
  {
    //ok
    memcpy(lpvWrite, (LPVOID)&samples[0], dwLength);
    HRESULT hres = new_track->Unlock(lpvWrite, dwLength, NULL, NULL);
    //push pointer to new buffer to vector
    sounds_.push_back(new_track);

    //return sound index
    return sounds_.size() - 1;
  }
  else
  {
    //error, invalid index returned
    return -1;
  }
}

void MusicBox::SoundPlay(INT sound_index)
{
  if(sound_index < 0 || sound_index > sounds_.size() - 1)
  {
    //incorrect index
    MessageBoxW(NULL, L"ERROR::MUSICBOX::PLAYSOUND::INVALID_INDEX", L"Error", MB_OK | MB_ICONERROR);
  }
  else
  {
    sounds_[sound_index]->SetCurrentPosition(0);
    HRESULT hr = sounds_[sound_index]->Play(0, 0, 0); //last flag can be DSBPLAY_LOOPING :O
    if(FAILED(hr))
    {
      MessageBoxW(NULL, L"ERROR::MUSICBOX::PLAYSOUND::PLAY_ERROR", L"Error", MB_OK | MB_ICONERROR);
    }
  }
}
