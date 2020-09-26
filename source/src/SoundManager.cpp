#include "SoundManager.h"

SoundManager::SoundManager()
{
  //przygotowanie danych do urz¹dzenia

  pcmWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
  pcmWaveFormat.nChannels = 1;
  pcmWaveFormat.nSamplesPerSec = 44100L;
  pcmWaveFormat.wBitsPerSample = 8;
  pcmWaveFormat.nAvgBytesPerSec = 44100L;
  pcmWaveFormat.nBlockAlign = 1;
  pcmWaveFormat.cbSize = 0;

  //WYBÓR URZ¥DZENIA ODTWARZAJ¥CEGO

  for (UINT devId = 0; devId < waveOutGetNumDevs(); ++devId)
  {
    mmResult = waveOutOpen(&hWO, devId, &pcmWaveFormat, 0, 0, CALLBACK_NULL);
    if (mmResult == MMSYSERR_NOERROR)
      break;
  }


}

SoundManager::~SoundManager()
{
  for(int i=0;i<tracks.size();++i)
  {
    delete tracks[i];
  }
}

void SoundManager::playSound(int soundNumber)
{
  waveOutSetVolume(hWO, 0xFFFFFFFF);
  mmResult = waveOutPrepareHeader(hWO, &whdrs[soundNumber], sizeof(whdrs[soundNumber]));

  mmResult = waveOutWrite(hWO, &whdrs[soundNumber], sizeof(whdrs[soundNumber]));

  //////////////////////////////////////////////////////////////////////////////

  //while ((whdrs[soundNumber].dwFlags & WHDR_DONE) != WHDR_DONE)
  //  Sleep(100);

  mmResult = waveOutUnprepareHeader(hWO, &whdrs[soundNumber], sizeof(whdrs[soundNumber]));
  //mmResult = waveOutClose(hWO);
 // delete[] whdr.lpData;

}

int SoundManager::addSound(std::string soundPath)
{
   std::ifstream input( soundPath, std::ios::binary);
  //KOPIA DO PLIKU I SAYOOOONARRAAA
  input.open(soundPath, std::ios::binary | std::ios::in);

  std::vector<byte> buffer(std::istreambuf_iterator<char>(input), {});

  byte* bufferWhdr = new byte[buffer.size()];
  std::copy(buffer.begin(), buffer.end(), bufferWhdr);

  tracks.push_back(bufferWhdr);

  WAVEHDR temp;

 // memset(&temp, 0, sizeof(temp));
  temp.lpData = (LPSTR)bufferWhdr;
  temp.dwBufferLength = buffer.size();
  temp.dwUser = 0;
  temp.dwFlags = 0;
  temp.dwBytesRecorded = 0;
  temp.lpNext = 0;
  temp.reserved = 0;

  whdrs.push_back(temp);

  // Wektor próbek

  //for (int i = 0; i < whdr.dwBufferLength; ++i)
  //{
  //  whdr.lpData[i] = buffer.at(i); //WYPE£NIENIE BUFORA DWIÊKAMI
  //}

  ///////////////////////////////////////////////////////////////////////////////
  return whdrs.size()-1;
}

