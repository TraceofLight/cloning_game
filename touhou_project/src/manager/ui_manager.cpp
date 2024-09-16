/**
 * @file ui_manager.cpp
 * @brief
 */

#include "include/manager/ui_manager.h"
#include "include/manager/key_manager.h"
#include "include/manager/level_manager.h"
#include "include/manager/debug_manager.h"

UIManager::UIManager() = default;

UIManager::~UIManager() = default;

/**
 * @brief UI 전체에 대해서 Tick을 진행하는 함수
 * 1. mouse 상태 가져와서 세팅
 * 2. UI의 Priority가 높은 경우에서 해당 UI의 Action 처리
 * 3. 그렇지 않은 경우의 UI인 경우 기존에 적용된 상태를 전부 해제함
 */
void UIManager::Tick() {
  // prevent frame check
  if (prevent_frame_) {
    prevent_frame_--;
    return;
  }

  // 직전 click이었는지 판단
  const bool was_clicked = is_left_button_clicked_;

  // current status check
  SetMouseState();
  vector<UI*>& ui_vector = reinterpret_cast<vector<UI*>&>(LevelManager::Get()
                                                         ->current_level()->layer(LAYER_TYPE::UI));

  // case1 - 타깃 없음: 부모 UI로부터 재탐색
  // case2 - TAP: 단순 TAP만으로 판단 불가능한 동작이 있으므로 다음 Tick Action Check
  // case3 - RELEASED: 동작 확정 지은 뒤 타깃 UI 재탐색
  for (auto parent_ui = ui_vector.begin(); parent_ui != ui_vector.end(); ++parent_ui) {
    // parent ui 중에서 현재 컨트롤 대상인 priority ui를 선택
    UI* priority_ui = GetPriorityUI(*parent_ui);

    // priority ui가 없다면 해당 parent ui 미사용이므로 관련 정보 disable 뒤 바로 탐색
    if (priority_ui == nullptr) {
      ResetUI(*parent_ui);
      continue;
    }

    // TODO(KHJ): Action들 여기서 처리하는 게 맞는지?
    if (is_left_button_down_) {
      priority_ui->set_left_button_down(true);
      priority_ui->LeftButtonDownAction();

      // ui_vector.erase((parent_ui + 1).base());
      // ui_vector.push_back(priority_ui);
    }

    if (is_left_button_clicked_) {
      priority_ui->set_left_button_down(false);
      priority_ui->set_left_button_clicked(true);
      priority_ui->LeftButtonClickedAction();

      // 마우스 클릭 Log 출력
      LOG(LOG_LEVEL::STATUS, "Mouse Clicked")
    }

    if (was_clicked) {
      priority_ui->set_left_button_clicked(false);
      priority_ui->LeftButtonUpAction();
    }

    // ResetUI(priority_ui);
    break;
  }
}

/**
 * @brief 현재 마우스의 타깃이 되는 UI를 체크하고 이벤트 상태를 변경하는 함수
 *
 * @param parent_ui 체크 대상들이 포함된 UI 중 가장 큰 범위의 UI
 * @return 현재 마우스가 올라와 있는 UI의 포인터를 반환
 */
UI* UIManager::GetPriorityUI(UI* parent_ui) {
  UI* result_ui = nullptr;

  // start from this ui
  process_list_.push_back(parent_ui);

  while (!process_list_.empty()) {
    UI* check_ui = process_list_.front();
    process_list_.pop_front();

    if (check_ui->is_mouse_on())
      result_ui = check_ui;

    // mouse event 반영
    if (!check_ui->is_mouse_on_previous() && check_ui->is_mouse_on())
      check_ui->BeginHoverAction();
    else if (check_ui->is_mouse_on_previous() && check_ui->is_mouse_on())
      check_ui->OnHoverAction();
    else if (check_ui->is_mouse_on_previous() && !check_ui->is_mouse_on())
      check_ui->EndHoverAction();

    for (auto each_child : check_ui->child_vector())
      process_list_.push_back(each_child);
  }

  return result_ui;
}

/**
 * @brief parent ui 이하의 state를 초기화하는 함수
 * 완전 탐색을 활용하여 관련 ui를 vector에 올린다.
 * @param parent_ui 변경 진행할 ui 중 가장 상단에 위치한 ui
 */
void UIManager::ResetUI(UI* parent_ui) {
  // add parent ui
  process_list_.push_back(parent_ui);

  // add child ui using breadth first search
  while (!process_list_.empty()) {
    UI* target_ui = process_list_.front();
    process_list_.pop_front();

    for (auto each_child : target_ui->child_vector())
      process_list_.push_back(each_child);

    // 버튼 관련 눌린 정보가 존재하는 경우 초기화
    if (target_ui->is_left_button_down())
      target_ui->set_left_button_down(false);
    if (target_ui->is_left_button_clicked())
      target_ui->set_left_button_clicked(false);
  }
}

/**
 * @brief 마우스 정보를 KeyManager로부터 가져와서 Tick마다 세팅하는 함수
 * ----------------------------------------------------------------------------------------
 * NONE    : down, click 상태 둘 다 아니므로 click 상태였다면 풀어줘야 함
 * TAP     : down은 맞지만 click은 아님
 * PRESSED : TAP 상태와 동일하게 down이지만 click은 아니므로 상태 변화 없음
 * RELEASED: down 상태는 아니게 되며 click 상태로 전환
 * ----------------------------------------------------------------------------------------
 * Mouse Position은 Parent UI로부터의 위치를 반영해야 하므로 여기서 state setting하지 않음
 * TODO(KHJ): NONE에서 click만 풀고 있는데 TAP에서 바로 RELEASE 없이 NONE으로 올 수 있는지 체크할 것
 */
void UIManager::SetMouseState() {
  KEY_STATE left_button_state = KeyManager::Get()->key_state(KEY::LBTN);

  // 아무 상태도 아님
  if (left_button_state == KEY_STATE::NONE) {
    is_left_button_clicked_ = false;

    // 눌러지기 시작한 시점
  } else if (left_button_state == KEY_STATE::TAP) {
    is_left_button_down_ = true;

    // 클릭 시점
  } else if (left_button_state == KEY_STATE::RELEASED) {
    is_left_button_clicked_ = true;
    is_left_button_down_ = false;
  }
}