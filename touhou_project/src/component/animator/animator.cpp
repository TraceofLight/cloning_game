/**
 * @file animator.cpp
 * @brief
 */

#include "include/component/animator/animator.h"

Animator::Animator()
    : Component(COMPONENT_TYPE::ANIMATOR), current_animation_(nullptr), is_repeat_(false) {}

Animator::Animator(const Animator& other)
    : Component(other), current_animation_(nullptr), is_repeat_(other.is_repeat_) {
  for (auto iter = other.animation_map_.begin(); iter != other.animation_map_.end(); ++iter) {
    Animation* clone_animation = iter->second->Clone();

    clone_animation->set_animator(this);
    animation_map_.insert(make_pair(iter->first, clone_animation));
  }

  if (other.current_animation_) {
    current_animation_ = FindAnimation(other.current_animation_->name());
  }
}

Animator::~Animator() {
  ReleaseMap(animation_map_);
}

/**
 * @brief 이름에 해당하는 animation을 찾아서 current 지정하는 메서드
 * @param name
 * @param is_repeat
 */
void Animator::Play(const wstring& name, const bool is_repeat) {
  current_animation_ = FindAnimation(name);
  current_animation_->Reset();

  is_repeat_ = is_repeat;
}

/**
 * @brief 재생 중인 animation에 대해서 Tick 진행하는 함수
 */
void Animator::FinalTick() {
  if (!current_animation_)
    return;
  if (current_animation_->is_finish_ && is_repeat_)
    current_animation_->Reset();
  current_animation_->FinalTick();
}

/**
 * @brief 현재 animation에 대해서 rendering 하는 함수
 */
void Animator::Render() const {
  if (!current_animation_)
    return;
  current_animation_->Render();
}

/**
 * @brief animation map에서 해당 이름을 가진 animation을 탐색하는 메서드
 * @param name
 * @return Animation * / nullptr
 */
Animation* Animator::FindAnimation(const wstring& name) {
  auto const pair_info = animation_map_.find(name);
  if (pair_info == animation_map_.end())
    return nullptr;
  return pair_info->second;
}

/**
 * @brief animation의 정보를 통해 animation을 생성하는 메서드
 * @param info 
 */
void Animator::CreateAnimation(const AnimationDescription& info) {
  Animation* animation = FindAnimation(info.name_);
  // 동일 이름의 animation이 존재하고 있으면 만들 수 없다.
  assert(animation == nullptr);

  // animation 생성 후 정보 주입
  animation = new Animation;
  animation->Create(info);
  animation->set_animator(this);

  animation_map_.insert({info.name_, animation});
}

void Animator::SaveAnimation(const wstring &relative_folder) {
  // TODO(KHJ): path 받아와서 처리하는 기능 필요
}

void Animator::LoadAnimation(const wstring &relative_path) {
  // TODO(KHJ): path 받아와서 처리하는 기능 필요
}
