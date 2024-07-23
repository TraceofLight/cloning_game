/**
 * @file asset_manager.h
 * @brief 에셋을 관리하는 매니저
 */

#ifndef TOUHOU_ASSET_MANAGER_H_
#define TOUHOU_ASSET_MANAGER_H_

#include "common/common_utility.h"
#include "include/asset/sound/sound.h"
#include "include/asset/texture/texture.h"

class Texture;
class Sound;

class AssetManager {
  SINGLE(AssetManager)
private:
  map<wstring, unique_ptr<Texture, HandleObjectDeleterWrapper>> texture_hash_;
  map<wstring, unique_ptr<Sound, HandleObjectDeleterWrapper>> sound_hash_;

public:
  void Init();

  Texture *LoadTexture(const wstring &key, const wstring &relative_path);
  Texture *FindTexture(const wstring &key);
  Texture *CreateTexture(const wstring &key, int width, int height);
  Sound *LoadSound(const wstring &key, const wstring &relative_path);
  Sound *FindSound(const wstring &key);
};

#endif // TOUHOU_ASSET_MANAGER_H_
