/**
 * @file animation_struct.h
 * @brief animation 모듈에서 사용할 자료 구조를 모아둔 헤더
 */

#ifndef TOUHOU_MODULE_COMMON_ANIMATION_STRUCT_H_
#define TOUHOU_MODULE_COMMON_ANIMATION_STRUCT_H_

#include "common/common_utility.h"

class Texture;

struct AnimationDescription {
  wstring name_;
  Texture *atlas_;
  Vector2 start_left_top_;
  Vector2 slice_size_;
  int frame_count_;
  int fps_;
};

struct AnimationFrame {
  Vector2 left_top_;
  Vector2 slice_;
  Vector2 offset_;
  float duration_;
};

#endif // TOUHOU_MODULE_COMMON_ANIMATION_STRUCT_H_
