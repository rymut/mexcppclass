/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "core/nonpersistentdata.hpp"

namespace mexcppclass {

NonpersistentData::NonpersistentData(const int &arg_in_count,
  const int &arg_in_offset, const mxArray * const * const arg_in,
  const int &arg_out_count, const int &arg_out_offset, mxArray **arg_out) :
  arg_in_count_(arg_in_count), arg_out_count_(arg_out_count),
  arg_in_offset_(arg_in_offset), arg_out_offset_(arg_out_offset),
  arg_in_(arg_in), arg_out_(arg_out) {
}

NonpersistentData::~NonpersistentData() {
}

void NonpersistentData::SetArgOut(const int &position, mxArray *value) {
  this->arg_out_[position] = value;
}

const mxArray * const * const NonpersistentData::ArgOut() const {
  return this->arg_out_;
}

mxArray **NonpersistentData::ArgOut() {
  return this->arg_out_;
}

const mxArray * const * const NonpersistentData::ArgIn() const {
  return this->arg_in_;
}

const int &NonpersistentData::ArgInCount() const {
  return this->arg_in_count_;
}

const int &NonpersistentData::ArgOutCount() const {
  return this->arg_out_count_;
}

const int &NonpersistentData::ArgInOffset() const {
  return this->arg_in_offset_;
}

const int &NonpersistentData::ArgOutOffset() const {
  return this->arg_out_offset_;
}

}  // namespace mexcppclass
