/**
 * @file macro.h
 * @brief 편하게 사용하기 위한 shortcut을 전역으로 관리하는 헤더
 */

#ifndef TOUHOU_MACRO_H_
#define TOUHOU_MACRO_H_

#define SINGLE(class_name)                                                     \
public:                                                                        \
  static class_name *Get() {                                                   \
    static class_name manager_;                                                \
    return &manager_;                                                          \
  }                                                                            \
                                                                               \
public:                                                                        \
  void operator=(const class_name &other) = delete;                            \
                                                                               \
private:                                                                       \
  class_name();                                                                \
  class_name(const class_name &other) = delete;                                \
  ~class_name();

#endif // TOUHOU_MACRO_H_
