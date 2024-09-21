/**
 * @file ui.cpp
 * @brief
 **/

#include "include/object/ui/ui.h"
#include "common/drawing_handle/drawing_handle.h"
#include "include/core/engine/engine.h"
#include "include/manager/key_manager.h"

UI::UI() = default;

UI::~UI() {
  ReleaseVector(child_vector_);
}

UI::UI(const UI& other)
  : Object(other) {
  for (const auto iter : other.child_vector())
    AddChild(iter->Clone());
}

/**
 * @brief 마우스의 위치를 반영하고 해당 오브젝트 및 하위 오브젝트들에 Tick을 돌리는 함수
 * UI Tick은 Level Tick 진행할 때 UI 레벨에 대해 진행한다
 * 부모가 존재할 경우 부모의 position을 고려해야 하므로 Manager Tick에서 진행할 수 없다
 */
void UI::Tick() {
  final_position_ = position();
  if (parent_)
    final_position_ += parent_->final_position();

  UpdateMouseHoveringInfo();
  TickSelf();

  for (auto child_ui : child_vector_)
    child_ui->Tick();
}

void UI::FinalTick() {
  Object::FinalTick();
  for (const auto child_ui : child_vector_)
    child_ui->FinalTick();
}

/**
 * @brief UI 렌더링하는 함수
 * 본인을 렌더링한 뒤 child ui도 순차적으로 렌더링 진행
 */
void UI::Render() {
  RenderSelf();
  for (const auto child_ui : child_vector_)
    child_ui->Render();
}

/**
 * @brief 본인을 렌더링하는 함수
 */
void UI::RenderSelf() {
  PEN_TYPE pen_type = PEN_TYPE::GREEN;

  if (is_mouse_on())
    pen_type = PEN_TYPE::BLUE;
  if (is_left_button_pressed())
    pen_type = PEN_TYPE::RED;

  DrawingHandle pen_handle(Engine::Get()->GetBackDC(), pen_type);
  DrawingHandle brush_handle(Engine::Get()->GetBackDC(), BRUSH_TYPE::HOLLOW);
  const Vector2 scale = Object::scale();

  Rectangle(Engine::Get()->GetBackDC(), static_cast<int>(final_position_.x()),
            static_cast<int>(final_position_.y()),
            static_cast<int>(scale.x()) + static_cast<int>(scale.x()),
            static_cast<int>(scale.y()) + static_cast<int>(scale.y()));
}

/**
 * @brief 마우스 상태를 ui manager로부터 받아온 뒤, mouse hovering 정보까지 세팅하는 함수
 * @param status_flag ui manager가 들고 있던 flag 정보
 */
void UI::UpdateMouseStatus(uint8_t status_flag) {
  // click 정보 갱신
  mouse_status_flag_ = (mouse_status_flag_ & 0xF0) | status_flag;
  // hovering 정보 갱신
  UpdateMouseHoveringInfo();
}

/**
 * @brief mouse cursor의 위치를 확인하고 hovering 체크를 진행하는 함수
 */
void UI::UpdateMouseHoveringInfo() {
  // previous 정보 업데이트
  if (is_mouse_on())
    set_mouse_on_previous();
  else
    set_mouse_off_previous();

  Vector2 mouse_position = KeyManager::Get()->mouse_position();
  Vector2 scale = Object::scale();

  // hovering 정보 세팅
  if (final_position_.x() <= mouse_position.x()
      && mouse_position.x() <= final_position_.x() + scale.x()
      && final_position_.y() <= mouse_position.y()
      && mouse_position.y() <= final_position_.y() + scale.y())
    set_mouse_on();
  else {
    set_mouse_off();
  }
}

void UI::Action() {
  // hover action
  if (!is_mouse_on_previous() && is_mouse_on())
    BeginHoverAction();
  else if (is_mouse_on_previous() && is_mouse_on())
    OnHoverAction();
  else if (is_mouse_on_previous() && !is_mouse_on())
    EndHoverAction();

  // tap action
  if (is_left_button_tapped())
    LeftButtonTappedAction();

  // click action
  if (is_left_button_clicked())
    LeftButtonClickedAction();
}

// end of ui.cpp