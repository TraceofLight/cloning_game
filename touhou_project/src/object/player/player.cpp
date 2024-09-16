/**
 * @file player.cpp
 * @brief
 *
 */

#include "include/object/player/player.h"
#include "include/manager/key_manager.h"

Player::Player()
  : speed_(300) {
  // animation
  animator_ = set_component(new Animator);

  animator_->LoadAnimation(R"(\player\reimu\idle.animation)");
  animator_->Play("idle", true);
}

Player::Player(const Player& other)
  : Object(other),
    speed_(other.speed()) {
  animator_ = other.animator_->Clone();
}

Player::~Player() = default;

void Player::Tick() {
  // 현재는 animation 명칭만 설정
  if (KEY_CHECK(KEY::LEFT, KEY_STATE::TAP))
    animator_->Play("idle", true);
  if (KEY_CHECK(KEY::LEFT, KEY_STATE::RELEASED))
    animator_->Play("idle", true);
  if (KEY_CHECK(KEY::LEFT, KEY_STATE::PRESSED)) {
    // TODO(KHJ): 움직임에 대한 세부 설정
  }

  if (KEY_CHECK(KEY::RIGHT, KEY_STATE::TAP))
    animator_->Play("idle", true);
  if (KEY_CHECK(KEY::RIGHT, KEY_STATE::RELEASED))
    animator_->Play("idle", true);
  if (KEY_CHECK(KEY::RIGHT, KEY_STATE::PRESSED)) {
    // TODO(KHJ): 움직임에 대한 세부 설정
  }

  // TODO(KHJ): 이하 모션도 세부 설정할 것
  if (KEY_CHECK(KEY::UP, KEY_STATE::TAP)) {
    animator_->Play("idle", true);
  }
  if (KEY_CHECK(KEY::UP, KEY_STATE::RELEASED))
    animator_->Play("idle", true);

  if (KEY_CHECK(KEY::DOWN, KEY_STATE::TAP)) {
    animator_->Play("idle", true);
  }
  if (KEY_CHECK(KEY::DOWN, KEY_STATE::RELEASED)) {
    animator_->Play("idle", true);
  }
  if (KEY_CHECK(KEY::SPACE, KEY_STATE::TAP)) {
    // TODO(KHJ): 대기 동작?
  }
}

void Player::Render() {
  animator_->Render();
}

// end of player.cpp