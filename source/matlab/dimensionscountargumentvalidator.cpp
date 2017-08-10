/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#include "mexcppclass/matlab/dimensionscountargumentvalidator.hpp"

#include <algorithm>
#include <vector>

#include "mexcppclass/utility/string.hpp"

namespace mexcppclass {
namespace matlab {

DimensionsCountArgumentValidator::DimensionsCountArgumentValidator(
  const int &position, int dims, const std::vector<mwSize> &minMax )
  : DimensionCountArgumentValidator(position, dims, dims) {
  this->maximum_.resize(dims, 0);
  this->minimum_.resize(dims, 0);
  for (int d = 0; d < std::min<mwSize>(dims, minMax.size()/2); ++d) {
    this->maximum_[d] = minMax[d*2 + 1];
    this->minimum_[d] = minMax[d*2 + 0];
  }
}

bool DimensionsCountArgumentValidator::IsValid(
  const int &nrhs, const mxArray * const * const prhs,
  const int nlhs, const mxArray * const * const plhs,
  MessageDeque &deque) const {
  if (DimensionCountArgumentValidator::IsValid(nrhs, prhs, nlhs, plhs, deque)
    == false) {
    return false;
  }
  const mxArray *array = this->GetArgument(nrhs, prhs, nlhs, plhs);
  if (array == nullptr) {  // should newer happen
    return false;
  }
  const size_t dim_count = mxGetNumberOfDimensions(array);
  const mwSize *dim_array = mxGetDimensions(array);

  for (int d = 0; d < std::min(dim_count, this->minimum_.size()); ++d) {
    if (dim_array[d] < this->minimum_[d]) {
      deque.push_back(Message(MsgId::DimensionLenghtToSmall,
        "dimensioncount",
        utility::FormatString("%s dimension %d must be"
          " equal or greater than %llu but is %llu", this->GetArgName(),
          d+1, this->minimum_[d], dim_array[d])));
      return false;
    }
    if (dim_array[d] > this->maximum_[d]) {
      deque.push_back(Message(MsgId::DimensionLenghtToBig,
        "dimensioncount",
        utility::FormatString("%s dimension %d must be "
          "equal or smaller than %llu but is %llu", this->GetArgName(),
          d+1, this->maximum_[d], dim_array[d])));
      return false;
    }
  }
  return true;
}

}  // namespace matlab
}  // namespace mexcppclass
