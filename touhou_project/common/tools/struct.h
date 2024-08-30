/**
 * @file struct.h
 * @brief 개발하면서 공용으로 사용할 자료구조를 모아둔 헤더
 */

#ifndef TOUHOU_STRUCT_H_
#define TOUHOU_STRUCT_H_

#include <cassert>
#include <valarray>
#include "enum.h"
#include "function.h"
#include "windef.h"

class Vector2 {
 private:
  double x_;
  double y_;
  float float_x_;
  float float_y_;

 public:
  Vector2() : x_(0), y_(0), float_x_(0), float_y_(0) {}

  Vector2(double const x, double const y) : x_(x), y_(y), float_x_(0), float_y_(0) {}

  double Length() const { return std::sqrt(pow(x_, 2) + pow(y_, 2)); }

  void Normalize() {
    double const length = Length();
    assert(length);
    x_ /= length;
    y_ /= length;
  }

  double Distance(const Vector2& other) const {
    return std::sqrt(pow(x_ - other.x(), 2) + pow(y_ - other.y(), 2));
  }

  Vector2& operator=(POINT point) {
    x_ = point.x;
    y_ = point.y;
    return *this;
  }

  void operator+=(const Vector2& other) {
    x_ += other.x();
    y_ += other.y();
  }

  bool operator==(const Vector2& other) const {
    if (IsSame(x_, other.x()) && IsSame(y_, other.y()))
      return true;
    return false;
  }

  // Getter & Setter
  double x() const { return x_; }

  double y() const { return y_; }

  void set_x(float const x) { x_ = x; }

  void set_y(float const y) { y_ = y; }

  // fwscanf_s 대응을 위한 Getter
  // float 값을 여기를 제외하면 사용하지 않기 때문에 여기서 lazy하게 sync한다
  float& float_x() {
    float_x_ = static_cast<float>(x_);
    return float_x_;
  }

  float& float_y() {
    float_y_ = static_cast<float>(y_);
    return float_y_;
  }
};

/**
 * @brief unique ptr로 선언된 list의 자원 해제를 위한 소멸자 래퍼
 */
struct HandleListDeleterWrapper {
  template <typename T>
  static void HandleListDeleter(T& object_list) {
    for (int i = 0; object_list[i] != nullptr; ++i) {
      DeleteObject(object_list[i]);
    }
    delete[] object_list;
  }

  template <typename T>
  void operator()(T* object) const {
    HandleListDeleter(object);
  }
};

/**
 * @brief unique ptr로 선언된 obj 자원 해제를 위한 소멸자 래퍼
 */
struct HandleObjectDeleterWrapper {
  template <typename T>
  void operator()(T* object) const {
    DeleteObject(object);
  }
};

struct DebugShapeInfo {
  DEBUG_SHAPE shape;
  Vector2 position;
  Vector2 scale;
  PEN_TYPE pen;
  BRUSH_TYPE brush;
  float duration;  // DebugShape 유지 시간
  float time; // 현재 진행 시간
};

struct LogInfo {
  wstring log_text; // 출력할 메세지
  LOG_LEVEL log_level; // 로그 수준
  float time; // 현재 시간
};

#endif // TOUHOU_STRUCT_H_
