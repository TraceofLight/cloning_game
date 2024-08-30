/**
 * @file asset_manager.cpp
 * @brief
 */

#include "include/manager/asset_manager.h"
#include "include/asset/texture/texture.h"

AssetManager::AssetManager() = default;
AssetManager::~AssetManager() = default;

void AssetManager::Init() {}

/**
 * @brief Texture를 File로부터 Load하는 함수
 * Key와 Path를 인자로 받는데 해당 Texture가 존재한다면 Key를 통해 불러오고,
 * 없다면 File Path를 통해 불러온 뒤 Key와 연결하여 Map에 넣는다.
 * @param key
 * @param relative_path
 * @return Texture* / nullptr
 */
Texture* AssetManager::LoadTexture(const wstring& key, const wstring& relative_path) {
  Texture* texture_ptr = FindTexture(key);

  if (texture_ptr != nullptr)
    return texture_ptr;

  // TODO(KHJ): path 불러올 수 있도록 처리할 것
  wstring file_path = L"foo";
  file_path.append(relative_path);

  // path를 가지고 직접 load 해보고 불가능하다면 nullptr 반환
  // TODO(KHJ): 임시 Texture를 반환하는 방법은 없을까?
  std::unique_ptr<Texture, HandleObjectDeleterWrapper> unique_texture_ptr(
      new Texture, HandleObjectDeleterWrapper());
  if (FAILED(texture_ptr->Load(file_path))) {
    return nullptr;
  }

  // 여기서 hash_로 unique_ptr의 소유권이 넘어간다.
  unique_texture_ptr->set_key(key);
  unique_texture_ptr->set_relative_path(relative_path);
  texture_hash_.emplace(key, std::move(unique_texture_ptr));

  // 새로 Find를 진행해서 받아온 원시 포인터를 반환
  texture_ptr = FindTexture(key);
  return texture_ptr;
}

/**
 * @brief Texture Map에서 Key를 통해 Texture를 탐색하는 함수
 * @param key
 * @return Texture* / nullptr
 */
Texture* AssetManager::FindTexture(const wstring& key) {
  auto const iter = texture_hash_.find(key);
  if (iter == texture_hash_.end())
    return nullptr;
  return iter->second.get();
}

/**
 * @brief 파라미터로 받은 크기의 Texture를 생성하는 함수
 * @param key
 * @param width
 * @param height
 * @return Texture* / nullptr
 */
Texture* AssetManager::CreateTexture(const wstring& key, int width, int height) {
  Texture* texture = FindTexture(key);
  // Create는 존재하지 않는 Texture를 만들기 위한 메서드
  assert(texture == nullptr);

  texture = new Texture;
  texture->Create(width, height);

  return texture;
}

/**
 * @brief Sound를 File로부터 Load하는 함수
 * Key와 Path를 인자로 받는데 해당 Sound가 존재한다면 Key를 통해 불러오고,
 * 없다면 File Path를 통해 불러온 뒤 Key와 연결하여 Map에 넣는다.
 * @param key
 * @param relative_path
 * @return Sound* / nullptr
 * TODO(KHJ): Template로 변경할 수 있다면 시도 후 테스트 해보는 것도 괜찮을 듯
 */
Sound* AssetManager::LoadSound(const wstring& key, const wstring& relative_path) {
  Sound* sound_ptr = FindSound(key);

  if (sound_ptr != nullptr)
    return sound_ptr;

  // TODO(KHJ): path 불러올 수 있도록 처리할 것
  wstring file_path = L"foo";
  file_path.append(relative_path);

  // path를 가지고 직접 load 해보고 불가능하다면 nullptr 반환
  unique_ptr<Sound, HandleObjectDeleterWrapper> unique_sound_ptr(new Sound,
                                                                 HandleObjectDeleterWrapper());
  if (FAILED(sound_ptr->Load(file_path))) {
    return nullptr;
  }

  // 여기서 hash_로 unique_ptr의 소유권이 넘어간다.
  unique_sound_ptr->set_key(key);
  unique_sound_ptr->set_relative_path(relative_path);
  sound_hash_.emplace(key, std::move(unique_sound_ptr));

  // 새로 Find를 진행해서 받아온 원시 포인터를 반환
  sound_ptr = FindSound(key);
  return sound_ptr;
}

/**
 * @brief Sound Map에서 Key를 통해 Texture를 탐색하는 함수
 * @param key
 * @return Sound* / nullptr
 */
Sound *AssetManager::FindSound(const wstring &key) {
  auto const iter = sound_hash_.find(key);
  if (iter == sound_hash_.end())
    return nullptr;
  return iter->second.get();
}