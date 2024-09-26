/**
 * @file rendering_box.cpp
 * @brief
 */

#include "include/object/ui/rendering_box/rendering_box.h"
#include "common/drawing_handle/drawing_handle.h"
#include "include/core/engine/engine.h"
#include "include/manager/asset_manager.h"
#include "include/manager/path_manager.h"

void RenderingBox::RenderSelf() {
  PEN_TYPE pen_type = PEN_TYPE::BLACK;
  BRUSH_TYPE brush_type = BRUSH_TYPE::HOLLOW;

  DrawingHandle drawing_pen(Engine::Get()->GetBackDC(), pen_type);
  DrawingHandle drawing_brush(Engine::Get()->GetBackDC(), brush_type);

  const Vector2 final_position = UI::final_position();
  const Vector2 scale = Object::scale();
  Rectangle(Engine::Get()->GetBackDC(), static_cast<int>(final_position.x()),
            static_cast<int>(final_position.y()),
            static_cast<int>(final_position.x()) + static_cast<int>(scale.x()),
            static_cast<int>(final_position.y()) + static_cast<int>(scale.y()));

  // render target texture
  if (target_texture_) {
    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

    AlphaBlend(Engine::Get()->GetBackDC(),
               static_cast<int>(final_position.x() + (scale.x() - target_texture_->width()) / 2),
               static_cast<int>(final_position.y() + (scale.y() - target_texture_->height()) / 2),
               static_cast<int>(target_texture_->width()),
               static_cast<int>(target_texture_->height()),
               target_texture_->dc_handle(),
               0,
               0,
               static_cast<int>(target_texture_->width()),
               static_cast<int>(target_texture_->height()),
               bf);
  }
}

void RenderingBox::SaveAnimation() {}

void RenderingBox::LoadAnimation() {}

void RenderingBox::LoadAsset() {
  OPENFILENAME ofn = {};
  wchar_t szFile[260] = {};

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = Engine::Get()->main_handle();
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = L"Image Files\0*.bmp;*.png\0All Files\0*.*\0";
  ofn.nFilterIndex = 1;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  if (GetOpenFileNameW(&ofn) == TRUE) {
    try {
      // 파일 경로를 상대 경로로 변환
      filesystem::path full_path(ofn.lpstrFile);
      filesystem::path asset_base_path = PathManager::Get()->asset_base_path();
      filesystem::path relative_path = relative(full_path, asset_base_path);

      // key와 path를 동일하게 적용
      std::string key = R"(\)" + relative_path.string();
      Texture* loaded_texture = AssetManager::Get()->LoadTexture(key, key);

      if (loaded_texture != nullptr) {
        // 텍스처 로드 성공
        target_texture_ = loaded_texture;
      } else {
        // 텍스처 로드 실패
        MessageBoxW(Engine::Get()->main_handle(), L"Failed to load image", L"Error",
                    MB_OK | MB_ICONERROR);
      }
    } catch (const std::exception& e) {
      // 예외 처리
      int size_needed = MultiByteToWideChar(CP_UTF8, 0, e.what(), -1, nullptr, 0);
      std::wstring wstrMessage(size_needed, 0);
      MultiByteToWideChar(CP_UTF8, 0, e.what(), -1, &wstrMessage[0], size_needed);

      MessageBoxW(Engine::Get()->main_handle(), wstrMessage.c_str(), L"Error",
                  MB_OK | MB_ICONERROR);
    }
  }
}

// end of rendering_box.cpp