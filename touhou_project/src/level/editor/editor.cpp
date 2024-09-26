/**
 * @file editor.cpp
 * @brief
 */

#include "include/level/editor/editor.h"

#include "common/resource.h"
#include "include/core/engine/engine.h"
#include "include/object/ui/button/button.h"
#include "include/object/ui/rendering_box/rendering_box.h"

void SampleFunction() {
  printf("this function is sample\n");
}

EditorLevel::EditorLevel()
  : Level() {
  menu_handle_ = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_TOUHOUPROJECT));
}

EditorLevel::~EditorLevel() {
  DestroyMenu(menu_handle_);
  menu_handle_ = nullptr;
}

void EditorLevel::SaveFrame() {}

void EditorLevel::LoadFrame() {}

void EditorLevel::Init() {
  SetMenu(Engine::Get()->main_handle(), menu_handle_);

  RenderingBox* rendering_box = new RenderingBox;
  rendering_box->set_name("editor_rendering_box");
  rendering_box->set_scale({900, 710});
  rendering_box->set_position({300, 10});

  // animation save 버튼
  Button* save_animation_button = new Button;
  save_animation_button->set_name("editor_animation_save_button");
  save_animation_button->set_scale({120, 50});
  save_animation_button->set_position({10, 10});
  // save_animation_button->set_delegate(this, &EditorLevel::SaveAnimation);

  // animation load 버튼
  Button* load_animation_button = new Button;
  load_animation_button->set_name("editor_animation_load_button");
  load_animation_button->set_scale({120, 50});
  load_animation_button->set_position({10, 70});
  // load_animation_button->set_delegate(this, &EditorLevel::LoadAnimation);

  // asset load 버튼
  Button* asset_load_button = new Button;
  asset_load_button->set_name("editor_asset_load_button");
  asset_load_button->set_scale({120, 50});
  asset_load_button->set_position({150, 10});
  asset_load_button->set_delegate(rendering_box, &RenderingBox::LoadAsset);

  // Player* player = new Player;
  // player->set_name("reimu");
  // Vector2 resolution = Engine::Get()->resolution();
  // player->set_position({resolution.x() / 2, resolution.y() / 2});
  // player->set_scale({256, 256});
  // player->set_speed(100);

  AddObjectToLayer(save_animation_button, LAYER_TYPE::UI);
  AddObjectToLayer(load_animation_button, LAYER_TYPE::UI);
  AddObjectToLayer(asset_load_button, LAYER_TYPE::UI);
  AddObjectToLayer(rendering_box, LAYER_TYPE::UI);
  // AddObjectToLayer(player, LAYER_TYPE::DEFAULT);
}

void EditorLevel::Exit() {
  DeleteAllObjects();
}

void EditorLevel::Tick() {
  Level::Tick();
}

// end of editor.cpp