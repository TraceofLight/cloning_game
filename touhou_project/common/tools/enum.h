/**
 * @file enum.h
 * @brief 개발하면서 사용할 enum들을 모아둔 헤더
 */

#ifndef TOUHOU_ENUM_H_
#define TOUHOU_ENUM_H_

enum class PEN_TYPE {
  RED,
  GREEN,
  BLUE,
  YELLOW,
  MAGENTA,
  END,
};

enum class BRUSH_TYPE {
  RED,
  BLUE,
  GREEN,
  GRAY,
  TURQUOISE,
  HOLLOW,
  END,
};

enum class KEY {
  W,
  S,
  A,
  D,

  LEFT,
  RIGHT,
  UP,
  DOWN,

  SPACE,
  ENTER,

  NUM_1,
  NUM_2,
  NUM_3,
  NUM_4,
  NUM_5,
  NUM_6,
  NUM_7,
  NUM_8,
  NUM_9,
  NUM_0,

  LBTN,
  RBTN,

  END,
};

enum class KEY_STATE {
  TAP,      // 누른 직후의 상태
  PRESSED,  // 눌린 상태
  RELEASED, // 떼어진 직후의 상태
  NONE,     // 눌리지 않은 상태
};

enum class COMPONENT_TYPE {
  COLLIDER,
  ANIMATOR,
};

enum class LAYER_TYPE {
  DEFAULT,
  UI,
  END,
};

enum class LEVEL_TYPE {
  START,
  EDITOR,
  END,
};

#endif // TOUHOU_ENUM_H_
