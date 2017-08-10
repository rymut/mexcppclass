/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/dimensioncountargumentvalidator.hpp"
#include "mexcppclass/utility/string.hpp"

namespace mexcppclass {
namespace matlab {

DimensionCountArgumentValidator::DimensionCountArgumentValidator(
  const int &position,
  const size_t &minimum, const size_t &maximum) :
  ExistArgumentValidator(Argument::Input, position) {
  this->dim_minimum_ = minimum;
  this->dim_maximum_ = maximum;
}

bool DimensionCountArgumentValidator::IsValid(
  const mxArray * const array) const {
  if (array == nullptr) {
    return false;
  }
  const size_t array_dim = mxGetNumberOfDimensions(array);
  return array_dim >= this->dim_minimum_ && array_dim <= this->dim_maximum_;
}

bool DimensionCountArgumentValidator::IsValid(
  const int &nrhs, const mxArray * const * const prhs,
  const int nlhs, const mxArray * const * const plhs,
  MessageDeque &deque) const {
  if (ExistArgumentValidator::IsValid(nrhs, prhs, nlhs, plhs, deque) == false) {
    return false;
  }
  const mxArray *array = GetArgument(nrhs, prhs, nlhs, plhs);
  const size_t array_dim = mxGetNumberOfDimensions(array);
  if (array_dim < this->dim_minimum_) {
    deque.push_back(Message(MsgId::NotEnoughDimensions, "dimensionCount",
      utility::FormatString("dimension of %s must be"
        "equal or greater than %d", this->GetArgName(), this->dim_minimum_)));
    return false;
  }
  if (array_dim > this->dim_maximum_) {
    deque.push_back(Message(MsgId::NotEnoughDimensions, "dimensionCount",
      utility::FormatString("dimension of %s must be"
        "equal or less than %d", this->GetArgName(), this->dim_maximum_)));
    return false;
  }
  return true;
}

}  // namespace matlab
}  // namespace mexcppclass
