/**
 * @file engine.h
 * @brief 기본적인 구조를 담당하는 엔진 모듈의 헤더
 */

#ifndef TOUHOU_ENGINE_H_
#define TOUHOU_ENGINE_H_

#include "common\common_utility.h"

class Engine {
  SINGLE(Engine);

private:
  HWND main_handle_;
  HDC device_context_;
  std::unique_ptr<HPEN[]> pen_list_;
  std::unique_ptr<HBRUSH[]> brush_list_;

public:
  void Init(HWND main_handle, UINT width, UINT height);
  void Progress();
  void ChangeResolution(UINT width, UINT height);

  HWND main_handle() { return main_handle_; }
  HDC device_context() { return device_context_; }
  HPEN pen() { return pen_list_[0]; }
  HBRUSH brush() { return brush_list_[0]; }

private:
  void Render();
  void GDIInit();
};

#endif // TOUHOU_ENGINE_H_
