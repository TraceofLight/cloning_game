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
  list<UI *> process_queue_;

public:
  void Tick();
  void set_prevent_frame(const int frame_count) {
    prevent_frame_ = frame_count;
  }

private:
  UI *GetPriorityUI(UI *parent_ui);
  void StatusCheck(UI *parent_ui);
};

#endif // TOUHOU_UI_MANAGER_H_