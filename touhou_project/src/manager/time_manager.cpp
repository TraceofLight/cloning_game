/**
 * @file time_manager.cpp
 * @brief
 */

#include "include/manager/time_manager.h"
#include "include/core/engine/engine.h"

TimeManager::TimeManager()
    : frequency_{}, current_count_{}, prev_count_{}, delta_time_(0),
      float_delta_time_(0), time_(0), second_(0), fps_(0) {}

TimeManager::~TimeManager() = default;

void TimeManager::Init() {
  QueryPerformanceFrequency(&frequency_);
  QueryPerformanceCounter(&prev_count_);
}

void TimeManager::Tick() {
  QueryPerformanceCounter(&current_count_);

  delta_time_ =
      static_cast<double>(current_count_.QuadPart - prev_count_.QuadPart) /
      static_cast<double>(frequency_.QuadPart);
  float_delta_time_ = static_cast<float>(delta_time_);

  // 시간 누적
  time_ += delta_time_;
  second_ += delta_time_;

  // 함수 호출 횟수 (1회 호출마다 1프레임 생성)
  ++fps_;

  if (1 < second_) {
    second_ -= 1.;

    // 윈도우 타이틀에 FPS 랑 DeltaTime 표시
    wchar_t buf[256] = {};
    swprintf_s(buf, 256, L"FPS : %d, DeltaTime : %f", fps_, float_delta_time_);
    SetWindowText(Engine::Get()->main_handle(), buf);

    fps_ = 0;
  }

  prev_count_ = current_count_;
}
