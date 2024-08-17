/**
 * @file texture.cpp
 * @brief
 *
 */

#include "include/asset/texture/texture.h"
#include "include/core/engine/engine.h"

Texture::Texture() : device_context_handle_(nullptr), bitmap_handle_(nullptr), bitmap_info_{} {}

Texture::~Texture() {
  if (device_context_handle_ != nullptr)
    DeleteDC(device_context_handle_);
  if (bitmap_handle_ != nullptr)
    DeleteObject(bitmap_handle_);
}

/**
 * @brief Texture 생성 뒤 특정 크기로 세팅
 * back buffer에서 사용하는 것을 기준으로 만들어져 있음
 * @param width
 * @param height
 * @return load status를 반환
 */
int Texture::Create(const int width, const int height) {
  const HDC device_context = Engine::Get()->device_context();

  // BackBuffer 및 Device Context 생성
  bitmap_handle_ = CreateCompatibleBitmap(device_context, width, height);
  device_context_handle_ = CreateCompatibleDC(device_context);

  // SelectObject: 기존 Default Buffer와 새로 만든 Buffer를 교체
  // DeleteObject: 반환값으로 나온 Default Buffer를 제거
  DeleteObject(SelectObject(device_context_handle_, bitmap_handle_));

  // 로드한 비트맵의 정보 가져옴
  GetObject(bitmap_handle_, sizeof(BITMAP), &bitmap_info_);

  return S_OK;
}

int Texture::Load(const wstring& file_path) {
  // TODO(KHJ): 파일 이름이 길어지면 사이즈를 늘려야 한다.
  wchar_t szExt[50];
  _wsplitpath_s(file_path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

  if (!wcscmp(szExt, L".bmp") || !wcscmp(szExt, L".BMP")) {
    // 지정된 경로로부터 비트맵 이미지를 메모리에 로딩하고 핸들값을 받아둠
    bitmap_handle_ = static_cast<HBITMAP>(LoadImage(nullptr, file_path.c_str(), IMAGE_BITMAP, 0, 0,
                                                    LR_LOADFROMFILE | LR_CREATEDIBSECTION));

    // 로딩이 정상적으로 되었는지 비트맵 핸들값을 확인
    if (bitmap_handle_ == nullptr) {
      const DWORD err = GetLastError();
      wchar_t buffer[256] = {};
      (void)swprintf_s(buffer, 256, L"Texture Load Failure@nError Code : %d", err);
      MessageBox(nullptr, buffer, L"에러 발생", MB_OK);

      return E_FAIL;
    }
  }

  else if (!wcscmp(szExt, L".png") || !wcscmp(szExt, L".PNG")) {
    ULONG_PTR gdiplustoken = 0;
    const GdiplusStartupInput input = {};
    GdiplusStartup(&gdiplustoken, &input, nullptr);

    Image* image = Image::FromFile(file_path.c_str());
    Bitmap* bitmap = reinterpret_cast<Bitmap*>(image->Clone());
    delete image;

    bitmap->GetHBITMAP(Color(0, 0, 0, 0), &bitmap_handle_);
  }

  device_context_handle_ = CreateCompatibleDC(Engine::Get()->device_context());
  DeleteObject(SelectObject(device_context_handle_, bitmap_handle_));
  GetObject(bitmap_handle_, sizeof(BITMAP), &bitmap_info_);

  return S_OK;
}
