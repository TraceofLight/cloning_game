/**
 * @file macro.h
 * @brief 편하게 사용하기 위한 shortcut을 전역으로 관리하는 헤더
 */

#ifndef TOUHOU_MACRO_H_
#define TOUHOU_MACRO_H_

#define SINGLE(ClassName)                                                      \
public:                                                                        \
  static ClassName *Get() {                                                    \
    static ClassName manager_;                                                 \
    return &manager_;                                                          \
  }                                                                            \
                                                                               \
public:                                                                        \
  void operator=(const ClassName &other) = delete;                             \
                                                                               \
private:                                                                       \
  ClassName();                                                                 \
  ~ClassName();                                                                \
  ClassName(const ClassName &other) = delete;

#define CLONE(ClassName)                                                       \
  ClassName *Clone() { return new ClassName(*this); }

#define CLONE_DISABLE(ClassName)                                               \
  ClassName *Clone() { return nullptr; }                                       \
  ClassName(const ClassName &_Other) = delete;

#endif // TOUHOU_MACRO_H_
