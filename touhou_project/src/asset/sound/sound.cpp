/**
 * @file sound.cpp
 * @brief
 *
 */

#include "include/asset/sound/sound.h"
#include "include/core/engine/engine.h"

int Sound::Load(const filesystem::path& file_path) {
  // assert(SoundManager::Get()->GetSoundDevice() != nullptr);

  filesystem::path path(file_path);
  string extension = path.extension().string();

  // Convert extension to lowercase for case-insensitive comparison
  transform(extension.begin(), extension.end(), extension.begin(),
            [](const unsigned char c) { return tolower(c); });

  if (extension == ".wav") {
    if (!LoadWaveSound(file_path)) {
      throw std::runtime_error("Failed to load WAV file: " + file_path.string());
    }
  } else {
    throw std::runtime_error("Unsupported file format: " + extension);
  }

  return true;
}

bool Sound::LoadWaveSound(const filesystem::path& file_path) {
  // File Load
  HMMIO file_handle = mmioOpenW(const_cast<wchar_t*>(file_path.c_str()), nullptr, MMIO_READ);
  if (file_handle == nullptr) {
    throw std::runtime_error("Failed to open sound file: " + file_path.string());
  }

  try {
    // Analyze File Structure
    MMCKINFO parent = {};
    parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
    if (mmioDescend(file_handle, &parent, nullptr, MMIO_FINDRIFF) != MMSYSERR_NOERROR) {
      throw std::runtime_error("Failed to find WAVE chunk");
    }

    MMCKINFO child = {};
    child.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(file_handle, &child, &parent, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
      throw std::runtime_error("Failed to find fmt chunk");
    }

    WAVEFORMATEX wft = {};
    if (mmioRead(file_handle, reinterpret_cast<char*>(&wft), sizeof(wft)) != sizeof(wft)) {
      throw std::runtime_error("Failed to read WAVEFORMATEX");
    }

    if (mmioAscend(file_handle, &child, 0) != MMSYSERR_NOERROR) {
      throw std::runtime_error("Failed to ascend from fmt chunk");
    }

    child.ckid = mmioFOURCC('d', 'a', 't', 'a');
    if (mmioDescend(file_handle, &child, &parent, MMIO_FINDCHUNK) != MMSYSERR_NOERROR) {
      throw std::runtime_error("Failed to find data chunk");
    }

    buffer_info_ = {};
    buffer_info_.dwBufferBytes = child.cksize;
    buffer_info_.dwSize = sizeof(DSBUFFERDESC);
    buffer_info_.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME;
    buffer_info_.lpwfxFormat = &wft;

    // TODO: SoundManager 개발 시 처리할 것
    // if (FAILED(SoundManager::Get()->GetSoundDevice()->CreateSoundBuffer(
    //         &buffer_info_, &sound_buffer_, nullptr))) {
    //     throw std::runtime_error("Failed to create sound buffer");
    // }

    void* write1 = nullptr;
    void* write2 = nullptr;
    DWORD length1 = 0;
    DWORD length2 = 0;

    if (FAILED(sound_buffer_->Lock(0, child.cksize, &write1, &length1, &write2, &length2, 0L))) {
      throw std::runtime_error("Failed to lock sound buffer");
    }

    if (write1 && mmioRead(file_handle, static_cast<char*>(write1), static_cast<LONG>(length1)) !=
        static_cast<LONG>(length1)) {
      sound_buffer_->Unlock(write1, length1, write2, length2);
      throw std::runtime_error("Failed to read first part of sound data");
    }

    if (write2 && mmioRead(file_handle, static_cast<char*>(write2), static_cast<LONG>(length2)) !=
        static_cast<LONG>(length2)) {
      sound_buffer_->Unlock(write1, length1, write2, length2);
      throw std::runtime_error("Failed to read second part of sound data");
    }

    if (FAILED(sound_buffer_->Unlock(write1, length1, write2, length2))) {
      throw std::runtime_error("Failed to unlock sound buffer");
    }

    mmioClose(file_handle, 0);

    // Set Default
    SetSoundBufferVolume(50.f);

    return true;
  } catch (const std::exception& e) {
    cerr << "Exception caught in LoadWaveSound: " << e.what() << "\n";
    if (file_handle) {
      mmioClose(file_handle, 0);
    }
    throw;
  }
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

  const DWORD dw_bytes =
      static_cast<DWORD>(position / 100.f * static_cast<float>(buffer_info_.dwBufferBytes));
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