/**
 * @file main.cpp
 * @brief program의 시작 지점
 */

#include "common/resource.h"
#include "common/common_utility.h"

// 전역 변수 설정
HINSTANCE HANDLE_INSTANCE = nullptr;

// 순환 종속성 방지용 전방 선언
ATOM GameClassInfo(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  HANDLE_INSTANCE = hInstance;

  // setting된 window class 호출
  GameClassInfo(HANDLE_INSTANCE);
  HWND window_handle = CreateWindowW(
      L"sample class", L"sample title", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0,
      CW_USEDEFAULT, 0, nullptr, nullptr, HANDLE_INSTANCE, nullptr);

  if (!window_handle)
    return FALSE;

  ShowWindow(window_handle, nCmdShow);
  UpdateWindow(window_handle);

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
    }
  }
  return static_cast<int>(msg.wParam);
}

/**
 * @brief 프로그램에 등록할 윈도우 클래스 정보
 * @param hInstance 프로세스 ID
 */
ATOM GameClassInfo(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOUHOUPROJECT));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TOUHOUPROJECT);
  wcex.lpszClassName = L"touhou_project";
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

/**
 * @brief 주 창의 메시지를 처리
 * @param hWnd
 * @param message
 * @param wParam
 * @param lParam
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  return 0;
}

/**
 * @brief About 정보 MsgHandler
 */
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
  case WM_INITDIALOG:
    return TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return TRUE;
    }
    break;
    }
    return  FALSE;
}
