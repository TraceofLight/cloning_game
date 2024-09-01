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
  filesystem::path content_path_;

 public:
  filesystem::path& content_path() { return content_path_; }
  void Init();
};

#endif // TOUHOU_PATH_MANAGER_H_
