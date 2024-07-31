/**
 * @file drawing_handle.cpp
 * @brief
 */

#include "common/drawing_handle/drawing_handle.h"
#include "include/core/engine/engine.h"

/**
 * @brief 생성자를 통한 pen 교체
 * 교체한 뒤 기존에 사용하던 pen type을 멤버 변수로 저장한다.
 * @param dc_handle device context handle
 * @param pen_type 교체할 펜 타입
 */
DrawingHandle::DrawingHandle(HDC dc_handle, PEN_TYPE pen_type)
    : dc_handle_(dc_handle), previous_gdi_handle_(nullptr) {
  previous_gdi_handle_ = SelectObject(dc_handle_, Engine::Get()->pen(pen_type));
}

/**
 * @brief 생성자를 통한 brush 교체
 * 교체한 뒤 기존에 사용하던 brush type을 멤버 변수로 저장한다.
 * @param dc_handle device context handle
 * @param brush_type 교체할 브러쉬 타입
 */
DrawingHandle::DrawingHandle(HDC dc_handle, BRUSH_TYPE brush_type)
    : dc_handle_(dc_handle), previous_gdi_handle_(nullptr) {
  previous_gdi_handle_ =
      SelectObject(dc_handle_, Engine::Get()->brush(brush_type));
}

/**
 * @brief 소멸자를 통한 handle 기본값 회귀
 * 여기서 멤버에 담아둔 기존 pen, brush로 되돌려 놓는다.
 */
DrawingHandle::~DrawingHandle() {
  SelectObject(dc_handle_, previous_gdi_handle_);
}
