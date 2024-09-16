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

void DebugManager::Render() {
  if (KEY_CHECK(KEY::NUM_9, KEY_STATE::TAP)) {
    debug_mode_on_ ? debug_mode_on_ = false : debug_mode_on_ = true;
  }

  HDC back_dc = Engine::Get()->GetBackDC();

  // 디버그 쉐이프 정보들을 모두 그리기위해서 반복문을 돈다.
  for (auto iter = debug_display_list_.begin(); iter != debug_display_list_.end(); ++iter) {
    if (debug_mode_on_) {
      DrawingHandle select_pen(back_dc, iter->pen);
      DrawingHandle select_brush(back_dc, iter->brush);

      switch (iter->shape) {
        case DEBUG_SHAPE::RECTANGLE:
          Rectangle(back_dc, static_cast<int>(iter->position.x() - iter->scale.x() / 2.f),
                    static_cast<int>(iter->position.y() - iter->scale.y() / 2.f),
                    static_cast<int>(iter->position.x() + iter->scale.x() / 2.f),
                    static_cast<int>(iter->position.y() + iter->scale.y() / 2.f));
          break;
        case DEBUG_SHAPE::CIRCLE:
          Ellipse(back_dc, static_cast<int>(iter->position.x() - iter->scale.x() / 2.f),
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
          LineTo(back_dc, static_cast<int>(iter->scale.x()), static_cast<int>(iter->scale.y()));
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

  // log 출력
  int i = 0;
  for (auto iter = log_list_.begin(); iter != log_list_.end(); ++i) {
    // TODO(KHJ): 전역 혹은 멤버 변수화 해야하는지 검토할 것
    // 상수 설정
    uint8_t log_step = 18;
    float log_retention_period = 3;

    Vector2 log_position = Vector2(10, 10 + i * log_step);

    // 로그 수준에따라 색상을 다르게 한다.
    switch (iter->log_level) {
      case LOG_LEVEL::STATUS:
        SetTextColor(Engine::Get()->GetBackDC(), RGB(254, 190, 152));
        break;
      case LOG_LEVEL::LOG:
        SetTextColor(Engine::Get()->GetBackDC(), RGB(220, 220, 220));
        break;
      case LOG_LEVEL::WARNING:
        SetTextColor(Engine::Get()->GetBackDC(), RGB(220, 220, 30));
        break;
      case LOG_LEVEL::BUG:
        SetTextColor(Engine::Get()->GetBackDC(), RGB(240, 30, 30));
        break;
    }

    SetBkMode(Engine::Get()->GetBackDC(), TRANSPARENT);
    wstring wide_string(iter->log_text.begin(), iter->log_text.end());
    TextOut(Engine::Get()->GetBackDC(), static_cast<int>(log_position.x()),
            static_cast<int>(log_position.y()), wide_string.c_str(),
            static_cast<int>(iter->log_text.length()));

    // 로그의 수명이 다 끝났으면 제거할 리스트에 올려둠
    iter->time += TimeManager::Get()->float_delta_time();
    if (log_retention_period < iter->time)
      iter = log_list_.erase(iter);
    else
      ++iter;
  }
}

void DebugManager::AddDebugInfo(const DebugShapeInfo& debug_info) {
  debug_display_list_.push_back(debug_info);
}

void DebugManager::AddLog(const LogInfo& log) {
#ifdef _DEBUG
  log_list_.push_front(log);
#endif
}