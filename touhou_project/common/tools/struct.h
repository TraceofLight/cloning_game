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
  Vector2() : x_(0), y_(0) {}
  Vector2(double const x, double const y) : x_(x), y_(y) {}

  double Length() const { return std::sqrt(pow(x_, 2) + pow(y_, 2)); }

  void Normalize() {
    double const length = Length();
    assert(length);
    x_ /= length;
    y_ /= length;
  }

  double Distance(Vector2 const &other) const {
    return std::sqrt(pow(x_ - other.x(), 2) + pow(y_ - other.y(), 2));
  }

  Vector2 &operator=(POINT point) {
    x_ = point.x;
    y_ = point.y;
    return *this;
  }

  bool operator==(Vector2 const &other) const {
    if (IsSame(x_, other.x()) && IsSame(y_, other.y()))
      return true;
    return false;
  }

  // Getter & Setter
  double x() const { return x_; }
  double y() const { return y_; }
  void SetX(float const x) { x_ = x; }
  void SetY(float const y) { y_ = y; }
};

#endif // TOUHOU_STRUCT_H_