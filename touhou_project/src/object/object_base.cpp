/**
 * @file object_base.cpp
 * @brief
 */

#include "include/object/object_base.h"
#include "common/drawing_handle/drawing_handle.h"
#include "include/core/engine/engine.h"

ObjectBase::ObjectBase() = default;

/**
 * @brief ObjectBase 복사 생성자
 * @param other
 * component 포인터들도 복사하여 복사된 객체에 넣어준다
 */
ObjectBase::ObjectBase(const ObjectBase &other)
    : Base(other), position_(other.position()), scale_(other.scale()) {
  for (int i = 0; i < static_cast<int>(component_vector_.size()); ++i) {
    component_vector_.emplace_back(other.component_vector_[i]->Clone());
  }
}

/**
 * @brief ObjectBase 소멸자
 * component vector가 가리키는 값들 자원도 해제
 */
ObjectBase::~ObjectBase() { ReleaseVector(component_vector_); }

/**
 * @brief ObjectBase가 가진 모든 Component도 동작하도록 하는 메서드
 */
void ObjectBase::FinalTick() {
  for (Component *component_element : component_vector_) {
    component_element->FinalTick();
  }
}

/**
 * @brief ObjectBase 기본 Render 메서드.
 * Asset 등이 존재하지 않는다면 사각형으로 표시된다.
 */
void ObjectBase::Render() {
  HDC const dc_handle = Engine::Get()->GetBackDC();

  DrawingHandle select_pen(dc_handle, PEN_TYPE::RED);
  DrawingHandle select_brush(dc_handle, BRUSH_TYPE::BLUE);

  Rectangle(dc_handle, static_cast<int>(position().x() - (scale_.x() / 2.f)),
            static_cast<int>(position().y() - (scale_.y() / 2.f)),
            static_cast<int>(position().x() + (scale_.x() / 2.f)),
            static_cast<int>(position().y() + (scale_.y() / 2.f)));
}
