/**
 * @file level_manager.cpp
 * @brief
 */

#include "include/manager/level_manager.h"

LevelManager::LevelManager() = default;

LevelManager::~LevelManager() {
  for (const auto single_level : level_arr_) {
    if (single_level != nullptr)
      delete single_level;
  }
}

void LevelManager::Init() {
  // TODO(KHJ): 에디터 레벨 내용 추가하기
  level_arr_[static_cast<int>(LEVEL_TYPE::EDITOR)] = nullptr;

  current_level_ = level_arr_[static_cast<int>(LEVEL_TYPE::EDITOR)];
  current_level_->Init();
}

void LevelManager::Tick() {
  current_level_->Tick();
  current_level_->FinalTick();
}

void LevelManager::Render() { current_level_->Render(); }

void LevelManager::SetLevel(LEVEL_TYPE type) {
  if (current_level_ != nullptr)
    current_level_->Exit();

  current_level_ = level_arr_[static_cast<int>(type)];
  current_level_->Init();
}