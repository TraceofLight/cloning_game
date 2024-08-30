/**
 * @file ui.h
 * @brief 전체 UI에 해당하는 오브젝트들의 base header
 */

#ifndef TOUHOU_OBJECT_UI_H_
#define TOUHOU_OBJECT_UI_H_

#include "include/object/object.h"

class UI : public Object {
  friend class UIManager;

 private:
  UI* parent_ = nullptr;
  vector<UI*> child_vector_;
  Vector2 final_position_;
  bool is_mouse_on_ = false;            // 현재 마우스가 UI 위에 있는지
  bool is_mouse_on_previous_ = false;   // 이전 프레임에 마우스가 UI 위에 있었는지

  // UI Manager가 세팅
  bool is_left_button_down_ = false;    // UI가 마우스 왼쪽 버튼이 눌린 상태인지
  bool is_left_button_clicked_ = false; // UI가 클릭된 상태인지

 public:
  UI();
  ~UI() override;
  UI(const UI& other);
  virtual UI* Clone() override = 0;

  void Tick() final;
  void FinalTick() override;
  void Render() final;

  virtual void TickSelf() = 0;

  virtual void RenderSelf();

  virtual void BeginHoverAction() {} // 마우스가 UI 위에 막 올라왔을 때

  virtual void OnHoverAction() {} // 마우스 UI 위에 있을 때

  virtual void EndHoverAction() {} // 마우스가 UI를 빠져 나가는 순간

  virtual void LeftButtonDownAction() {}

  virtual void LeftButtonUpAction() {}

  virtual void LeftButtonClickedAction() {}

  // Getter & Setter
  UI* parent() const { return parent_; }

  const vector<UI*>& child_vector() const { return child_vector_; }

  Vector2 final_position() const { return final_position_; }

  bool is_mouse_on() const { return is_mouse_on_; }

  bool is_mouse_on_previous() const { return is_mouse_on_previous_; }

  bool is_left_button_down() const { return is_left_button_down_; }

  bool is_left_button_clicked() const { return is_left_button_clicked_; }

  void set_left_button_down(const bool state) { is_left_button_down_ = state; }

  void set_left_button_clicked(const bool state) { is_left_button_clicked_ = state; }

  void set_parent(UI* parent) { parent_ = parent; }

  void add_child(UI* child) {
    child->set_parent(this);
    child_vector_.push_back(child);
  }

 private:
  virtual void CheckMouseOn();
};

#endif // TOUHOU_OBJECT_UI_H_