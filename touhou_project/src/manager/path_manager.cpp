/**
 * @file path_manager.cpp
 * @brief
 */

#include "include/manager/path_manager.h"

PathManager::PathManager() = default;

PathManager::~PathManager() = default;

void PathManager::Init() {
  const filesystem::path current_path = filesystem::current_path();
  content_path_ = current_path.parent_path();
  content_path_ /= "content";

  try {
    if (!exists(content_path_))
      create_directory(content_path_);
  } catch (const filesystem::filesystem_error& e) {
    cerr << "FILE SYSTEM ERROR: " << e.what() << "\n";
  }
}

// end of path_manager.cpp
