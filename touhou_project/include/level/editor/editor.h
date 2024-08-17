/**
 * @file editor.h
 * @brief 에디터에서만 사용할 수 있는 오브젝트들을 처리하는 클래스
 */

#ifndef TOUHOU_LEVEL_EDITOR_H_
#define TOUHOU_LEVEL_EDITOR_H_

#include "include/level/level.h"

class EditorLevel : public Level {
 private:
  HMENU menu_handle_;

 public:
  EditorLevel();
  ~EditorLevel() override;
  void SaveFrame();
  void LoadFrame();

 private:
  virtual void Init() override;
  virtual void Exit() override;
  virtual void Tick() override;
};

#endif // TOUHOU_LEVEL_EDITOR_H_
