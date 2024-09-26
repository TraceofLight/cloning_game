/**
 * @file asset_manager.h
 * @brief 에셋을 관리하는 매니저
 */

#ifndef TOUHOU_ASSET_MANAGER_H_
#define TOUHOU_ASSET_MANAGER_H_

#include "common/common_utility.h"

class Texture;
class Sound;

class AssetManager {
  SINGLE(AssetManager)
 private:
  map<string, unique_ptr<Texture, HandleObjectDeleterWrapper>> texture_hash_;
  map<string, unique_ptr<Sound, HandleObjectDeleterWrapper>> sound_hash_;

 public:
  void Init();

  Texture* LoadTexture(const string& key, const filesystem::path& relative_path);
  Texture* FindTexture(const string& key);
  Texture* CreateTexture(const string& name, uint32_t width, uint32_t height);
  Sound* LoadSound(const string& key, const string& relative_path);
  Sound* FindSound(const string& key);
};

#endif  // TOUHOU_ASSET_MANAGER_H_
