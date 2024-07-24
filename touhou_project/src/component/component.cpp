/**
 * @file component.cpp
 * @brief
 */

#include "include/component/component.h"

Component::Component(COMPONENT_TYPE type)
    : component_type_(type), owner_(nullptr) {}

Component::Component(const Component &other) = default;
