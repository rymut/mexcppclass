/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_DIMENSIONCOUNTARGUMENTVALIDATOR_HPP_
#define MEXCPPCLASS_MATLAB_DIMENSIONCOUNTARGUMENTVALIDATOR_HPP_

#include "mexcppclass/matlab/existargumentvalidator.hpp"

namespace mexcppclass {
namespace matlab {

class DimensionCountArgumentValidator : public ExistArgumentValidator {
 public:
  DimensionCountArgumentValidator(const int &position,
    const size_t &minimum = 1, const size_t &maximum = 1);

  bool IsValid(const mxArray * const array) const;

  virtual bool IsValid(const int &nrhs, const mxArray * const * const prhs,
    const int nlhs, const mxArray * const * const plhs,
    MessageDeque &deque = MessageDeque()) const;

 private:
  size_t dim_minimum_;
  size_t dim_maximum_;
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_DIMENSIONCOUNTARGUMENTVALIDATOR_HPP_
