/**
 * @file object_base.h
 * @brief 전체 오브젝트들의 베이스 헤더
 */

#ifndef TOUHOU_OBJECT_BASE_H_
#define TOUHOU_OBJECT_BASE_H_

#include "common/base/base.h"
#include "common/common_utility.h"
#include "include/component/component.h"

class Component;

class ObjectBase : public Base {
private:
  Vector2 position_;
  Vector2 scale_;
  vector<Component *> component_vector_;

public:
  ObjectBase();
  explicit ObjectBase(const ObjectBase &other);
  virtual ~ObjectBase();

  virtual void Tick() = 0;
  virtual void FinalTick();
  virtual void Render();

  virtual ObjectBase *Clone() override = 0;

  // Getter & Setter
  Vector2 position() const { return position_; }
  Vector2 scale() const { return scale_; }
  void set_position(const Vector2 position) { position_ = position; }
  void set_scale(const Vector2 scale) { scale_ = scale; }
};

#endif // TOUHOU_OBJECT_BASE_H_
