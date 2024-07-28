/**
 * @file time_manager.h
 * @brief 오브젝트들의 상태를 시간 단위로 관리할 수 있도록 하는 모듈
 */

#ifndef TOUHOU_TIME_MANAGER_H_
#define TOUHOU_TIME_MANAGER_H_

#include "common/common_utility.h"

class TimeManager {
  SINGLE(TimeManager)

private:
  LARGE_INTEGER frequency_;
  LARGE_INTEGER current_count_;
  LARGE_INTEGER prev_count_;
  double delta_time_;
  float float_delta_time_;
  double time_;
  double second_;
  UINT fps_;

public:
  void Init();
  void Tick();

  double delta_time() const { return delta_time_; }
  float float_delta_time() const { return float_delta_time_; }
};

#endif //TOUHOU_TIME_MANAGER_H_
