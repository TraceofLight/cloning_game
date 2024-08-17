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

void UI::Tick() {
  final_position_ = position();
  if (parent_)
    final_position_ += parent_->final_position();
}

void UI::FinalTick() {
  Object::FinalTick();
  for (const auto iter : child_vector_)
    iter->FinalTick();
}

/**
 * @brief UI 렌더링하는 함수, 본인을 렌더링하고 child도 순차적으로 렌더링한다.
 */
void UI::Render() {
  RenderUI();
  for (const auto iter : child_vector_)
    iter->Render();
}

void UI::RenderUI() {
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

void UI::CheckMouseOn() {
  // 마우스가 UI 위에 올라와있는지 확인한다.
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
