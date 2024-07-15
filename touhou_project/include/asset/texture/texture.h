/**
 * @file texture.h
 * @brief 실제 이미지 파일로부터 불러오는 로딩된 이미지 구조체 정의 헤더
 */

#ifndef TOUHOU_TEXTURE_H_
#define TOUHOU_TEXTURE_H_

#include "include/asset/asset.h"

class Texture : public Asset {
private:
  HDC         device_context_handle_;
  HBITMAP     bitmap_handle_;
  BITMAP      bitmap_info_;

public:
  // Constructor & Destructor
  Texture();
  ~Texture();

  // Getter & Setter
  HDC dc_handle() const { return device_context_handle_; }
  UINT width() const { return bitmap_info_.bmWidth; }
  UINT height() const { return bitmap_info_.bmHeight; }

private:
  int Load(const wstring& file_path) override;
  int Create(UINT width, UINT height);
};

#endif // TOUHOU_TEXTURE_H_
