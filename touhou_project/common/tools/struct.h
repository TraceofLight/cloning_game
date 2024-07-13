/**
 * @file struct.h
 * @brief 개발하면서 공용으로 사용할 자료구조를 모아둔 헤더
 */

#ifndef TOUHOU_STRUCT_H_
#define TOUHOU_STRUCT_H_

#include "function.h"
#include "windef.h"
#include <cassert>
#include <valarray>

class Vector2 {
private:
  double x_;
  double y_;

public:
  Vector2(double x, double y) : x_(x), y_(y) {}

  double Length() { return std::sqrt(pow(x_, 2) + pow(y_, 2)); }

  void Normalize() {
    double length = Length();
    assert(length);
    x_ /= length;
    y_ /= length;
  }

  double Distance(Vector2 &other) {
    return std::sqrt(pow(x_ - other.x(), 2) + pow(y_ - other.y(), 2));
  }

  Vector2 &operator=(POINT point) {
    x_ = point.x;
    y_ = point.y;
  }

  bool operator==(Vector2 &other) {
    if (IsSame(x_, other.x()) && IsSame(y_, other.y()))
      return true;
    return false;
  }

  double x() const { return x_; }
  double y() const { return y_; }
};

#endif // TOUHOU_STRUCT_H_