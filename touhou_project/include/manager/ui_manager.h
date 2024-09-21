/**
 * @file ui_manager.h
 * @brief UI에 대한 정보를 관리하는 매니저 클래스의 헤더
 */

#ifndef TOUHOU_UI_MANAGER_H_
#define TOUHOU_UI_MANAGER_H_

#include "common/common_utility.h"
#include "include/object/ui/ui.h"

class UIManager {
  SINGLE(UIManager)
 private:
  int prevent_frame_;
  list<UI*> process_list_;

  // 클릭 상태는 ui manager에서 관리할 수 있지만 호버링은 ui 위치가 확정된 상태에서 확인이 가능
  // 따라서 ui manager에서는 앞 4비트만 세팅하고 뒷 4비트는 정보를 받은 각 ui에서 처리한다
  uint8_t mouse_status_flag_;

 public:
  static constexpr uint8_t kLeftButtonNone = 1 << 0;
  static constexpr uint8_t kLeftButtonTap = 1 << 1;
  static constexpr uint8_t kLeftButtonPressed = 1 << 2;
  static constexpr uint8_t kLeftButtonReleased = 1 << 3;
  static constexpr uint8_t kMouseHover = 1 << 4;

  void Tick();

  void set_prevent_frame(const int frame_count) { prevent_frame_ = frame_count; }

 private:
  UI* GetPriorityUI(UI* parent_ui);
  void ResetUI(UI* parent_ui);
  void SetClickStatus();
};

#endif // TOUHOU_UI_MANAGER_H_