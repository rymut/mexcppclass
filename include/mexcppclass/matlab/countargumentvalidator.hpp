/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_COUNTARGUMENTVALIDATOR_HPP_
#define MEXCPPCLASS_MATLAB_COUNTARGUMENTVALIDATOR_HPP_

#include "mexcppclass/matlab/existsargumentvalidator.hpp"

namespace mexcppclass {
namespace matlab {

class CountArgumentValidator : public ExistsArgumentValidator {
 public:
  CountArgumentValidator(const Argument &side, const int &minimum,
    const int &maximum);

  ~CountArgumentValidator();

  virtual bool IsValid(const int &nrhs, const mxArray * const * const prhs,
    const int nlhs, const mxArray * const * const plhs,
    MessageDeque &deque = MessageDeque()) const;

 private:
  int minimum_;
  int maximum_;
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_COUNTARGUMENTVALIDATOR_HPP_
