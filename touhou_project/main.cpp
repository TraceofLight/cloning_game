/**
 * @file main.cpp
 * @brief program의 시작 지점
 */

#include "common/common_utility.h"
#include "common/resource.h"
#include "include/core/engine/engine.h"

// 전역 변수 설정
HINSTANCE kInstanceHandle = nullptr;
constexpr wchar_t kClassName[255] = L"touhou_project";

// 순환 종속성 방지용 전방 선언
ATOM WindowClassInfo(HINSTANCE instance_handle);
LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param,
                         LPARAM l_param);
INT_PTR CALLBACK About(HWND h_dialog, UINT message, WPARAM w_param,
                       LPARAM l_param);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  kInstanceHandle = hInstance;

  // setting된 window class 호출
  WindowClassInfo(kInstanceHandle);
  const HWND window_handle = CreateWindowW(
      kClassName, L"Touhou Project Clone", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
      0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  // 여기서 null이면 작동하지 않는다
  assert(window_handle != nullptr);

  if (!window_handle)
    return FALSE;

  ShowWindow(window_handle, true);
  UpdateWindow(window_handle);

  Engine::Get()->Init(window_handle, 1280, 768);

  MSG msg = {};

  while (true) {
    // 메시지큐의 메시지 유무에 따라 T / F 반환
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    } else {
      // 실제 게임 로직
      Engine::Get()->Progress();
    }
  }
  return static_cast<int>(msg.wParam);
}

/**
 * @brief 주 창의 메시지를 처리
 * @param window_handle
 * @param message
 * @param w_param
 * @param l_param
 */

LRESULT CALLBACK WndProc(HWND window_handle, UINT message, WPARAM w_param,
                         LPARAM l_param) {
  switch (message) {
  case WM_COMMAND: {
    int wmId = LOWORD(w_param);
    // 메뉴 선택을 구문 분석합니다:
    switch (wmId) {
    case IDM_ABOUT:
      DialogBox(kInstanceHandle, MAKEINTRESOURCE(IDD_ABOUTBOX), window_handle,
                About);
      break;
    case IDM_EXIT:
      DestroyWindow(window_handle);
      break;
    default:
      return DefWindowProc(window_handle, message, w_param, l_param);
    }
  } break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(window_handle, message, w_param, l_param);
  }
  return 0;
}

/**
 * @brief 프로그램에 등록할 윈도우 클래스 정보
 * @param instance_handle 프로세스 ID
 */
ATOM WindowClassInfo(HINSTANCE instance_handle) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = instance_handle;
  wcex.hIcon = LoadIcon(instance_handle, MAKEINTRESOURCE(IDI_TOUHOUPROJECT));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
  wcex.lpszMenuName = nullptr; // MAKEINTRESOURCEW(IDC_WINAPI);
  wcex.lpszClassName = kClassName;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

/**
 * @brief About 정보 MsgHandler
 */
INT_PTR CALLBACK About(HWND h_dialog, UINT message, WPARAM w_param,
                       LPARAM l_param) {
  UNREFERENCED_PARAMETER(l_param);
  switch (message) {
  case WM_INITDIALOG:
    return TRUE;

  case WM_COMMAND:
    if (LOWORD(w_param) == IDOK || LOWORD(w_param) == IDCANCEL) {
      EndDialog(h_dialog, LOWORD(w_param));
      return TRUE;
    }
    break;
    default:
      break;
    }
    return  FALSE;
}
