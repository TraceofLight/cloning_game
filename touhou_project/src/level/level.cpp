/**
 * @file level.cpp
 * @brief
 */

#include "include/level/level.h"

Level::~Level() {
  for (int i = 0; i < static_cast<int>(LAYER_TYPE::END); ++i)
    ReleaseVector(layer_list_[i]);
}

void Level::Tick() {
  for (vector<Object *> single_layer : layer_list_) {
    for (Object *layer_object : single_layer)
      layer_object->Tick();
  }
}

void Level::FinalTick() {
  for (vector<Object *> single_layer : layer_list_) {
    for (Object *layer_object : single_layer)
      layer_object->FinalTick();
  }
}

void Level::Render() {
  for (vector<Object *> single_layer : layer_list_) {
    for (Object *layer_object : single_layer)
      layer_object->Render();
  }
}
void Level::DeleteObjects(LAYER_TYPE type) {
  for (const Object *layer_object : layer(type))
    delete layer_object;
}

void Level::DeleteAllObjects() {
  for (int i = 0; i < static_cast<int>(LAYER_TYPE::END); ++i) {
    DeleteObjects(static_cast<LAYER_TYPE>(i));
  }
}
