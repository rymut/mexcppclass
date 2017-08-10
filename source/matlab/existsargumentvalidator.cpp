/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/existsargumentvalidator.hpp"

#include <string>

namespace mexcppclass {
namespace matlab {

ExistsArgumentValidator::ExistsArgumentValidator(const Argument &side) {
  this->side_ = side;
}

ExistsArgumentValidator::~ExistsArgumentValidator() {
}

const mxArray *const *const ExistsArgumentValidator::GetArguments(
  const mxArray *const *const input,
  const mxArray *const *const output) const {
  if (this->side_ == Argument::Input) {
    return input;
  }
  return output;
}

int ExistsArgumentValidator::GetArgumentsNumber(const int &input_count,
  const int &output_count) const {
  if (this->side_ == Argument::Input) {
    return input_count;
  }
  return output_count;
}

bool ExistsArgumentValidator::IsValid(
  const int &input_count, const mxArray * const * const input,
  const int output_count, const mxArray * const * const output,
  MessageDeque &list) const {
  return this->GetArgumentsNumber(input_count, output_count) >= 0;
}

Argument ExistsArgumentValidator::GetArgument() const {
  return this->side_;
}

std::string ExistsArgumentValidator::GetArgsName() const {
  if (this->side_ == Argument::Input) {
    return "argin";
  }
  return "argout";
}

}  // namespace matlab
}  // namespace mexcppclass
