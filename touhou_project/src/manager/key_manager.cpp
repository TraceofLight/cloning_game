/**
 * @file key_manager.cpp
 * @brief
 */

#include "include/manager/key_manager.h"
#include "include/core/engine/engine.h"

// key enum과 실제 key를 매핑하는 자료 구조
int key_value_hash[static_cast<int>(KEY::END)] = {
    'W',        'S',        'A',        'D',

    VK_LEFT,    VK_RIGHT,   VK_UP,      VK_DOWN,

    VK_SPACE,   VK_RETURN,

    VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, VK_NUMPAD4, VK_NUMPAD5,
    VK_NUMPAD6, VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9, VK_NUMPAD0,

    VK_LBUTTON, VK_RBUTTON,
};

KeyManager::KeyManager() = default;
KeyManager::~KeyManager() = default;

void KeyManager::Init() {
  for (int i = 0; i < static_cast<int>(KEY::END); ++i) {
    key_info_vector_.emplace_back(make_unique<KeyInformation>());
  }
}

/**
 * @brief Key마다 돌면서 상태를 체크하는 함수
 * - 키가 눌려 있는지 확인한다.
 * CASE1. 눌려 있고 이전에 눌려 있지 않았음: 현재 막 눌리기 시작 (TAP)
 * CASE2. 눌려 있고 이전에도 눌려 있던 상태: 눌러지는 중 (PRESSED)
 * CASE3. 눌려 있지 않은데 이전에는 눌려 있었음: 막 뗀 상태 (RELEASED)
 * CASE4. 눌려 있지 않고 이전에도 그렇지 않았음: 눌러지지 않은 상태 (NONE)
 */
void KeyManager::Tick() {
  for (size_t i = 0; i < key_info_vector_.size(); ++i) {

    if (GetAsyncKeyState(key_value_hash[i]) & 0x8001) {
      if (!key_info_vector_[i]->was_pressed_) {
        key_info_vector_[i]->state_ = KEY_STATE::TAP;  // CASE1
      } else {
        key_info_vector_[i]->state_ = KEY_STATE::PRESSED;  // CASE2
      }
      key_info_vector_[i]->was_pressed_ = true;
    } else {
      if (key_info_vector_[i]->was_pressed_) {
        key_info_vector_[i]->state_ = KEY_STATE::RELEASED;  // CASE3
      } else {
        key_info_vector_[i]->state_ = KEY_STATE::NONE; // CASE4
      }

      key_info_vector_[i]->was_pressed_ = false;
    }
  }

  POINT cursor_position;
  GetCursorPos(&cursor_position);
  ScreenToClient(Engine::Get()->main_handle(), &cursor_position);
  mouse_position_ = cursor_position;
}