/**
 * @file engine.cpp
 * @brief
 */

#include "include/core/engine/engine.h"

#include "common/drawing_handle/drawing_handle.h"
#include "include/manager/key_manager.h"
#include "include/manager/time_manager.h"

Engine::Engine()
    : main_handle_(nullptr), device_context_(nullptr), back_buffer_(nullptr) {}
Engine::~Engine() { ReleaseDC(main_handle_, device_context_); }

void Engine::Init(HWND main_handle, UINT width, UINT height) {
  main_handle_ = main_handle;
  device_context_ = GetDC(main_handle_);

  // 해상도 적용하고 GDI 초기화
  ChangeResolution(width, height);
  GDIInit();

  // BackBuffer Texture 생성
  back_buffer_ = AssetManager::Get()->CreateTexture(
      L"BackBufferTexture", static_cast<int>(width), static_cast<int>(height));

  // Manager 초기화
  TimeManager::Get()->Init();
  KeyManager::Get()->Init();
}

void Engine::Progress() const {
  // Manager Tick
  TimeManager::Get()->Tick();
  KeyManager::Get()->Tick();

  // Render
  Render();
}

void Engine::ChangeResolution(UINT width, UINT height) {
  // 변경하고자 하는 해상도 정보를 Engine 객체에 저장
  resolution_ = Vector2(width, height);

  // 크기를 변경하려는 윈도우의 메뉴 존재여부를 확인
  const HMENU menu_handle = GetMenu(main_handle_);

  // 윈도우의 그리기 영역의 해상도를 원하는 수치로 만들기 위해서
  // 실제 윈도우가 가져야 하는 크기를 계산
  RECT rt = {0, 0, static_cast<long>(width), static_cast<long>(height)};
  AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, static_cast<bool>(menu_handle));

  // 최종으로 계산된 윈도우 크기값으로 윈도우 크기를 변경
  SetWindowPos(main_handle_, nullptr, 0, 0, rt.right - rt.left,
               rt.bottom - rt.top, 0);
}

HDC Engine::GetBackDC() const { return back_buffer_->dc_handle(); }

/**
 * @brief 1프레임 단위로 렌더링하기 위한 함수
 */
void Engine::Render() const {
  // Clearing Screen
  DrawingHandle drawing_handle(GetBackDC(), BRUSH_TYPE::GRAY);
  Rectangle(GetBackDC(), -1, -1, static_cast<int>(resolution_.x()) + 1,
            static_cast<int>(resolution_.y()) + 1);
  BitBlt(device_context_, 0, 0, static_cast<int>(resolution_.x()),
         static_cast<int>(resolution_.y()), GetBackDC(), 0, 0, SRCCOPY);
}

/**
 * @brief Graphic Device Interface를 초기 구축하는 함수
 */
void Engine::GDIInit() {
  pen_list_.reset(new HPEN[static_cast<int>(PEN_TYPE::END)]);
  brush_list_.reset(new HBRUSH[static_cast<int>(BRUSH_TYPE::END)]);

  ADD_PEN(PEN_TYPE::RED, 255, 0, 0);
  ADD_PEN(PEN_TYPE::GREEN, 0, 255, 0);
  ADD_PEN(PEN_TYPE::BLUE, 0, 0, 255);
  ADD_PEN(PEN_TYPE::YELLOW, 255, 255, 0);
  ADD_PEN(PEN_TYPE::MAGENTA, 255, 0, 255);

  ADD_BRUSH(BRUSH_TYPE::RED, 255, 0, 0);
  ADD_BRUSH(BRUSH_TYPE::GREEN, 0, 255, 0);
  ADD_BRUSH(BRUSH_TYPE::BLUE, 0, 0, 255);
  ADD_BRUSH(BRUSH_TYPE::GRAY, 100, 100, 100);
  ADD_BRUSH(BRUSH_TYPE::TURQUOISE, 57, 215, 237);

  // 투명 브러쉬
  brush_list_[static_cast<int>(BRUSH_TYPE::HOLLOW)] =
      static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));
}