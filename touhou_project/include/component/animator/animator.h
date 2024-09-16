/**
 * @file animator.h
 * @brief animation을 표현하는 class의 헤더
 */

#ifndef TOUHOU_COMPONENT_ANIMATOR_H_
#define TOUHOU_COMPONENT_ANIMATOR_H_

#include "include/component/component.h"
#include "include/module/animation/animation.h"

class Animator : public Component {
 private:
  map<string, Animation *> animation_map_;
  Animation *current_animation_;
  bool is_repeat_;

 public:
  MAKE_COMPONENT_TYPE(ANIMATOR)
  CLONE(Animator)
  Animator();
  Animator(const Animator& other);
  ~Animator() override;

  void Play(const string& name, const bool is_repeat);
  virtual void FinalTick() override;
  void Render() const;

  Animation* FindAnimation(const string& name);

  void CreateAnimation(const AnimationDescription& info);
  void SaveAnimation(const string& relative_path);
  void LoadAnimation(const string& relative_path);
};

#endif  // TOUHOU_COMPONENT_ANIMATOR_H_
