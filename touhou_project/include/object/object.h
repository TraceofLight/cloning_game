/**
 * @file object.h
 * @brief 전체 오브젝트들의 베이스 헤더
 */

#ifndef TOUHOU_OBJECT_H_
#define TOUHOU_OBJECT_H_

#include "common/base/base.h"
#include "common/common_utility.h"
#include "include/component/component.h"

class Component;

class Object : public Base {
 private:
  Vector2 position_;
  Vector2 scale_;
  vector<Component*> component_vector_;
  LAYER_TYPE layer_type_;

 public:
  Object();
  explicit Object(const Object& other);
  virtual ~Object() override;

  virtual void Tick() = 0;
  virtual void FinalTick();
  virtual void Render();

  virtual Object* Clone() override = 0;

  // Getter & Setter
  Vector2 position() const { return position_; }

  Vector2 scale() const { return scale_; }

  LAYER_TYPE layer_type() const { return layer_type_; }

  void set_position(const Vector2& position) { position_ = position; }

  void set_scale(const Vector2& scale) { scale_ = scale; }

  void set_layer_type(const LAYER_TYPE type) { layer_type_ = type; }

  /**
   * @brief 컴포넌트를 오브젝트에 추가하는 함수
   * @tparam T 컴포넌트 타입 전체
   * @param component 컴포넌트의 ptr
   */
  template <typename T>
  void set_component(T* component) {
    component_vector_.push_back(move(component));
    component->set_owner(this);
  }

  /**
   * @brief 오브젝트가 가진 특정 속성의 컴포넌트를 탐색하는 함수
   * @tparam T componenent
   * @return 해당 타입에 해당하는 component의 ptr / nullptr
   */
  template <typename T>
  T* component() {
    for (auto iter : component_vector_) {
      if (iter->component_type() == T::type_) {
        return static_cast<T *>(iter);
      }
    }
    return nullptr;
  }
};

#endif // TOUHOU_OBJECT_H_
