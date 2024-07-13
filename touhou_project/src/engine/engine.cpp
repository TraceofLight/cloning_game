/**
 * @file engine.cpp
 * @brief
 */

#include "include/engine/engine.h"

Engine::Engine() : main_handle_(nullptr), device_context_(nullptr) {}
Engine::~Engine() { ReleaseDC(main_handle_, device_context_); }

void Engine::Init(HWND main_handle, UINT width, UINT height) {
  main_handle_ = main_handle;
  device_context_ = GetDC(main_handle_);

  // 해상도 적용하고 GDI 초기화
  ChangeResolution(width, height);
  GDIInit();

  // Manager 초기화
}

void Engine::Progress() {}
void Engine::ChangeResolution(UINT width, UINT height) {}
void Engine::Render() {}

/**
 * @brief Graphic Device Interface를 초기 구축하는 함수
 */
void Engine::GDIInit() {
  pen_list_[static_cast<int>(PEN_TYPE::RED)] =
      CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
  pen_list_[static_cast<int>(PEN_TYPE::GREEN)] =
      CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
  pen_list_[static_cast<int>(PEN_TYPE::BLUE)] =
      CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
  pen_list_[static_cast<int>(PEN_TYPE::YELLOW)] =
      CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
  pen_list_[static_cast<int>(PEN_TYPE::MAGENTA)] =
      CreatePen(PS_SOLID, 1, RGB(255, 0, 255));

  brush_list_[static_cast<int>(BRUSH_TYPE::RED)] =
      CreateSolidBrush(RGB(255, 0, 0));
  brush_list_[static_cast<int>(BRUSH_TYPE::GREEN)] =
      CreateSolidBrush(RGB(0, 255, 0));
  brush_list_[static_cast<int>(BRUSH_TYPE::BLUE)] =
      CreateSolidBrush(RGB(0, 0, 255));
  brush_list_[static_cast<int>(BRUSH_TYPE::GRAY)] =
      CreateSolidBrush(RGB(100, 100, 100));
  brush_list_[static_cast<int>(BRUSH_TYPE::TURQUOISE)] =
      CreateSolidBrush(RGB(57, 215, 237));
  brush_list_[static_cast<int>(BRUSH_TYPE::HOLLOW)] =
      static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));
}