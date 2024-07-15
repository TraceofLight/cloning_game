/**
 * @file texture.cpp
 * @brief
 *
 */

#include "include/asset/texture/texture.h"
#include "include/engine/engine.h"

Texture::Texture()
    : device_context_handle_(nullptr), bitmap_handle_(nullptr), bitmap_info_{} {
}

Texture::~Texture() {
  if (device_context_handle_ != nullptr)
    DeleteDC(device_context_handle_);
  if (bitmap_handle_ != nullptr)
    DeleteObject(bitmap_handle_);
}

int Texture::Load(const wstring &file_path) {
  wchar_t szExt[50];
  _wsplitpath_s(file_path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt,
                50);

  if (!wcscmp(szExt, L".bmp") || !wcscmp(szExt, L".BMP")) {
    // 지정된 경로로부터 비트맵 이미지를 메모리에 로딩하고 핸들값을 받아둠
    bitmap_handle_ = static_cast<HBITMAP>(
        LoadImage(nullptr, file_path.c_str(), IMAGE_BITMAP, 0, 0,
                  LR_LOADFROMFILE | LR_CREATEDIBSECTION));

    // 로딩이 정상적으로 되었는지 비트맵 핸들값을 확인
    if (bitmap_handle_ == nullptr) {
      DWORD err = GetLastError();
      wchar_t buffer[256] = {};
      swprintf_s(buffer, 256, L"Texture Load Failure\nError Code : %d", err);
      MessageBox(nullptr, buffer, L"에러 발생", MB_OK);

      return E_FAIL;
    }
  }

  else if (!wcscmp(szExt, L".png") || !wcscmp(szExt, L".PNG")) {
    ULONG_PTR gdiplustoken = 0;
    GdiplusStartupInput input = {};
    GdiplusStartup(&gdiplustoken, &input, nullptr);

    Image *image = Image::FromFile(file_path.c_str());
    Bitmap *bitmap = reinterpret_cast<Bitmap *>(image->Clone());
    delete image;

    bitmap->GetHBITMAP(Color(0, 0, 0, 0), &bitmap_handle_);
  }

  device_context_handle_ = CreateCompatibleDC(Engine::Get()->device_context());
  DeleteObject(SelectObject(device_context_handle_, bitmap_handle_));
  GetObject(bitmap_handle_, sizeof(BITMAP), &bitmap_info_);

  return S_OK;
}
