/**
 * @file path_manager.cpp
 * @brief
 */

#include "include/manager/path_manager.h"

PathManager::PathManager() = default;

PathManager::~PathManager() = default;

void PathManager::Init() {
  const filesystem::path current_path = filesystem::current_path();
  // TODO(KHJ): release 버전에서 따로 인식할 수 있도록 혹은 자동으로 세팅할 수 있도록 할 것
  asset_base_path_ = current_path.parent_path() / R"(touhou_project\common\asset)";
  animation_base_path_ = current_path.parent_path() /  R"(touhou_project\common\animation)";

  try {
    if (!exists(asset_base_path_))
      create_directory(asset_base_path_);
  } catch (const filesystem::filesystem_error& e) {
    cerr << "FILE SYSTEM ERROR: " << e.what() << "\n";
  }
}

// end of path_manager.cpp
