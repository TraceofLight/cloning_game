/**
 * @file function.h
 * @brief 전역에서 사용할 function 모을 헤더
 */

#ifndef TOUHOU_FUNCTION_H_
#define TOUHOU_FUNCTION_H_

#include "pch.h"
#include <cfloat>
#include <cmath>

inline bool IsSame(const double a, const double b) {
  return fabs(a - b) < LDBL_EPSILON;
}

/**
 * @brief 일반적인 map 자료구조에 대해서 소멸 처리하는 템플릿
 * @tparam T1 map의 key
 * @tparam T2 map의 value
 * @param map
 */
template <typename T1, typename T2> void ReleaseMap(map<T1, T2> &map) {
  for (auto iter = map.begin(); iter != map.end(); ++iter) {
    if (iter->second == nullptr)
      continue;
    delete iter->second;
  }
  map.clear();
}

/**
 * @brief 일반적인 vector 자료 구조에 대해서 소멸 처리하는 템플릿
 * @tparam T vector가 가리키는 오브젝트의 클래스
 * @param vector
 */
template <typename T> void ReleaseVector(vector<T> &vector) {
  for (auto iter = vector.begin(); iter != vector.end(); ++iter) {
    if (*iter == nullptr)
      continue;
    delete *iter;
  }
}

#endif // TOUHOU_FUNCTION_H_
