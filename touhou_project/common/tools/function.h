/**
 * @file function.h
 * @brief 전역에서 사용할 function 모을 헤더
 */

#ifndef TOUHOU_FUNCTION_H_
#define TOUHOU_FUNCTION_H_

inline bool IsSame(double a, double b) { return fabs(a - b) < LDBL_EPSILON; }

#endif // TOUHOU_FUNCTION_H_
