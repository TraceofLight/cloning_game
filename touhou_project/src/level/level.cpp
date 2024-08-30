/**
 * @file level.cpp
 * @brief
 */

#include "include/level/level.h"

Level::~Level() {
  for (vector<Object*>& single_layer : layer_vector_)
    ReleaseVector(single_layer);
}

void Level::Tick() {
  for (vector<Object*>& single_layer : layer_vector_) {
    for (Object* layer_object : single_layer)
      layer_object->Tick();
  }
}

void Level::FinalTick() {
  for (vector<Object*>& single_layer : layer_vector_) {
    for (Object* layer_object : single_layer)
      layer_object->FinalTick();
  }
}

void Level::Render() {
  for (vector<Object*>& single_layer : layer_vector_) {
    for (Object* layer_object : single_layer)
      layer_object->Render();
  }
}

void Level::DeleteObjects(const LAYER_TYPE type) {
  for (const Object* layer_object : layer(type))
    delete layer_object;
}

void Level::DeleteAllObjects() {
  for (int i = 0; i < static_cast<int>(LAYER_TYPE::END); ++i) {
    DeleteObjects(static_cast<LAYER_TYPE>(i));
  }
}