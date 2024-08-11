/**
 * @file debug_manager.cpp
 * @brief
 */

#include "include/manager/debug_manager.h"

#include "common/drawing_handle/drawing_handle.h"
#include "include/core/engine/engine.h"
#include "include/manager/key_manager.h"
#include "include/manager/time_manager.h"

DebugManager::DebugManager() : debug_mode_on_(false) {}

DebugManager::~DebugManager() = default;

void DebugManager::Tick() {
  if (KEY_CHECK(KEY::NUM_9, KEY_STATE::TAP)) {
    debug_mode_on_ ? debug_mode_on_ = false : debug_mode_on_ = true;
  }

  HDC back_dc = Engine::Get()->GetBackDC();

  // 디버그 쉐이프 정보들을 모두 그리기위해서 반복문을 돈다.
  for (auto iter = debug_display_list_.begin();
       iter != debug_display_list_.end(); ++iter) {
    if (debug_mode_on_) {
      DrawingHandle select_pen(back_dc, iter->pen);
      DrawingHandle select_brush(back_dc, iter->brush);

      switch (iter->shape) {
      case DEBUG_SHAPE::RECTANGLE:
        Rectangle(back_dc,
                  static_cast<int>(iter->position.x() - iter->scale.x() / 2.f),
                  static_cast<int>(iter->position.y() - iter->scale.y() / 2.f),
                  static_cast<int>(iter->position.x() + iter->scale.x() / 2.f),
                  static_cast<int>(iter->position.y() + iter->scale.y() / 2.f));
        break;
      case DEBUG_SHAPE::CIRCLE:
        Ellipse(back_dc,
                static_cast<int>(iter->position.x() - iter->scale.x() / 2.f),
                static_cast<int>(iter->position.y() - iter->scale.y() / 2.f),
                static_cast<int>(iter->position.x() + iter->scale.x() / 2.f),
                static_cast<int>(iter->position.y() + iter->scale.y() / 2.f));
        break;
      case DEBUG_SHAPE::CROSS:
        MoveToEx(back_dc,
                 static_cast<int>(iter->position.x() - iter->scale.x() / 2.f),
                 static_cast<int>(iter->position.y()), nullptr);
        LineTo(back_dc,
               static_cast<int>(iter->position.x() + iter->scale.x() / 2.f),
               static_cast<int>(iter->position.y()));
        MoveToEx(back_dc, static_cast<int>(iter->position.x()),
                 static_cast<int>(iter->position.y() - iter->scale.y() / 2.f),
                 nullptr);
        LineTo(back_dc, static_cast<int>(iter->position.x()),
               static_cast<int>(iter->position.y() + iter->scale.y() / 2.f));
        break;
      case DEBUG_SHAPE::LINE:
        MoveToEx(back_dc, static_cast<int>(iter->position.x()),
                 static_cast<int>(iter->position.y()), nullptr);
        LineTo(back_dc, static_cast<int>(iter->scale.x()),
               static_cast<int>(iter->scale.y()));
        break;
      }
    }
    iter->time += TimeManager::Get()->float_delta_time();
    if (iter->duration < iter->time) {
      iter = debug_display_list_.erase(iter);
    } else {
      ++iter;
    }
  }
}

void DebugManager::AddDebugInfo(const DebugShapeInfo &debug_info) {
  debug_display_list_.push_back(debug_info);
}
