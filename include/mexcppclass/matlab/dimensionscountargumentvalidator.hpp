/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_DIMENSIONSCOUNTARGUMENTVALIDATOR_HPP_
#define MEXCPPCLASS_MATLAB_DIMENSIONSCOUNTARGUMENTVALIDATOR_HPP_

#include <mex.h>
#include <vector>

#include "mexcppclass/matlab/dimensioncountargumentvalidator.hpp"

namespace mexcppclass {
namespace matlab {

class DimensionsCountArgumentValidator
  : public DimensionCountArgumentValidator {
 public:
  DimensionsCountArgumentValidator(const int &position, int dims,
    const std::vector<mwSize> &minMax);

  virtual bool IsValid(const int &nrhs, const mxArray * const * const prhs,
    const int nlhs, const mxArray * const * const plhs,
    MessageDeque &deque = MessageDeque()) const;

 private:
  std::vector<mwSize> minimum_;
  std::vector<mwSize> maximum_;
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_DIMENSIONSCOUNTARGUMENTVALIDATOR_HPP_
