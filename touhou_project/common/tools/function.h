/**
 * @file function.h
 * @brief 전역에서 사용할 function 모을 헤더
 */

#ifndef TOUHOU_FUNCTION_H_
#define TOUHOU_FUNCTION_H_

#include "pch.h"
#include <cfloat>
#include <cmath>

inline bool IsSame(double a, double b) { return fabs(a - b) < LDBL_EPSILON; }

struct HandleObjectDeleterWrapper {
  template <typename T> static void HandleObjectDeleter(T &object_list) {
    for (int i = 0; object_list[i] != nullptr; ++i) {
      DeleteObject(object_list[i]);
    }
    delete[] object_list;
  }

  template <typename T> void operator()(T *object) const {
    HandleObjectDeleter(object);
  }
};

#endif // TOUHOU_FUNCTION_H_
