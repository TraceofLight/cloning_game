/**
 * @file base.cpp
 * @brief
 */

#include "base.h"

uint32_t Base::global_serial_number_ = 0;

Base::Base()
  : id_(++global_serial_number_) {}

Base::Base(const Base& other)
  : id_(++global_serial_number_),
    name_(other.name_) {}

// end of base.cpp