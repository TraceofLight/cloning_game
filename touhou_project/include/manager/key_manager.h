/**
 * @file key_manager.h
 * @brief 사용하는 key들의 상태를 관리하는 클래스 헤더
 */

#ifndef TOUHOU_KEY_MANAGER_H_
#define TOUHOU_KEY_MANAGER_H_

#include "common/common_utility.h"

// TODO(KHJ): 해당 자료구조를 따로 분리할 클래스를 만드는 것 고민
struct KeyInformation {
  KEY_STATE state_;  // 현재 상태를 보여주는 멤버
  bool was_pressed_; // 상태 변화 적용할 때 기존 상태 확인용 멤버

  // 생성자 구축
  KeyInformation() : state_(KEY_STATE::NONE), was_pressed_(false) {}
};

class KeyManager {
  SINGLE(KeyManager)
private:
  vector<unique_ptr<KeyInformation>> key_info_vector_;
  Vector2 mouse_position_;

public:
  void Init();
  void Tick();

  // Getter & Setter
  KEY_STATE key_state(const KEY key) const {
    return key_info_vector_[static_cast<int>(key)].get()->state_;
  }
  Vector2 mouse_position() const { return mouse_position_; }
};

#endif // TOUHOU_KEY_MANAGER_H_