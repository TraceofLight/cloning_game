/**
 * @file editor.cpp
 * @brief
 */

#include "include/level/editor/editor.h"

#include "common/resource.h"
#include "include/core/engine/engine.h"
#include "include/object/ui/button/button.h"

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

  Button* button_ui = new Button;
  button_ui->set_scale(Vector2(120, 50));
  button_ui->set_position(Vector2(10, 220));
  button_ui->set_call_back(SampleFunction);

  AddObjectToLayer(button_ui, LAYER_TYPE::UI);

  Player* player = new Player;
  player->set_name("reimu");
  Vector2 resolution = Engine::Get()->resolution();
  player->set_position({resolution.x() / 2, resolution.y() / 2});
  player->set_scale({256, 256});
  player->set_speed(100);

  AddObjectToLayer(player, LAYER_TYPE::DEFAULT);
}

void EditorLevel::Exit() {
  DeleteAllObjects();
}

void EditorLevel::Tick() {
  Level::Tick();
}