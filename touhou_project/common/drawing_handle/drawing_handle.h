/**
 * @file drawing_handle.h
 * @brief pen, brush 관리 용이성을 위한 클래스 헤더
 */

#ifndef TOUHOU_CORE_DRAWING_HANDLE_H_
#define TOUHOU_CORE_DRAWING_HANDLE_H_

#include "common/common_utility.h"

class DrawingHandle {
 private:
  HDC dc_handle_;
  HGDIOBJ previous_gdi_handle_;

 public:
  DrawingHandle(HDC dc_handle, PEN_TYPE pen_type);
  DrawingHandle(HDC dc_handle, BRUSH_TYPE brush_type);
  ~DrawingHandle();
};

#endif  // TOUHOU_CORE_DRAWING_HANDLE_H_
