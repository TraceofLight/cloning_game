/**
 * @file component.h
 * @brief object들의 속성을 추가하는 컴포넌트 class의 헤더
 */

#ifndef TOUHOU_COMPONENT_H_
#define TOUHOU_COMPONENT_H_

#include "include/object/object.h"
#include "common/base/base.h"

class Component : public Base {
  friend class Object;

private:
  const COMPONENT_TYPE component_type_;
  Object *owner_;

public:
  Component(COMPONENT_TYPE type);
  Component(const Component &other);
  ~Component() override = default;

  virtual Component *Clone() override = 0;
  virtual void FinalTick() = 0;

  // Getter & Setter
  COMPONENT_TYPE component_type() const { return component_type_; }
  Object *owner() const { return owner_; }
};

#endif // TOUHOU_COMPONENT_H_