/**
 * @file ui_manager.cpp
 * @brief
 */

#include "include/manager/ui_manager.h"

#include "include/manager/key_manager.h"
#include "include/manager/level_manager.h"

/**
 * @brief UI 전체에 대해서 Tick을 진행하는 함수
 */
void UIManager::Tick() {
  // prevent frame check
  if (prevent_frame_) {
    prevent_frame_--;
    return;
  }

  // current status check
  KEY_STATE left_button_state = KeyManager::Get()->key_state(KEY::LBTN);
  Level *current_level = LevelManager::Get()->current_level();

  vector<UI *> &ui_vector =
      reinterpret_cast<vector<UI *> &>(current_level->layer(LAYER_TYPE::UI));

  // case1 - 타깃 없음: 부모 UI로부터 재탐색
  // case2 - TAP: 단순 TAP만으로 판단 불가능한 동작이 있으므로 다음 Tick Action Check
  // case3 - RELEASED: 동작 확정 지은 뒤 타깃 UI 재탐색
  for (auto parent_ui = ui_vector.rbegin(); parent_ui != ui_vector.rend();
       ++parent_ui) {
    UI *priority_ui = GetPriorityUI(*parent_ui);

    if (priority_ui == nullptr) {
      StatusCheck(*parent_ui);
      continue;
    }

    if (left_button_state == KEY_STATE::TAP) {
      priority_ui->set_left_button_down(true);
      priority_ui->LeftButtonDown();

      ui_vector.erase((parent_ui + 1).base());
      ui_vector.push_back(priority_ui);

      break;
    }

    if (left_button_state == KEY_STATE::RELEASED) {
      priority_ui->LeftButtonUp();
      if (priority_ui->is_left_button_down()) {
        priority_ui->LeftButtonClicked();
      }
    }

    StatusCheck(priority_ui);
  }
}

/**
 * @brief 현재 마우스의 타깃이 되는 UI를 체크하고 이벤트 상태를 변경하는 함수
 *
 * @param parent_ui 체크 대상들이 포함된 UI 중 가장 큰 범위의 UI
 * @return 현재 마우스가 올라와 있는 UI의 포인터를 반환
 */
UI *UIManager::GetPriorityUI(UI *parent_ui) {
  UI *result_ui = nullptr;

  // start from this ui
  process_queue_.push_back(parent_ui);

  while (!process_queue_.empty()) {
    UI *check_ui = process_queue_.front();
    process_queue_.pop_front();

    if (check_ui->is_mouse_on())
      result_ui = check_ui;

    // mouse event 반영
    if (!check_ui->is_mouse_on_previous() && check_ui->is_mouse_on())
      check_ui->BeginHover();
    else if (check_ui->is_mouse_on_previous() && check_ui->is_mouse_on())
      check_ui->OnHover();
    else if (check_ui->is_mouse_on_previous() && !check_ui->is_mouse_on())
      check_ui->EndHover();

    for (auto each_child : check_ui->child_vector())
      process_queue_.push_back(each_child);
  }

  return result_ui;
}

/**
 * @brief 마우스의 상태를 확인하고 해당 UI 이하의 status를 일괄 변경하는 함수
 *
 * @param parent_ui 변경 진행할 ui 중 가장 parent인 ui, 이하는 BFS 탐색
 */
void UIManager::StatusCheck(UI *parent_ui) {
  KEY_STATE left_button_state = KeyManager::Get()->key_state(KEY::LBTN);

  // BFS
  process_queue_.push_back(parent_ui);

  while (!process_queue_.empty()) {
    UI *check_ui = process_queue_.front();
    process_queue_.pop_front();

    for (auto each_child : check_ui->child_vector())
      process_queue_.push_back(each_child);

    if (left_button_state == KEY_STATE::RELEASED)
      check_ui->set_left_button_down(false);
  }
}
