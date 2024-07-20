/**
 * @file object.cpp
 * @brief
 */

#include "include/object/object.h"
#include "common/drawing_handle/drawing_handle.h"
#include "include/core/engine/engine.h"

Object::Object() = default;

Object::Object(const Object &other)
    : Base(other), position_(other.position()), scale_(other.scale()) {}

Object::~Object() = default;

void Object::Render() {
  HDC const dc_handle = Engine::Get()->GetBackDC();

  DrawingHandle(dc_handle, PEN_TYPE::RED);
  DrawingHandle(dc_handle, BRUSH_TYPE::BLUE);

  Rectangle(dc_handle, static_cast<int>(position().x() - (scale_.x() / 2.f)),
            static_cast<int>(position().y() - (scale_.y() / 2.f)),
            static_cast<int>(position().x() + (scale_.x() / 2.f)),
            static_cast<int>(position().y() + (scale_.y() / 2.f)));
}
