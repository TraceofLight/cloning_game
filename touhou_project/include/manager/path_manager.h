/**
 * @file path_manager.h
 * @brief 화면에 출력할 레벨을 관리하는 클래스
 */

#ifndef TOUHOU_PATH_MANAGER_H_
#define TOUHOU_PATH_MANAGER_H_

#include "common/common_utility.h"

class PathManager {
  SINGLE(PathManager)
 private:
  filesystem::path asset_base_path_;
  filesystem::path animation_base_path_;

 public:
  filesystem::path& asset_base_path() { return asset_base_path_; }
  filesystem::path& animation_base_path() { return animation_base_path_; }
  void Init();
};

#endif // TOUHOU_PATH_MANAGER_H_
