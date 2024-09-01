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
  bool is_left_button_down_;
  bool is_left_button_clicked_;

 public:
  void Tick();

  void set_prevent_frame(const int frame_count) { prevent_frame_ = frame_count; }

 private:
  UI* GetPriorityUI(UI* parent_ui);
  void ResetUI(UI* parent_ui);
  void SetMouseState();
};

#endif // TOUHOU_UI_MANAGER_H_