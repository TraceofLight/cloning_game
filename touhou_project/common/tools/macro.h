/**
 * @file macro.h
 * @brief 편하게 사용하기 위한 shortcut을 전역으로 관리하는 헤더
 */

#ifndef TOUHOU_MACRO_H_
#define TOUHOU_MACRO_H_

#define SINGLE(ClassName)                                                                          \
 public:                                                                                           \
  static ClassName* Get() {                                                                        \
    static ClassName manager_;                                                                     \
    return &manager_;                                                                              \
  }                                                                                                \
                                                                                                   \
 public:                                                                                           \
  void operator=(const ClassName& other) = delete;                                                 \
                                                                                                   \
 private:                                                                                          \
  ClassName();                                                                                     \
  ~ClassName();                                                                                    \
  ClassName(const ClassName& other) = delete;

#define CLONE(ClassName)                                                                           \
  ClassName* Clone() {                                                                             \
    return new ClassName(*this);                                                                   \
  }

#define CLONE_DISABLE(ClassName)                                                                   \
  ClassName* Clone() {                                                                             \
    return nullptr;                                                                                \
  }                                                                                                \
  ClassName(const ClassName& _Other) = delete;

// 컴포넌트 속성 주입
#define MAKE_COMPONENT_TYPE(ComponentName)                                                         \
  static constexpr COMPONENT_TYPE type_ = COMPONENT_TYPE::ComponentName;

// Key Checker
// TIP: 통짜 구현과 이중 매크로가 차이가 날지라도 컴파일 타임 성능에만 영향을
// 미친다
// TODO(KHJ): 이중 매크로 쓰려고 했는데 컴파일 오류, 전처리기 처리 순서 확인해볼
// 것 #define KEY_TAP(key)		KEY_CHECK(key, KEY_STATE::TAP) #define
// KEY_PRESSED(key)	KEY_CHECK(key, KEY_STATE::PRESSED) #define
// KEY_RELEASED(key)	KEY_CHECK(key, KEY_STATE::RELEASED) #define
// KEY_NONE(key)		KEY_CHECK(key, KEY_STATE::NONE)
#define KEY_CHECK(key, state) KeyManager::Get()->key_state(key) == state

// GDI Macro
#define ADD_PEN(BrushColor, R, G, B)                                                               \
  pen_list_[static_cast<int>(BrushColor)] = CreatePen(PS_SOLID, 1, RGB(R, G, B))
#define ADD_BRUSH(BrushColor, R, G, B)                                                             \
  brush_list_[static_cast<int>(BrushColor)] = CreateSolidBrush(RGB(R, G, B))

#define LOG(level, message)                                                                        \
  wstring function_name;                                                                           \
  string str(__FUNCTION__);                                                                        \
  function_name = wstring(str.begin(), str.end());                                                 \
  wchar_t szBuff[255] = {};                                                                        \
  swprintf_s(szBuff, 255, L"Name: %s, { Line: %d }, %s", function_name.c_str(), __LINE__,          \
             message);                                                                             \
  if ((level) == LOG_LEVEL::STATUS)                                                                \
    DebugManager::Get()->AddLog(LogInfo{szBuff, level, 2.5});                                      \
  else                                                                                             \
    DebugManager::Get()->AddLog(LogInfo{szBuff, level, 0});

#endif // TOUHOU_MACRO_H_
