/**
 * @file button.h
 * @brief 버튼 UI의 클래스 헤더
 */

#ifndef TOUHOU_OBJECT_UI_BUTTON_H_
#define TOUHOU_OBJECT_UI_BUTTON_H_

#include <functional>

#include "include/object/ui/ui.h"

class Button : public UI {
 private:
  Base* instance_;
  void (*call_back_ptr_)(void);
  void (Base::*base_function_ptr_)(void);
  // TODO(KHJ): 필요성 잘 모르겠음 추후 필요하면 활성화
  // void (Base::*function_ptr_type1_)(DWORD_PTR);
  // void (Base::*function_ptr_type2_)(DWORD_PTR, DWORD_PTR);

  // TODO(KHJ): 버튼 외관 추가 시 활성화
  // Texture *normal_image_;
  // Texture *hover_image_;
  // Texture *pressed_image_;

 public:
  CLONE(Button)
  Button() = default;

  void TickSelf() override {}

  void RenderSelf() override;

  void LeftButtonTappedAction() override {}

  void LeftButtonClickedAction() override;

  // Setter
  void set_call_back(void (*call_back_ptr)(void)) { call_back_ptr_ = call_back_ptr; }

  // Base를 상속 받은 클래스의 인스턴스가 일을 직접하도록 만드는 함수
  template<typename T>
  void set_delegate(T* instance, void (T::*base_function)()) {
    instance_ = static_cast<Base *>(instance);
    base_function_ptr_ =  reinterpret_cast<void (Base::*)()>(base_function);
  }
};

#endif // TOUHOU_OBJECT_UI_BUTTON_H_
