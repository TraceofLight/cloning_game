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
  uint8_t mouse_status_flag_;

 public:
  static constexpr uint8_t kLeftButtonNone = 1 << 0;
  static constexpr uint8_t kLeftButtonTap = 1 << 1;
  static constexpr uint8_t kLeftButtonPressed = 1 << 2;
  static constexpr uint8_t kLeftButtonReleased = 1 << 3;
  static constexpr uint8_t kMouseHover = 1 << 4;
  static constexpr uint8_t kMouseHoverPrevious = 1 << 5;

  UI();
  ~UI() override;
  UI(const UI& other);
  virtual UI* Clone() override = 0;

  void Tick() final;
  void FinalTick() override;
  void Render() final;

  void UpdateMouseStatus(uint8_t status_flag);
  void Action();
  void Reset() { mouse_status_flag_ = 0; }

  virtual void TickSelf() = 0;
  virtual void RenderSelf();

  // BeginHover: UI에 마우스 커서가 올라온 그 시점
  // OnHover: 마우스 커서가 UI 위에 존재
  // EndHover: 마우스 커서가 UI를 빠져나가는 그 시점
  // LeftButtonTapped: 마우스 좌측 버튼 누르기 시작하는 시점
  // LeftButtonClicked: 클릭 시점 (눌렀다가 떼기 시작하는 시점)

  virtual void BeginHoverAction() {}
  virtual void OnHoverAction() {}
  virtual void EndHoverAction() {}
  virtual void LeftButtonTappedAction() {}
  virtual void LeftButtonClickedAction() {}

  bool is_mouse_on() const { return mouse_status_flag_ & kMouseHover; }
  bool is_mouse_on_previous() const { return mouse_status_flag_ & kMouseHoverPrevious; }
  bool is_left_button_tapped() const { return mouse_status_flag_ & kLeftButtonTap; }
  bool is_left_button_pressed() const { return mouse_status_flag_ & kLeftButtonPressed; }
  bool is_left_button_clicked() const { return mouse_status_flag_ & kLeftButtonReleased; }

  UI* parent() const { return parent_; }
  const vector<UI*>& child_vector() const { return child_vector_; }
  Vector2 final_position() const { return final_position_; }

  void AddChild(UI* child) {
    child->set_parent(this);
    child_vector_.push_back(child);
  }

 private:
  virtual void UpdateMouseHoveringInfo();
  void set_mouse_on() { mouse_status_flag_ |= kMouseHover; }
  void set_mouse_off() { mouse_status_flag_ &= ~kMouseHover; }
  void set_mouse_on_previous() { mouse_status_flag_ |= kMouseHoverPrevious; }
  void set_mouse_off_previous() { mouse_status_flag_ &= ~kMouseHoverPrevious; }
  void set_parent(UI* parent) { parent_ = parent; }
};

#endif // TOUHOU_OBJECT_UI_H_