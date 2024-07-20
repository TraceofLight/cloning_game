/**
 * @file player.cpp
 * @brief
 *
 */

#include "include/object/player/player.h"

#include "include/manager/time_manager.h"

Player::Player() : speed_(300) {}

Player::Player(const Player &other) : ObjectBase(other) {}

Player::~Player() {}

void Player::Tick() {
  float delta_time = TimeManager::Get()->float_delta_time();
  // TODO(KHJ): 움직일 수 있도록 처리할 것
}

void Player::Render() {}
