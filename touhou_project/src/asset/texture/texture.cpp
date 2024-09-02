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

int Texture::Load(const string& file_path) {
  filesystem::path path(file_path);
  string extension = path.extension().string();

  // Convert extension to lowercase for case-insensitive comparison
  transform(extension.begin(), extension.end(), extension.begin(),
            [](const unsigned char c) { return tolower(c); });

  // bmp 타입 처리
  if (extension == ".bmp") {
    wstring wide_path = wstring(file_path.begin(), file_path.end());
    bitmap_handle_ = static_cast<HBITMAP>(LoadImageW(nullptr, wide_path.c_str(), IMAGE_BITMAP,
                                                     0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION));
    if (bitmap_handle_ == nullptr) {
      DWORD err = GetLastError();
      throw runtime_error("Texture Load Failure. Error Code: " + to_string(err));
    }
    // png 타입 처리
  } else if (extension == ".png") {
    ULONG_PTR gdiplustoken = 0;
    GdiplusStartupInput input;
    GdiplusStartup(&gdiplustoken, &input, nullptr);

    wstring wide_path = wstring(file_path.begin(), file_path.end());
    unique_ptr<Image> image(Image::FromFile(wide_path.c_str()));

    if (!image) {
      throw runtime_error("Failed to load image: " + file_path);
    }

    unique_ptr<Bitmap> bitmap(static_cast<Bitmap*>(image->Clone()));
    Color background_color(0, 0, 0, 0);
    bitmap->GetHBITMAP(background_color, &bitmap_handle_);
    GdiplusShutdown(gdiplustoken);

    // 유효하지 않은 타입의 파일 처리
  } else {
    throw runtime_error("Unsupported file format: " + extension);
  }

  device_context_handle_ = CreateCompatibleDC(Engine::Get()->device_context());
  DeleteObject(SelectObject(device_context_handle_, bitmap_handle_));
  GetObject(bitmap_handle_, sizeof(BITMAP), &bitmap_info_);

  return S_OK;
}