/**
 * @file texture.h
 * @brief 실제 이미지 파일로부터 불러오는 로딩된 이미지 구조체 정의 헤더
 */

#ifndef TOUHOU_TEXTURE_H_
#define TOUHOU_TEXTURE_H_

#include "include/asset/asset.h"

class AssetManager;

class Texture : public Asset {
 private:
  HDC device_context_handle_;
  HBITMAP bitmap_handle_;
  BITMAP bitmap_info_;

 public:
  friend class AssetManager;

  // Constructor & Destructor
  Texture();
  ~Texture() override;

  // Getter & Setter
  HDC dc_handle() const { return device_context_handle_; }
  uint32_t width() const { return bitmap_info_.bmWidth; }
  uint32_t height() const { return bitmap_info_.bmHeight; }

 private:
  int Create(uint32_t width, uint32_t height);
  int Load(const filesystem::path& file_path) override;
};

#endif // TOUHOU_TEXTURE_H_