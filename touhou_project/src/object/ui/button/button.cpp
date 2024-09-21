/**
 * @file button.cpp
 * @brief
 **/

#include "include/object/ui/button/button.h"

#include "common/drawing_handle/drawing_handle.h"
#include "include/core/engine/engine.h"

void Button::RenderSelf() {
  PEN_TYPE pen_type = PEN_TYPE::GREEN;
  BRUSH_TYPE brush_type = BRUSH_TYPE::BLUE;

  if (is_mouse_on())
    pen_type = PEN_TYPE::YELLOW;
  if (is_left_button_pressed())
    pen_type = PEN_TYPE::RED;

  DrawingHandle drawing_pen(Engine::Get()->GetBackDC(), pen_type);
  DrawingHandle drawing_brush(Engine::Get()->GetBackDC(), brush_type);

  const Vector2 final_position = UI::final_position();
  const Vector2 scale = Object::scale();
  Rectangle(Engine::Get()->GetBackDC(), static_cast<int>(final_position.x()),
            static_cast<int>(final_position.y()),
            static_cast<int>(final_position.x()) + static_cast<int>(scale.x()),
            static_cast<int>(final_position.y()) + static_cast<int>(scale.y()));
}

void Button::LeftButtonClickedAction() {
  if (call_back_ptr_)
    call_back_ptr_();

  if (instance_ && base_function_ptr_)
    (instance_->*base_function_ptr_)();
}
