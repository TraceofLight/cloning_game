/**
 * @file asset.h
 * @brief 실제 file로부터 불러오는 asset의 메모리 구조체 정의 헤더
 */

#ifndef TOUHOU_ASSET_H_
#define TOUHOU_ASSET_H_

#include "common/base/base.h"

namespace std {
namespace filesystem {
  class path;
}
}

class Asset : public Base {
 private:
  string relative_path_;

 public:
  CLONE_DISABLE(Asset)
  Asset() = default;
  virtual ~Asset() override = default;

  // Getter & Setter
  const string& relative_path() { return relative_path_; }
  void set_relative_path(const string& relative_path) { relative_path_ = relative_path; }

 private:
  virtual int Load(const std::filesystem::path& file_path) = 0;
};

#endif // TOUHOU_ASSET_H_
