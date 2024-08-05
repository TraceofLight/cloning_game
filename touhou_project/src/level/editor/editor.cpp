/**
 * @file editor.cpp
 * @brief
 */

#include "include/level/editor/editor.h"

#include "common/resource.h"
#include "include/core/engine/engine.h"
#include "include/object/ui/button/button.h"

void SampleFunction() { printf("this function is sample"); }

EditorLevel::EditorLevel() : Level() {
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

  Button *button_ui = new Button;
  button_ui->set_scale(Vector2(120, 50));
  button_ui->set_position(Vector2(10, 220));
  button_ui->set_call_back(SampleFunction);

  AddObjectToLayer(button_ui, LAYER_TYPE::UI);
}

void EditorLevel::Exit() { DeleteAllObjects(); }

void EditorLevel::Tick() {
  Level::Tick();
}