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

UI::UI(const UI& other) : Object(other) {
  for (const auto iter : other.child_vector())
    add_child(iter->Clone());
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

  CheckMouseOn();
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

  if (is_mouse_on_)
    pen_type = PEN_TYPE::BLUE;
  if (is_left_button_down_)
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
 * @brief 마우스 커서의 위치를 확인하고 커서가 위에 위치하는 경우라면 멤버 변수에 반영하는 함수
 */
void UI::CheckMouseOn() {
  is_mouse_on_previous_ = is_mouse_on_;

  const Vector2 mouse_position = KeyManager::Get()->mouse_position();
  const Vector2 scale = Object::scale();

  const bool is_in_condition = final_position_.x() <= mouse_position.x() &&
                               mouse_position.x() <= final_position_.x() + scale.x() &&
                               final_position_.y() <= mouse_position.y() &&
                               mouse_position.y() <= final_position_.y() + scale.y();

  if (is_in_condition)
    is_mouse_on_ = true;
  else
    is_mouse_on_ = false;
}
