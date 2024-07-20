﻿/**
 * @file object.h
 * @brief 전체 오브젝트들의 베이스 헤더
 */

#ifndef TOUHOU_OBJECT_H_
#define TOUHOU_OBJECT_H_

#include "common/base/base.h"
#include "common/common_utility.h"

class Object : public Base {
private:
  Vector2 position_;
  Vector2 scale_;

public:
  Object();
  explicit Object(const Object &other);
  virtual ~Object();

  virtual void Tick() = 0;
  virtual void FinalTick() = 0;
  virtual void Render();

  virtual Object *Clone() = 0;

  // Getter & Setter
  Vector2 position() const { return position_; }
  Vector2 scale() const { return scale_; }
  void set_position(const Vector2 position) { position_ = position; }
  void set_scale(const Vector2 scale) { scale_ = scale; }
};

#endif // TOUHOU_OBJECT_H_
