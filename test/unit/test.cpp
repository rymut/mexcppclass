/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "test.hpp"
#include <mex.h>

namespace mexcppclass {
namespace test {
namespace unit {

TestInternal::TestInternal() {
}
TestInternal::~TestInternal() {
}

Test::Test() {
  this->value_ = 0;
}

Test::Test(const double &value) {
  this->value_ = value;
}

Test::Test(const Test &rhs) {
  this->value_ = rhs.value_;
}

Test::~Test() {
}

void Test::SetValue(const double &value) {
  this->value_ = value;
}

const double &Test::GetValue() const {
  return this->value_;
}

bool Test::IsEqual(const Test &object) const {
  return this->value_ == object.value_;
}

bool Test::IsEqual(const double &value) const {
  return this->value_ == value;
}

Test &Test::operator=(const Test &rhs) {
  this->value_ = rhs.value_;
  return *this;
}

}  // namespace unit
}  // namespace test
}  // namespace mexcppclass
