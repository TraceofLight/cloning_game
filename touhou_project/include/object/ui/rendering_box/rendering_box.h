/**
* @file box.h
 * @brief 버튼 UI의 클래스 헤더
 */

#ifndef TOUHOU_OBJECT_UI_BOX_H_
#define TOUHOU_OBJECT_UI_BOX_H_

#include "include/object/ui/ui.h"
#include "include/asset/texture/texture.h"

class RenderingBox : public UI {
 private:
  Texture* target_texture_;

 public:
  CLONE(RenderingBox)
  RenderingBox() = default;

  void TickSelf() override {}
  void RenderSelf() override;

  void SaveAnimation();
  void LoadAnimation();
  void LoadAsset();
};

#endif // TOUHOU_OBJECT_UI_BOX_H_
