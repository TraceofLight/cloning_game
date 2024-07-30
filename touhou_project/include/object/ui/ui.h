﻿/**
 * @file ui.h
 * @brief 전체 UI에 해당하는 오브젝트들의 base header
 */

#ifndef TOUHOU_OBJECT_UI_H_
#define TOUHOU_OBJECT_UI_H_

#include "include/object/object.h"

class UI : public Object {
  // TODO(KHJ): UI 처리할 수 있는 Manager 추가 필요
  // friend class UIManager;
private:
  UI *parent_ = nullptr;
  vector<UI *> child_vector_;
  Vector2 final_position_;

  bool is_mouse_on_ = false;          // 현재 마우스가 UI 위에 있는지
  bool is_mouse_on_previous_ = false; // 이전 프레임에 마우스가 UI 위에 있었는지
  bool is_left_button_down_ = false;  // UI가 마우스 왼쪽 버튼이 눌린 상태인지

public:
  UI();
  ~UI() override;
  UI(const UI &other);
  virtual UI *Clone() override = 0;

  void Tick() final;
  void FinalTick() override;
  void Render() final;

  virtual void TickUI() = 0;
  virtual void RenderUI();

  virtual void BeginHover() {} // 마우스가 UI 위에 막 올라왔을 때
  virtual void OnHover() {}    // 마우스 UI 위에 있을 때
  virtual void EndHover() {}   // 마우스가 UI를 빠져 나가는 순간

  virtual void LeftButtonDown() {}
  virtual void LeftButtonUp() {}
  virtual void LeftButtonClicked() {}

  // Getter & Setter
  UI *parent() const { return parent_; }
  const vector<UI *> &child_vector() const { return child_vector_; }
  Vector2 final_position() const { return final_position_; }
  bool is_mouse_on() const { return is_mouse_on_; }
  bool is_left_button_down() const { return is_left_button_down_; }
  void set_parent(UI *parent) { parent_ = parent; }
  void add_child(UI *child) {
    child->set_parent(this);
    child_vector_.push_back(child);
  }

private:
  virtual void CheckMouseOn();
};

#endif // TOUHOU_OBJECT_UI_H_
