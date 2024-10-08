﻿/**
 * @file base.h
 * @brief 모든 클래스가 공통으로 가지는 base class
 */

#ifndef TOUHOU_BASE_H_
#define TOUHOU_BASE_H_

#include "common/common_utility.h"

class Base {
 private:
  // 프로그램 전역에서 관리하는 값
  static UINT global_serial_number_;

  // 각 인스턴스가 발급 받는 값
  const UINT id_;
  wstring name_;

 public:
  Base();
  Base(const Base& other);
  virtual ~Base() = default;
  virtual Base* Clone() = 0;

  // Getter & Setter
  const wstring& name() { return name_; }

  void set_name(const wstring& name) { name_ = name; }
  UINT id() const { return id_; }
};

#endif // TOUHOU_BASE_H_
