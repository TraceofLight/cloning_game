﻿/**
 * @file asset.h
 * @brief 실제 file로부터 불러오는 asset의 메모리 구조체 정의 헤더
 */

#ifndef TOUHOU_ASSET_H_
#define TOUHOU_ASSET_H_

#include "common/base/base.h"

class Asset : public Base {
 private:
  wstring key_;
  wstring relative_path_;

 public:
  CLONE_DISABLE(Asset)
  Asset() = default;
  virtual ~Asset() override = default;

  // Getter & Setter
  const wstring& key() { return key_; }

  const wstring& relative_path() { return relative_path_; }

  void set_key(const wstring& key) { key_ = key; }

  void set_relative_path(const wstring& relative_path) { relative_path_ = relative_path; }

 private:
  virtual int Load(const wstring& file_path) = 0;
};

#endif // TOUHOU_ASSET_H_
