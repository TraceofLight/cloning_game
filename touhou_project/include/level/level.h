/**
 * @file level.h
 * @brief 일종의 Layer 역할을 하는 클래스의 헤더
 */

#ifndef TOUHOU_LEVEL_H_
#define TOUHOU_LEVEL_H_

#include "common/base/base.h"
#include "include/object/object.h"
#include "include/object/player/player.h"

class Level : public Base {
private:
  vector<Object *> layer_list_[LAYER_TYPE::END];
  Player *current_player_;

public:
  CLONE_DISABLE(Level)
  Level() = default;
  ~Level() override;

  virtual void Init() = 0;
  virtual void Exit() = 0;
  virtual void Tick();
  virtual void FinalTick();
  virtual void Render();

  void DeleteObjects(LAYER_TYPE type);
  void DeleteAllObjects();

  // Getter & Setter
  Player *current_player() const { return current_player_; }
  vector<Object *> &layer(const LAYER_TYPE type) {
    return layer_list_[static_cast<int>(type)];
  }
  void set_current_player(Player *player) { current_player_ = player; }
  void add_object_to_layer(Object *object, const LAYER_TYPE type) {
    object->set_layer_type(type);
    layer_list_->push_back(std::move(object));
  }
};

#endif // TOUHOU_LEVEL_H_
