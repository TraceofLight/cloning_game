/**
 * @file engine.h
 * @brief 기본적인 구조를 담당하는 엔진 모듈의 헤더
 */

#ifndef TOUHOU_CORE_ENGINE_H_
#define TOUHOU_CORE_ENGINE_H_

#include "common/common_utility.h"
#include "include/asset/texture/texture.h"

class Engine {
  SINGLE(Engine)

private:
  HWND main_handle_;
  HDC device_context_;
  Vector2 resolution_;
  Texture *back_buffer_;
  unique_ptr<HPEN[], HandleListDeleterWrapper> pen_list_;
  unique_ptr<HBRUSH[], HandleListDeleterWrapper> brush_list_;

public:
  void Init(HWND main_handle, UINT width, UINT height);
  void Progress();
  void ChangeResolution(UINT width, UINT height);
  HDC GetBackDC() const;

  HWND main_handle() const { return main_handle_; }
  HDC device_context() const { return device_context_; }
  HPEN pen(PEN_TYPE type) const { return pen_list_[static_cast<int>(type)]; }
  HBRUSH brush(BRUSH_TYPE type) const {
    return brush_list_[static_cast<int>(type)];
  }

private:
  void Render() const;
  void GDIInit();
};

#endif // TOUHOU_CORE_ENGINE_H_
