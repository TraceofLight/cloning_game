/**
 * @file level_manager.h
 * @brief 화면에 출력할 레벨을 관리하는 클래스
 */

#ifndef TOUHOU_LEVEL_MANAGER_H_
#define TOUHOU_LEVEL_MANAGER_H_

#include "common/common_utility.h"
#include "include/level/level.h"

class LevelManager {
  SINGLE(LevelManager)
private:
  Level *level_arr_[static_cast<int>(LEVEL_TYPE::END)];
  Level *current_level_;

public:
  void Init();
  void Tick();
  void Render();

  // Getter
  Level *current_level() const { return current_level_; }

private:
  void SetLevel(LEVEL_TYPE type);
};

#endif // TOUHOU_LEVEL_MANAGER_H_
