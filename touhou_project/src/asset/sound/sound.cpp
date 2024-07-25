/**
 * @file sound.cpp
 * @brief
 *
 */

#include "include/asset/sound/sound.h"
#include "include/core/engine/engine.h"

int Sound::Load(const wstring &file_path) {
  // assert(SoundManager::Get()->GetSoundDevice() != nullptr);

  wchar_t szExt[10] = {0};
  _wsplitpath_s(file_path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt,
                10);

  if (!wcscmp(szExt, L".wav")) {
    if (false == LoadWaveSound(file_path))
      assert(nullptr);
  } else
    assert(nullptr);

  return true;
}

bool Sound::LoadWaveSound(const wstring &file_path) {

  // Open File
  const HMMIO file_handle =
      mmioOpen(const_cast<wchar_t *>(file_path.c_str()), nullptr, MMIO_READ);

  // Fail Assert
  if (file_handle == nullptr) {
    MessageBox(nullptr, L"사운드 로딩 실패", L"에러", MB_OK);
    return false;
  }

  // Chunk, wave 파일 구조 분석
  MMCKINFO parent;
  memset(&parent, 0, sizeof(parent));
  parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
  mmioDescend(file_handle, &parent, nullptr, MMIO_FINDRIFF);

  MMCKINFO child;
  memset(&child, 0, sizeof(child));
  child.ckid = mmioFOURCC('f', 'm', 't', ' ');
  mmioDescend(file_handle, &child, &parent, MMIO_FINDCHUNK);

  WAVEFORMATEX wft;
  memset(&wft, 0, sizeof(wft));
  mmioRead(file_handle, reinterpret_cast<char *>(&wft), sizeof(wft));

  mmioAscend(file_handle, &child, 0);
  child.ckid = mmioFOURCC('d', 'a', 't', 'a');
  mmioDescend(file_handle, &child, &parent, MMIO_FINDCHUNK);

  memset(&buffer_info_, 0, sizeof(DSBUFFERDESC));
  buffer_info_.dwBufferBytes = child.cksize;
  buffer_info_.dwSize = sizeof(DSBUFFERDESC);
  buffer_info_.dwFlags =
      DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME;
  buffer_info_.lpwfxFormat = &wft;

  // TODO SoundManager 개발 시 처리할 것
  // if (FAILED(SoundManager::Get()->GetSoundDevice()->CreateSoundBuffer(
  //         &buffer_info_, &sound_buffer_, NULL))) {
  //   MessageBox(NULL, L"웨이브 파일 로딩 실패", L"에러", MB_OK);
  //   return false;
  // }

  void *write1 = nullptr;
  void *write2 = nullptr;
  DWORD dwlength1, dwlength2;

  sound_buffer_->Lock(0, child.cksize, &write1, &dwlength1, &write2, &dwlength2,
                      0L);

  if (write1 != nullptr)
    mmioRead(file_handle, static_cast<char *>(write1),
             static_cast<LONG>(dwlength1));
  if (write2 != nullptr)
    mmioRead(file_handle, static_cast<char *>(write2),
             static_cast<LONG>(dwlength2));

  sound_buffer_->Unlock(write1, dwlength1, write2, dwlength2);

  mmioClose(file_handle, 0);

  // Default
  SetSoundBufferVolume(50.f);

  return true;
}

/**
 * @brief 재생 함수
 * @param loop 반복 재생할지 선택하는 인자
 */
void Sound::Play(bool loop) const {
  sound_buffer_->SetCurrentPosition(0);

  if (loop)
    sound_buffer_->Play(0, 0, DSBPLAY_LOOPING);
  else
    sound_buffer_->Play(0, 0, 0);
}

/**
 * @brief BGM 함수
 * @param loop 반복 재생할지 선택하는 인자
 */
void Sound::PlayToBGM(const bool loop) {
  // TODO SoundManager 나오면 처리할 것
  // SoundManager::Get()->RegisterToBGM(this);

  if (loop)
    sound_buffer_->Play(0, 0, DSBPLAY_LOOPING);
  else
    sound_buffer_->Play(0, 0, 0);
}

/**
 * @brief 현재 사운드 버퍼를 정지하는 함수
 * @param reset 해당 변수값이 true라면 position 0로 이동
 */
void Sound::Stop(const bool reset) const {
  sound_buffer_->Stop();
  if (reset)
    sound_buffer_->SetCurrentPosition(0);
}

/**
 * @brief 볼륨 설정 함수
 * @param volume setter로 멤버 세팅 -> 멤버값을 버퍼에 세팅
 */
void Sound::SetSoundBufferVolume(float volume) {
  set_volume(volume);
  sound_buffer_->SetVolume(volume_);
}

/**
 * @brief 재생 지점 설정 함수
 * @param position 지점 정보를 받아와서 buffer에 값 세팅
 */
void Sound::SetPosition(float position) const {
  Stop(true);

  const DWORD dw_bytes = static_cast<DWORD>(
      position / 100.f * static_cast<float>(buffer_info_.dwBufferBytes));
  sound_buffer_->SetCurrentPosition(dw_bytes);
}

/**
 * @brief 볼륨값을 받아서 데시벨 치환된 볼륨을 세팅하는 함수
 * @param volume 여기서 입력한 값을 변환
 */
void Sound::set_volume(float volume) {
  if (volume > 100.f)
    volume = 100.f;
  else if (volume <= 0.f)
    volume = 0.00001f;

  // 1 ~ 100 의 값을 데시벨로 변환
  int decibel_volume = static_cast<LONG>(-2000.0 * log10(100.f / volume));

  if (decibel_volume < -10000)
    decibel_volume = -10000;
	volume_ = decibel_volume;
}

