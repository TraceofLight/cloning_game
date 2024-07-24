/**
 * @file animation.h
 * @brief 애니메이션을 표현하는 모듈 클래스의 헤더
 */

#ifndef TOUHOU_MODULE_ANIMATION_H_
#define TOUHOU_MODULE_ANIMATION_H_

#include "common/animation_struct.h"
#include "common/base/base.h"

class Animation : public Base {
  friend class Animator;

private:
  Animator *animator_;
  vector<AnimationFrame> frame_vector_;
  Texture *atlas_;
  int current_idx_;
  float accumulated_time_;
  bool is_finish_;

public:
  CLONE(Animation)
  Animation();
  Animation(const Animation &other);
  ~Animation() override;

  void FinalTick();
  void Render() const;

  void Save(const wstring &folder_path) const;
  void Load(const wstring &file_path);

  void Create(const AnimationDescription &info);
  void Reset();

  // Getter
  Animator *animator() const { return animator_; }
  AnimationFrame &frame(const int idx) { return frame_vector_[idx]; }
  bool is_finish() const { return is_finish_; }
  void set_animator(Animator *animator) { animator_ = animator; }
};

#endif // TOUHOU_MODULE_ANIMATION_H_
