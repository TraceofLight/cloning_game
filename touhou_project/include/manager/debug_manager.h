/**
 * @file debug_manager.h
 * @brief 디버깅 내역을 받아서 출력하는 매니저 클래스
 */

#ifndef TOUHOU_DEBUG_MANAGER_H_
#define TOUHOU_DEBUG_MANAGER_H_

#include "common/common_utility.h"

class DebugManager {
  SINGLE(DebugManager)
private:
  vector<DebugShapeInfo> debug_display_list_;
  bool debug_mode_on_;

public:
  void Tick();
  void AddDebugInfo(const DebugShapeInfo& debug_info);
};

#endif // TOUHOU_DEBUG_MANAGER_H_
