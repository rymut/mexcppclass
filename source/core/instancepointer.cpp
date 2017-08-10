/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "core/instancepointer.hpp"

namespace mexcppclass {

InstancePointer::InstancePointer() {
}

InstancePointer::InstancePointer(const InstancePointer &rhs)
  : smart_pointer_(rhs.smart_pointer_) {
}

InstancePointer::InstancePointer(const InstancePointer::SmartPointer &pointer)
  : smart_pointer_(pointer) {
}

InstancePointer::InstancePointer(Pointer pointer)
  : smart_pointer_(pointer) {
}

InstancePointer::~InstancePointer() {
}

InstancePointer &InstancePointer::operator=(const InstancePointer &rhs) {
  this->smart_pointer_ = rhs.smart_pointer_;
  return *this;
}

bool InstancePointer::operator<(const InstancePointer &rhs) const {
  return this->smart_pointer_ < rhs.smart_pointer_;
}

InstancePointer::Pointer InstancePointer::GetPointer() const {
  return this->smart_pointer_.get();
}

const InstancePointer::SmartPointer &InstancePointer::GetSmartPointer() const {
  return this->smart_pointer_;
}

}  // namespace mexcppclass
