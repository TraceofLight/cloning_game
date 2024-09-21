/**
 * @file base.h
 * @brief 모든 클래스가 공통으로 가지는 base class
 */

#ifndef TOUHOU_BASE_H_
#define TOUHOU_BASE_H_

#include "common/common_utility.h"

class Base {
 private:
  // 프로그램 전역에서 관리하는 값
  static uint32_t global_serial_number_;

  // 각 인스턴스가 발급 받는 값
  uint32_t id_;
  string name_;

 public:
  Base();
  Base(const Base& other);
  virtual ~Base() = default;
  virtual Base* Clone() = 0;

  // Getter & Setter
  const string& name() { return name_; }
  uint32_t id() const { return id_; }

  void set_name(const string& name) { name_ = name; }
};

#endif // TOUHOU_BASE_H_