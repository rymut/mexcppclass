/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/existargumentvalidator.hpp"
#include <string>
#include "mexcppclass/utility/string.hpp"

namespace mexcppclass {
namespace matlab {

ExistArgumentValidator::ExistArgumentValidator(const Argument &type,
  const int &position) : ExistsArgumentValidator(type) {
  this->argument_ = type;
  this->position_ = position;
}


ExistArgumentValidator::~ExistArgumentValidator() {
}

const mxArray *ExistArgumentValidator::GetArgument(const int &nrhs,
  const mxArray * const * const prhs,
  const int &nlhs, const mxArray * const * const plhs) const {
  if (this->argument_ == Argument::Output &&
    nlhs > this->position_ && plhs != nullptr ) {
    return plhs[this->position_];
  }
  if (this->argument_ == Argument::Input &&
    nrhs > this->position_ && prhs != nullptr) {
    return prhs[this->position_];
  }
  return nullptr;
}

bool ExistArgumentValidator::IsValid(const int &nrhs,
  const mxArray * const * const prhs,
  const int nlhs, const mxArray * const * const plhs,
  MessageDeque &deque) const {
    const mxArray *array = this->GetArgument(nrhs, prhs, nlhs, plhs);
  if (array == nullptr) {
    deque.push_back(Message(MsgId::RequiredArgumentMissing, "exist",
      utility::FormatString("%s is required", this->GetArgName())));
    return false;
  }
  return true;
}

int ExistArgumentValidator::GetPosition() const {
  return this->position_;
}
Argument ExistArgumentValidator::GetArgument() const {
  return this->argument_;
}

std::string ExistArgumentValidator::GetArgName() const {
  if (this->argument_ == Argument::Input) {
    return utility::FormatString("argin(%d)", this->GetPosition() + 1);
  } else {
    return utility::FormatString("argout(%d)", this->GetPosition() + 1);
  }
}

}  // namespace matlab
}  // namespace mexcppclass
