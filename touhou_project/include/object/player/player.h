/**
 * @file player.h
 * @brief 오브젝트 중 플레이어 클래스의 헤더
 */

#ifndef TOUHOU_OBJECT_PLAYER_H_
#define TOUHOU_OBJECT_PLAYER_H_

#include "include/asset/texture/texture.h"
#include "include/object/object_base.h"

class Player : public ObjectBase {
private:
  Texture *texture_;
  float speed_;

public:
  CLONE(Player) // player가 컨트롤할 object는 다수가 될 수 있음
  Player();
  Player(const Player &other);
  ~Player() override;
  virtual void Tick() override;
  virtual void Render() override;

  // Getter & Setter
  float speed() const { return speed_; }
  void set_speed(const float speed) { speed_ = speed; }
};

#endif  // TOUHOU_OBJECT_PLAYER_H_