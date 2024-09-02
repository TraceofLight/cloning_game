/**
 * @file sound.h
 * @brief 실제 이미지 파일로부터 불러오는 로딩된 이미지 구조체 정의 헤더
 */

#ifndef TOUHOU_SOUND_H_
#define TOUHOU_SOUND_H_

#include "include/asset/asset.h"

class Sound : public Asset {
 private:
  LPDIRECTSOUNDBUFFER sound_buffer_;
  DSBUFFERDESC buffer_info_;
  int volume_;

 public:
  CLONE_DISABLE(Sound)
  Sound() = default;
  ~Sound() override = default;

  // 재생 관련 옵션
  void Play(bool loop = false) const;
  void PlayToBGM(bool loop = false);
  void Stop(bool reset = false) const;

  int Load(const string& file_path) override;

  // Setter
  void set_volume(float volume);

  void SetSoundBufferVolume(float volume);  // 0 ~ 100
  void SetPosition(float position) const;   // 0 ~ 100

 private:
  bool LoadWaveSound(const string &file_path);
};

#endif // TOUHOU_SOUND_H_
