/// Copyright (C) 2017 by Boguslaw Rymut
///
/// This file is distributed under the Apache 2.0 License
/// See LICENSE for details.

#ifndef MEXCPPCLASS_MATLAB_EXISTARGUMENTVALIDATOR_HPP_
#define MEXCPPCLASS_MATLAB_EXISTARGUMENTVALIDATOR_HPP_

#include <mex.h>
#include <string>
#include "mexcppclass/matlab/existsargumentvalidator.hpp"

namespace mexcppclass {
namespace matlab {

class ExistArgumentValidator : public ExistsArgumentValidator {
 public:
  ExistArgumentValidator(const Argument &type, const int &position);

  ~ExistArgumentValidator();

  const mxArray *GetArgument(
    const int &nrhs, const mxArray * const * const prhs,
    const int &nlhs, const mxArray * const * const plhs) const;

  bool IsValid(const int &nrhs, const mxArray * const * const prhs,
    const int nlhs, const mxArray * const * const plhs,
    MessageDeque &deque = MessageDeque()) const;

 protected:
  int GetPosition() const;

  Argument GetArgument() const;

  std::string GetArgName() const;

 private:
  Argument argument_;
  int position_;
};

}  // namespace matlab
}  // namespace mexcppclass

#endif  // MEXCPPCLASS_MATLAB_EXISTARGUMENTVALIDATOR_HPP_
