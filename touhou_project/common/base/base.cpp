/**
 * @file base.cpp
 * @brief
 */

#include "base.h"

UINT Base::global_serial_number_ = 0;

Base::Base() : id_(++global_serial_number_) {}

Base::Base(const Base &other)
    : id_(++global_serial_number_), name_(other.name_) {}
